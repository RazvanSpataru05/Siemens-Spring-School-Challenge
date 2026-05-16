#include <Services/RandomNumbersGenerator.h>

int RandomNumbersGenerator::GenerateIntegerNumberInRange(int lowerBound, int upperBound)
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_int_distribution<> distribution(lowerBound, upperBound);

	return distribution(generator);
}

double RandomNumbersGenerator::GenerateRealNumberInRange(int lowerBound, int upperBound)
{
	std::random_device randomDevice;
	std::mt19937 generator(randomDevice());
	std::uniform_real_distribution<> distribution(lowerBound, upperBound);

	return distribution(generator);
}

std::vector<double> RandomNumbersGenerator::GenerateRealNumbers(int lowerBound, int upperBound, size_t size)
{
	std::vector<double> randomNumbers;

	for (int index = 0; index < size; ++index)
	{
		randomNumbers.push_back(GenerateRealNumberInRange(lowerBound, upperBound));
	}

	return randomNumbers;
}
