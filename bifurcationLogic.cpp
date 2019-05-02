#include "bifurcationLogic.h"


bool bifurcationLogic::isEnoughtBifurcation()
{
	if (this->bifurcationLevel < this->bifurcationMaxLevel)
		return false;
	return true;
}

bool bifurcationLogic::createYNewBranch()
{
	if (this->isEnoughtBifurcation())
		return false;
	if (randRange(0.0, 1.0) < this->chanceY)
	{
		this->chanceY = 0;
		return true;
	}
	this->chanceY += this->chanceIncrementY;
	return false;
}

bool bifurcationLogic::createTNewBranch()
{
	if (this->isEnoughtBifurcation())
		return false;

	if (randRange(0.0, 1.0) < this->chanceT)
	{
		this->chanceT = 0;
		return true;
	}
	this->chanceT += this->chanceIncrementT;
	return false;
}

bifurcationDirection directionNewBranch(double rightBranch, double leftBranch) 
{
	if (randRange(0.0, 1.0) < rightBranch)
		return rightDirection;
	return leftDirection;
}

bifurcationDirection bifurcationLogic::calculateBifurcationBranchY()
{
	if (this->createYNewBranch())
	{
		return directionNewBranch(this->chanceRightY,this->chanceLeftY);
	}
	return noneDirection;
}

bifurcationDirection bifurcationLogic::calculateBifurcationBranchT()
{
	if (this->createTNewBranch())
	{
		return directionNewBranch(this->chanceRightT, this->chanceLeftT);
	}
	return noneDirection;
}

bifurcationDirection bifurcationLogic::calculateBifurcationBranchY(double actualPointX)
{
	if (this->bifurcationAreaY.empty())
		return noneDirection;
	if (actualPointX > this->bifurcationAreaY.front())
	{
		return leftDirection;
	}
	return noneDirection;
}

bifurcationDirection bifurcationLogic::calculateBifurcationBranchT(double actualPointX)
{
	if (this->bifurcationAreaT.empty())
		return noneDirection;
	if (actualPointX > this->bifurcationAreaT.front())
	{
		return rightDirection;
	}
	return noneDirection;
}

void bifurcationLogic::deleteFirstBifurcationPointY()
{
	if (!this->bifurcationAreaY.empty())
	{
		this->bifurcationAreaY.erase(bifurcationAreaY.begin());
	}
}

void bifurcationLogic::deleteFirstBifurcationPointT()
{
	if (!this->bifurcationAreaT.empty())
	{
		this->bifurcationAreaT.erase(bifurcationAreaT.begin());
	}
}

int numberOfBifurcationY(double randomValue, double oneBifurcationExpectation, double twoBifurcationExpectation, double threeBifurcationExpectation)
{
	if (randomValue < oneBifurcationExpectation)
	{
		return 1;
	}
	else if (randomValue < twoBifurcationExpectation)
	{
		return 2;
	}
	else if (randomValue <= threeBifurcationExpectation)
	{
		return 3;
	}
}

int numberOfBifurcationT(double randomValue, double twoBifurcationExpectation, double threeBifurcationExpectation)
{
	if (randomValue < twoBifurcationExpectation)
	{
		return 2;
	}
	else if (randomValue <= threeBifurcationExpectation)
	{
		return 3;
	}
}

double corection(double value, double delta, int max, int min)
{
	if ((value - delta) < min)
		return min;
	if ((value - delta) > max)
		return max;
	return value - delta;
}

std::vector<double> computatePositionOfBifurationY(int countOfBifurcation)
{
	generator* uniformDistribution = generator::getGenerator();
	
	std::vector<double> tmp;
	if (countOfBifurcation == 1)
	{
		tmp.push_back((int)uniformDistribution->getNextValue(3, 25));
	}
	else if (countOfBifurcation == 2)
	{
		double value = uniformDistribution->getNextValue(2, 15);
		double delta = (15 - 2)/2 - value;
		tmp.push_back((int)value);

		value = corection(uniformDistribution->getNextValue(20, 40),delta,40,20);
		tmp.push_back((int)value);
	}
	else {
		double value = uniformDistribution->getNextValue(2, 15);
		double delta = (15 - 2)/2 - value;
		tmp.push_back((int)value);

		value = corection(uniformDistribution->getNextValue(15, 35), delta, 35, 15);
		delta = (35 - 15)/2 - value;
		tmp.push_back((int)value);
	
		value = corection(uniformDistribution->getNextValue(35, 60), delta, 60, 35);
		tmp.push_back((int)value);
	}
	return tmp;
}

std::vector<double> computatePositionOfBifurationT(int countOfBifurcation)
{
	generator* uniformDistribution = generator::getGenerator();

	std::vector<double> tmp;
	if (countOfBifurcation == 2)
	{
		double value = uniformDistribution->getNextValue(2, 15);
		double delta = (15 - 2)/2 - value;
		tmp.push_back((int)value);

		value = corection(uniformDistribution->getNextValue(20, 40), delta, 40, 20);
		tmp.push_back((int)value);
	}
	else {
		double value = uniformDistribution->getNextValue(2, 15);
		double delta = (15 - 2)/2 - value;
		tmp.push_back((int)value);

		value = corection(uniformDistribution->getNextValue(15, 35), delta, 35, 15);
		delta = (35 - 15)/2 - value;
		tmp.push_back((int)value);

		value = corection(uniformDistribution->getNextValue(35, 60), delta, 60, 35);
		tmp.push_back((int)value);
	}
	return tmp;
}

bifurcationLogic::bifurcationLogic(int actualBifurcationLevel, int maxBifurcationLevel, nasalOrMacular nasalOrMacular)
{
	this->bifurcationLevel = actualBifurcationLevel;
	this->bifurcationMaxLevel = maxBifurcationLevel;

	generator* uniformDistribution = generator::getGenerator();
	
	if(nasalOrMacular == macular)
	{ 
		double randomValue = uniformDistribution->getNextUniformValue();
		int countOfBifurcation = numberOfBifurcationY(randomValue, 0.1, 0.60, 1.0);
		this->bifurcationAreaY = computatePositionOfBifurationY(countOfBifurcation);

		randomValue = uniformDistribution->getNextUniformValue();
		countOfBifurcation = numberOfBifurcationT(randomValue, 0.33, 1.0);
		this->bifurcationAreaT = computatePositionOfBifurationT(countOfBifurcation);

	}
	else if(nasalOrMacular == nasal){
		std::vector<double> tmp;
		tmp.push_back(5.0);
		this->bifurcationAreaY = tmp;
	}
	
}

bifurcationLogic::bifurcationLogic(double chanceY, double chanceT, double incrementY, double incrementT,int bifurcationLevel, int bifurcationMaxLevel)
{
	this->bifurcationMaxLevel = bifurcationMaxLevel;
	this->bifurcationLevel = bifurcationLevel;

	this->chanceLeftT = 0.0;
	this->chanceRightT = 1.0;

	this->chanceLeftY = 0.5;
	this->chanceRightY = 0.5;

	this->chanceT = chanceT;
	this->chanceY = chanceY;

	this->chanceIncrementT = incrementT;
	this->chanceIncrementY = incrementY;
}

void bifurcationLogic::setMaxBifurcationLevel(int newMaxLevel)
{
	this->bifurcationMaxLevel = newMaxLevel;
}

int bifurcationLogic::getActualBifurcationLevel()
{
	return this->bifurcationLevel;
}

int bifurcationLogic::getMaximalBifurcationLevel()
{
	return this->bifurcationMaxLevel;
}

double bifurcationLogic::getActualChanceY()
{
	return this->chanceY;
}

