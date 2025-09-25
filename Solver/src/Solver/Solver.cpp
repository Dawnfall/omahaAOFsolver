#include "Solver.h"

#include "Data/SolverData.h"
#include "Solver/PokerUtils.h"
#include "Position.h"
#include "RandomGenerator.h"

void Solver::SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, SolverResult)> callback)
{
	std::thread([solverParams, callback]() {
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

		SolverData solverData(solverParams);

		auto [range, evs] = BuildRangeAndEvs(solverParams, solverData);
		auto threadTask = [&solverParams, &solverData, &range, &evs]() {
			ExecuteThreadTask(solverParams, solverData, range, evs);
			};

		std::vector<std::thread> threads;
		for (unsigned int i = 0; i < solverParams.threadCount; ++i)
			threads.emplace_back(threadTask);

		for (auto& thread : threads)
			thread.join();

		// Prepare the result
		std::chrono::duration<double> duration = std::chrono::high_resolution_clock::now() - startTime;

		SolverResult results;
		results.IsSuccess = true;
		results.Solution = std::make_shared<Solution>(solverParams, range, evs);
		results.Duration = duration.count();

		// Invoke the callback with the solution
		if (callback)
		{
			callback(solverParams, std::move(results));
		}
		}).detach(); // Detach the thread to allow it to run independently
}

void Solver::ExecuteThreadTask(const SolverParams& solverParams, SolverData& solverData,
	const std::vector<uint8_t>& range, std::vector<float>& evs)
{
	thread_local RandomGenerator randGen;

	Board_TR turnAndRiver;
	std::unordered_set<uint8_t> removedFlop{ solverParams.flop[0], solverParams.flop[1], solverParams.flop[2] };

	for (size_t repeatIter = 0; repeatIter < solverData.repeatIters; ++repeatIter)
	{
		for (unsigned int rangeIndex = 0; rangeIndex < solverData.totalRanges; rangeIndex++)
		{
			float evFold = PokerUtils::GetFoldLoss(rangeIndex, solverParams);

			while (solverData.handIndex < static_cast<int>(PokerUtils::rangeSize))
			{
				unsigned int start = solverData.handIndex.fetch_add(solverParams.chunkSize);
				unsigned int end = std::min(start + solverParams.chunkSize, PokerUtils::rangeSize);

				for (size_t handIndex = start; handIndex < end; ++handIndex)
				{
					std::vector<size_t> hands{ handIndex };
					std::unordered_set<uint8_t> removedCards = removedFlop;
					removedCards.insert(range.begin() + handIndex * 4, range.begin() + handIndex * 4 + 4);

					float handEV = CalcBbEvForAiHand(rangeIndex, range, evs, solverParams, solverData,
						removedCards, hands, turnAndRiver, randGen, repeatIter);
					evs[handIndex * solverData.totalRanges + rangeIndex] = handEV + evFold + solverParams.margin;
				}
			}
			solverData.syncPoint.arrive_and_wait();
			solverData.handIndex = 0;
			solverData.syncPoint.arrive_and_wait();
		}
	}
}

std::tuple<std::vector<uint8_t>, std::vector<float>> Solver::BuildRangeAndEvs(const SolverParams& solParams, SolverData& solverData)
{
	std::vector<uint8_t> range;
	range.reserve(PokerUtils::rangeSize * 4);
	std::vector<float> evs;
	evs.reserve(PokerUtils::rangeSize * solverData.totalRanges);

	std::unordered_set<uint8_t> removedCards;
	removedCards.insert(solParams.flop.begin(), solParams.flop.end());

	int i0, i1, i2, i3;
	for (i0 = 51; i0 >= 3; --i0)
	{
		if (removedCards.contains(i0))
			continue;
		for (i1 = i0 - 1; i1 >= 2; --i1)
		{
			if (removedCards.contains(i1))
				continue;
			for (i2 = i1 - 1; i2 >= 1; --i2)
			{
				if (removedCards.contains(i2))
					continue;
				for (i3 = i2 - 1; i3 >= 0; --i3)
				{
					if (removedCards.contains(i3))
						continue;
					range.emplace_back((uint8_t)i0);
					range.emplace_back((uint8_t)i1);
					range.emplace_back((uint8_t)i2);
					range.emplace_back((uint8_t)i3);
				}
			}
		}
	}
	for (unsigned int i = 0; i < solverData.totalRanges * PokerUtils::rangeSize; i++)
		evs.emplace_back(0.0f);

	return { range,evs };
}

float Solver::CalcBbEvForAiHand(size_t rangeIndex, const std::vector<uint8_t>& range, const std::vector<float>& evs, const SolverParams& solParams, SolverData& solverData, const std::unordered_set<uint8_t>& removedCards, const std::vector<size_t>& hands, Board_TR& turnAndRiver, RandomGenerator& randGen, size_t currRepeat)
{
	float bbEv = 0.0f;

	unsigned int handIter = 0;
	while (handIter++ < solverData.maxHandIters[currRepeat])
	{
		std::unordered_set<uint8_t> removed = removedCards;
		std::vector<size_t> aiHands = hands;
		for (const size_t prevAIRange : PokerUtils::GetAllFromAiRanges(rangeIndex, solParams.totalPlayers)) //determine hands for previous players
		{
			int prevHandIndex = Solver::ForcePickHand(range, evs, prevAIRange, solverData.totalRanges, removed, randGen, true); //previous players are always all in
			aiHands.emplace_back(prevHandIndex);
			removed.insert(range.begin() + prevHandIndex * 4, range.begin() + prevHandIndex * 4 + 4);
		}

		if (currRepeat > 4) //only matters after EV has been at least somewhat balanced
		{
			//previous players that folded still pick hand! so that certain cards are removed more often
			// for example if board is 335, players never fold 3, so when they do...3 becomes more likely for further players
			for (const size_t prevFoldRange : PokerUtils::GetAllFromFoldRanges(rangeIndex, solParams.totalPlayers))
			{
				int prevHandIndex = Solver::ForcePickHand(range, evs, prevFoldRange, solverData.totalRanges, removed, randGen, false);
				removed.insert(range.begin() + prevHandIndex * 4, range.begin() + prevHandIndex * 4 + 4);
			}
		}

		size_t currRange = rangeIndex;
		int nextRange = static_cast<int>(currRange);
		bool isAI = true;
		while (true) //find hands for further players
		{
			nextRange = PokerUtils::GetToRange(currRange, isAI, solParams.totalPlayers);
			if (nextRange == -1)
				break;

			int nextHandIndex = Solver::GetRandomHand(range, removed, randGen);
			if (evs[solverData.totalRanges * nextHandIndex + nextRange] >= 0)
			{
				aiHands.emplace_back(nextHandIndex);
				removed.insert(range.begin() + nextHandIndex * 4, range.begin() + nextHandIndex * 4 + 4);
				isAI = true;
			}
			else
				isAI = false;
			currRange = nextRange;
		}

		float pot = PokerUtils::GetPotAfter(currRange, solParams, isAI);
		float ev = CalculateEquityOnFlop(aiHands, range, solParams.flop, turnAndRiver, removed, randGen);
		bbEv += ev * pot - solParams.stackSize; //sb and bb need adjustmend

		if (handIter >= solverData.minHandIters[currRepeat] && handIter % 5 == 0) //optimization for big winners/loosers
		{
			float currEv = bbEv / handIter;
			if (std::abs(currEv) > PokerUtils::GetMinWinrate(rangeIndex, solParams.totalPlayers, handIter, solverData.is99Conf[currRepeat]))
				return currEv;
		}
	}
	return bbEv / static_cast<float>(solverData.maxHandIters[currRepeat]);
}

inline float Solver::CalculateEquityOnFlop(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& ranges, const Board_F& flop, Board_TR& turnAndRiver,
	const std::unordered_set<uint8_t> removedCards, RandomGenerator& randGen)
{
	if (handIndices.size() == 1)
		return 1.0f;

	while (true)
	{
		turnAndRiver[0] = randGen.GetRandomCardFromDeck();
		if (!removedCards.contains(turnAndRiver[0]))
			break;
	}
	while (true)
	{
		turnAndRiver[1] = randGen.GetRandomCardFromDeck();
		if (turnAndRiver[0] != turnAndRiver[1] && !removedCards.contains(turnAndRiver[1]))
			break;
	}

	return CalcEqOfHand1(handIndices, ranges, flop, turnAndRiver);
}

inline float Solver::CalcEqOfHand1(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& range,
	const Board_F& flop, const std::array<uint8_t, 2>& turnAndRiver)
{
	std::vector<Rank> ranks;
	ranks.reserve(handIndices.size());
	ranks.emplace_back(evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], range[handIndices[0] * 4], range[handIndices[0] * 4 + 1], range[handIndices[0] * 4 + 2], range[handIndices[0] * 4 + 3]));

	int equalCount = 1;
	for (int i = 1; i < handIndices.size(); i++)
	{
		ranks.emplace_back(evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], range[handIndices[i] * 4], range[handIndices[i] * 4 + 1], range[handIndices[i] * 4 + 2], range[handIndices[i] * 4 + 3]));

		if (ranks.back() > ranks[0])
			return 0.0f;
		if (ranks.back() == ranks[0])
			equalCount += 1;
	}
	return 1.0f / equalCount;
}