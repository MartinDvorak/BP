#include "bifurcationLogic.h"


bool bifurcationLogic::isEnoughtBifurcation()
{
	if (this->bifurcationLevel < 3)
		return false;
	return true;
}

bool bifurcationLogic::createYNewBranch()
{
	if (isEnoughtBifurcation())
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
	if (isEnoughtBifurcation())
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

bifurcationLogic::bifurcationLogic(double chanceY, double chanceT)
{
	this->chanceLeftT = 0.0;
	this->chanceRightT = 1.0;
	
	this->chanceLeftY = 0.65;
	this->chanceRightY = 0.35;

	this->chanceT = chanceT;
	this->chanceY = chanceY;

	this->chanceIncrementT = 0.1;
	this->chanceIncrementY = 0.1;

}

bifurcationLogic::bifurcationLogic(double chanceY, double chanceT, double incrementY, double incrementT,int level)
{
	this->bifurcationLevel = level;

	this->chanceLeftT = 0.0;
	this->chanceRightT = 1.0;

	this->chanceLeftY = 0.5;
	this->chanceRightY = 0.5;

	this->chanceT = chanceT;
	this->chanceY = chanceY;

	this->chanceIncrementT = incrementT;
	this->chanceIncrementY = incrementY;
}

void bifurcationLogic::setChanceY(double newChanceY,double incrementY)
{
	this->chanceY = newChanceY;
	this->chanceIncrementY = incrementY;
}

int bifurcationLogic::getActualBifurcationLevel()
{
	return this->bifurcationLevel;
}

