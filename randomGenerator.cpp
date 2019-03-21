#include "randomGenerator.h"
#include <ctime>
#include <iostream>

generator::generator()
{
	std::srand((unsigned)std::time(nullptr));
}

double nextValue()
{
	return std::rand() / (double)((RAND_MAX + 1u));
}

generator* generator::instance = nullptr;


generator * generator::getGenerator()
{
	if (instance == nullptr)
	{
		instance = new generator();
	}
	return instance;
}

double generator::getNextUniformValue()
{
	return nextValue();
}

double generator::getNextValue(double minValue, double maxValue)
{
	double randomNumber = nextValue();
	return randomNumber * (maxValue - minValue) + minValue;
}
