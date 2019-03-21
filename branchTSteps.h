#pragma once
class branchTSteps
{
	int counterOfStep;
	int maxNumberOfStep;

public:
	branchTSteps(int maxNumberOfStep);
	bool isEndOfVascular();
	int numberOfRemainSteps();
};

