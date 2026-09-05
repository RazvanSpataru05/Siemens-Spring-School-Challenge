#include <Services/RandomNumbersGenerator.h>

namespace
{
	// Seeding a mt19937 costs far more than drawing from it, and this is called once per
	// gene per individual per epoch. One engine per thread keeps the draws independent
	// across the parallel fitness evaluations without paying that cost every time.
	std::mt19937& Engine()
	{
		static thread_local std::mt19937 generator{ std::random_device{}() };
		return generator;
	}
}

int RandomNumbersGenerator::GenerateIntegerNumberInRange(int lowerBound, int upperBound)
{
	std::uniform_int_distribution<int> distribution(lowerBound, upperBound);

	return distribution(Engine());
}

double RandomNumbersGenerator::GenerateRealNumberInRange(int lowerBound, int upperBound)
{
	std::uniform_real_distribution<double> distribution(lowerBound, upperBound);

	return distribution(Engine());
}

std::vector<double> RandomNumbersGenerator::GenerateRealNumbers(int lowerBound, int upperBound, size_t size)
{
	std::uniform_real_distribution<double> distribution(lowerBound, upperBound);
	std::mt19937& generator = Engine();

	std::vector<double> randomNumbers;
	randomNumbers.reserve(size);

	for (size_t index = 0; index < size; ++index)
	{
		randomNumbers.push_back(distribution(generator));
	}

	return randomNumbers;
}
