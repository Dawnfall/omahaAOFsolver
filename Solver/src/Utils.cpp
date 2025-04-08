#include "Utils.h"

namespace Utils
{
	std::unique_ptr<std::minstd_rand> m_randGen = nullptr;
	unsigned int NumberOfThreads = std::thread::hardware_concurrency();
	//unsigned int NumberOfThreads = 1;

	std::minstd_rand& GetRandomGen()
	{
		if (!m_randGen)
		{
			m_randGen = std::make_unique<std::minstd_rand>(std::random_device{}());
		}
		return *m_randGen;
	}
}


