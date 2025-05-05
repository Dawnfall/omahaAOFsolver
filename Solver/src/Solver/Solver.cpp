#include "Solver.h"

#include "Data/SolverData.h"
#include "Solver/PokerUtils.h"
#include "Position.h"

void Solver::SolveAsync(const SolverParams& solverParams, std::function<void(const SolverParams&, SolverResult)> callback)
{
	std::thread([solverParams, callback]() {
		std::chrono::steady_clock::time_point startTime = std::chrono::steady_clock::now();

		SolverData solverData(solverParams);

		std::vector<std::minstd_rand> randGens;
		for (int i = 0; i < solverParams.threadCount; i++)
			randGens.emplace_back(std::random_device{}());

		auto [range, evs] = BuildRangeAndEvs(solverParams, solverData);

		auto threadTask = [&solverParams, &solverData, &range, &evs](std::minstd_rand& randGen)
			{
				Board_TR turnAndRiver;

				for (unsigned int repeatIter = 0; repeatIter < PokerUtils::maxRepeatIters; ++repeatIter)
				{
					auto [perHandIters, is99Conf] = PokerUtils::GetHandItersAndIs99Confidence(repeatIter);

					for (unsigned int rangeIndex = 0; rangeIndex < solverData.totalRanges; rangeIndex++)
					{
						while (solverData.handIndex < static_cast<int>(PokerUtils::rangeSize))
						{
							unsigned int start = solverData.handIndex.fetch_add(solverParams.chunkSize);
							unsigned int end = std::min(start + solverParams.chunkSize, PokerUtils::rangeSize);

							for (size_t handIndex = start; handIndex < end; ++handIndex)
							{
								std::vector<size_t> hands{ handIndex };
								std::unordered_set<uint8_t> removedCards;
								removedCards.insert(range.begin() + handIndex * 4, range.begin() + handIndex * 4 + 4);

								evs[handIndex * solverData.totalRanges + rangeIndex] =
									CalcBbEvForAiHand(rangeIndex, range, evs, solverParams, solverData, removedCards, hands, turnAndRiver, randGen, perHandIters, is99Conf, rangeIndex>2);
							}
						}
						solverData.syncPoint.arrive_and_wait();
						solverData.handIndex = 0;
						solverData.syncPoint.arrive_and_wait();
					}
				}
			};

		std::vector<std::thread> threads;
		for (unsigned int i = 1; i < solverParams.threadCount; ++i)
			threads.emplace_back(threadTask, std::ref(randGens[i]));
		threadTask(randGens[0]); //this thread also does the job

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

std::tuple<std::vector<uint8_t>, std::vector<float>> Solver::BuildRangeAndEvs(const SolverParams& solParams, SolverData& solverData)//, std::minstd_rand& randGen)
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
	for (int i = 0; i < solverData.totalRanges * PokerUtils::rangeSize; i++)
		evs.emplace_back(1.0f);
	//evs.emplace_back(static_cast<float>(PokerUtils::GetRandomEv(randGen)));

	return { range,evs };
}

float Solver::CalcBbEvForAiHand(size_t rangeIndex, const std::vector<uint8_t>& range, const std::vector<float>& evs, const SolverParams& solParams, SolverData& solverData, const std::unordered_set<uint8_t>& removedCards, const std::vector<size_t>& hands, Board_TR& turnAndRiver, std::minstd_rand& randGen, size_t totalPerHandIters, bool is99Conf, bool doRemoveFolded)
{
	float evFold = PokerUtils::GetFoldLoss(rangeIndex, solParams);

	float bbEv = 0.0f;
	unsigned int handIter = 0;
	while (handIter++ < totalPerHandIters)
	{
		float pot = solParams.sb + solParams.bb + PokerUtils::GetRangeAiSize(rangeIndex, solParams);
		std::unordered_set<uint8_t> removed = removedCards;
		std::vector<size_t> aiHands = hands;
		for (const size_t prevAIRange : PokerUtils::GetAllFromAiRanges(rangeIndex, solParams.totalPlayers))
		{
			int prevHandIndex = Solver::ForcePickHand(range, evs, prevAIRange, solverData.totalRanges, removed, randGen, true);
			aiHands.emplace_back(prevHandIndex);
			removed.insert(range.begin() + prevHandIndex * 4, range.begin() + prevHandIndex * 4 + 4);
			pot += PokerUtils::GetRangeAiSize(prevAIRange, solParams);
		}

		if (doRemoveFolded)
		{
			for (const size_t prevFoldRange : PokerUtils::GetAllFromFoldRanges(rangeIndex, solParams.totalPlayers))
			{
				int prevHandIndex = Solver::ForcePickHand(range, evs, prevFoldRange, solverData.totalRanges, removed, randGen, false);
				removed.insert(range.begin() + prevHandIndex * 4, range.begin() + prevHandIndex * 4 + 4);
			}
		}

		size_t currNodeIndex = rangeIndex;
		size_t nextNodeIndex;
		bool isAI = true;
		while (PokerUtils::GetToRange(currNodeIndex, nextNodeIndex, isAI, solParams.totalPlayers))
		{
			int nextHandIndex = Solver::GetRandomHand(range, removed, randGen);
			if (evs[solverData.totalRanges * nextHandIndex + nextNodeIndex] > 0)
			{
				aiHands.emplace_back(nextHandIndex);
				removed.insert(range.begin() + nextHandIndex * 4, range.begin() + nextHandIndex * 4 + 4);
				pot += PokerUtils::GetRangeAiSize(nextNodeIndex, solParams);
				isAI = true;
			}
			else
				isAI = false;
			currNodeIndex = nextNodeIndex;
		}

		float ev = CalculateEquityOnFlop(aiHands, range, solParams.flop, turnAndRiver, removed, randGen);
		bbEv += ev * pot - solParams.stackSize;


		if (handIter % 5 == 0)
		{
			float currEv = bbEv / handIter - evFold + solParams.margin;
			if (std::abs(currEv) > PokerUtils::GetMinWinrate(rangeIndex, solParams.totalPlayers, handIter, is99Conf))
				return currEv;
		}
	}
	return bbEv / handIter - evFold + solParams.margin;
}

inline float Solver::CalculateEquityOnFlop(const std::vector<size_t>& handIndices, const std::vector<uint8_t>& ranges, const Board_F& flop, Board_TR& turnAndRiver, const std::unordered_set<uint8_t> removedCards, std::minstd_rand& randGen)
{
	if (handIndices.size() == 1)
		return 1.0f;

	while (true)
	{
		turnAndRiver[0] = PokerUtils::GetRandomCardFromDeck(randGen);
		if (!removedCards.contains(turnAndRiver[0]))
			break;
	}
	while (true)
	{
		turnAndRiver[1] = PokerUtils::GetRandomCardFromDeck(randGen);
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
		size_t handIndex = handIndices[i];
		ranks.emplace_back(evaluate_plo4_cards(flop[0], flop[1], flop[2], turnAndRiver[0], turnAndRiver[1], range[handIndices[i] * 4], range[handIndices[i] * 4 + 1], range[handIndices[i] * 4 + 2], range[handIndices[i] * 4 + 3]));

		if (ranks.back() > ranks[0])
			return 0.0f;
		if (ranks.back() == ranks[0])
			equalCount += 1;
	}
	return 1.0f / equalCount;
}