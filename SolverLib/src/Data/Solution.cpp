#include "Data/Solution.h"
#include "PokerUtils.h"
#include "RandomGenerator.h"

std::string Solution::GetRangeName(int rangeIndex)const
{
	return PokerUtils::GetRangeName(rangeIndex, PlayerCount);
}
std::tuple<Hand, float> Solution::GetRandomHandAndEv(int rangeIndex, RandomGenerator& randGen)const
{
	size_t randomHandIndex = randGen.GetRandomHandFromRange();
	return GetHandAndEv(rangeIndex, randomHandIndex);
}
std::tuple<Hand, float> Solution::GetHandAndEv(int node, int handIndex)const
{
	return
	{
		Hand{Hands[handIndex*4],Hands[handIndex*4 + 1],Hands[handIndex*4 + 2],Hands[handIndex*4 + 3]},
		Evs[handIndex * PokerUtils::GetTotalRanges(PlayerCount) + node]
	};
}


void Solution::Serialize(const std::string& folderPath, const Solution& solution)
{
	std::string filePath = folderPath + "/" + solution.GetName() + ".sol";
	if (std::filesystem::exists(filePath))
		std::filesystem::remove(filePath);

	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile)
		throw std::runtime_error("Failed to open file for writing");

	outFile.write(reinterpret_cast<const char*>(&solution.PlayerCount), sizeof(unsigned int));
	outFile.write(reinterpret_cast<const char*>(&solution.BbSize), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.SbSize), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.Margin), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[0]), sizeof(uint8_t));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[1]), sizeof(uint8_t));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[2]), sizeof(uint8_t));

	for (const int8_t& card : solution.Hands)
	{
		outFile.write(reinterpret_cast<const char*>(&card), sizeof(uint8_t));
	}
	for (float ev : solution.Evs)
	{
		outFile.write(reinterpret_cast<const char*>(&ev), sizeof(float));
	}
	outFile.close();
}

std::shared_ptr<Solution> Solution::Deserialize(const std::string& filePath)
{
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile)
		throw std::runtime_error("Failed to open file for reading");

	std::shared_ptr<Solution> solution = std::make_shared<Solution>();

	inFile.read(reinterpret_cast<char*>(&solution->PlayerCount), sizeof(unsigned int));
	inFile.read(reinterpret_cast<char*>(&solution->BbSize), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->SbSize), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->Margin), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[0]), sizeof(uint8_t));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[1]), sizeof(uint8_t));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[2]), sizeof(uint8_t));

	size_t totalRanges = PokerUtils::GetTotalRanges(solution->PlayerCount);
	size_t totalEvs = totalRanges * PokerUtils::rangeSize;

	solution->Hands.reserve(PokerUtils::rangeSize * 4);
	solution->Hands.reserve(PokerUtils::rangeSize * totalRanges);

	for (int i = 0; i < PokerUtils::rangeSize * 4; i++)
	{
		int8_t card;
		inFile.read(reinterpret_cast<char*>(&card), sizeof(uint8_t));
		solution->Hands.emplace_back(card);
	}

	for (int i = 0; i < totalEvs; i++)
	{
		float ev;
		inFile.read(reinterpret_cast<char*>(&ev), sizeof(float));
		solution->Evs.emplace_back(ev);
	}

	inFile.close();
	return solution;
}