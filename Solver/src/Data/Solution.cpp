
#include "Core.h"
#include "Solution.h"
#include "Solver/Node.h"


// Serialize the Solution object to a binary file
void Solution::Serialize(const std::string& folderPath, const Solution& solution)
{
	std::string filePath = folderPath + "/" + solution.GetName() + ".sol";
	if (std::filesystem::exists(filePath))
		std::filesystem::remove(filePath);

	std::ofstream outFile(filePath, std::ios::binary);
	if (!outFile)
		throw std::runtime_error("Failed to open file for writing");

	// Write primitive types directly
	outFile.write(reinterpret_cast<const char*>(&solution.PlayerCount), sizeof(unsigned int));
	outFile.write(reinterpret_cast<const char*>(&solution.BbSize), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.SbSize), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.Margin), sizeof(float));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[0]), sizeof(uint8_t));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[1]), sizeof(uint8_t));
	outFile.write(reinterpret_cast<const char*>(&solution.Flop[2]), sizeof(uint8_t));
	outFile.write(reinterpret_cast<const char*>(&solution.Iters), sizeof(unsigned int));
	outFile.write(reinterpret_cast<const char*>(&solution.PerHandIters), sizeof(unsigned int));

	size_t totalRanges = solution.AllRanges.size();
	outFile.write(reinterpret_cast<const char*>(&totalRanges), sizeof(size_t));
	for (const auto& [key, range] : solution.AllRanges)
	{
		size_t keySize = key.size();
		outFile.write(reinterpret_cast<const char*>(&keySize), sizeof(size_t));
		outFile.write(reinterpret_cast<const char*>(key.data()), keySize);

		size_t currRangeSize = range.size();
		outFile.write(reinterpret_cast<const char*>(&currRangeSize), sizeof(size_t));

		for (auto& weHand : range)
		{
			outFile.write(reinterpret_cast<const char*>(&weHand.H[0]), sizeof(uint8_t));
			outFile.write(reinterpret_cast<const char*>(&weHand.H[1]), sizeof(uint8_t));
			outFile.write(reinterpret_cast<const char*>(&weHand.H[2]), sizeof(uint8_t));
			outFile.write(reinterpret_cast<const char*>(&weHand.H[3]), sizeof(uint8_t));
			outFile.write(reinterpret_cast<const char*>(&weHand.Ev), sizeof(float));
		}
	}
	outFile.close();
}

std::shared_ptr<Solution> Solution::Deserialize(const std::string& filePath)
{
	std::ifstream inFile(filePath, std::ios::binary);
	if (!inFile)
		throw std::runtime_error("Failed to open file for reading");

	std::shared_ptr<Solution> solution=std::make_shared<Solution>();

	// Write primitive types directly
	inFile.read(reinterpret_cast<char*>(&solution->PlayerCount), sizeof(unsigned int));
	inFile.read(reinterpret_cast<char*>(&solution->BbSize), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->SbSize), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->Margin), sizeof(float));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[0]), sizeof(uint8_t));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[1]), sizeof(uint8_t));
	inFile.read(reinterpret_cast<char*>(&solution->Flop[2]), sizeof(uint8_t));
	inFile.read(reinterpret_cast<char*>(&solution->Iters), sizeof(unsigned int));
	inFile.read(reinterpret_cast<char*>(&solution->PerHandIters), sizeof(unsigned int));

	size_t totalRanges;
	inFile.read(reinterpret_cast<char*>(&totalRanges), sizeof(size_t));

	for (int i = 0; i < totalRanges; i++)
	{
		size_t keySize;
		inFile.read(reinterpret_cast<char*>(&keySize), sizeof(size_t));

		std::string key(keySize, '\0'); // Allocate a string with the required size
		inFile.read(reinterpret_cast<char*>(&key[0]), keySize); // Read th

		size_t currRangeSize;
		inFile.read(reinterpret_cast<char*>(&currRangeSize), sizeof(size_t));

		std::vector<WeightedHand> hands;
		for (int j = 0; j < currRangeSize; j++)
		{
			Hand hand;
			inFile.read(reinterpret_cast<char*>(&hand[0]), sizeof(uint8_t));
			inFile.read(reinterpret_cast<char*>(&hand[1]), sizeof(uint8_t));
			inFile.read(reinterpret_cast<char*>(&hand[2]), sizeof(uint8_t));
			inFile.read(reinterpret_cast<char*>(&hand[3]), sizeof(uint8_t));
			float ev;
			inFile.read(reinterpret_cast<char*>(&ev), sizeof(float));

			hands.emplace_back(hand, ev);
		}
		solution->AllRanges[key] = std::move(hands);
	}

	inFile.close();
	return solution;
}