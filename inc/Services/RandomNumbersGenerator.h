#pragma once

#include <vector>
#include <random>

class RandomNumbersGenerator
{
public:
	static int GenerateIntegerNumberInRange(int lowerBound, int upperBound);
	static double GenerateRealNumberInRange(int lowerBound, int upperBound);

	static std::vector<double> GenerateRealNumbers(int lowerBound, int upperBound, size_t size);
};