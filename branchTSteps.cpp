#include "branchTSteps.h"


branchTSteps::branchTSteps(int maxNumberOfStep)
{
	this->counterOfStep = 0;
	this->maxNumberOfStep = maxNumberOfStep;
}

bool branchTSteps::isEndOfVascular()
{
	counterOfStep++;
	return counterOfStep >= maxNumberOfStep;
}

int branchTSteps::numberOfRemainSteps()
{
	return this->maxNumberOfStep - this->counterOfStep;
}
