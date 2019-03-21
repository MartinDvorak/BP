#ifndef BIFURCATION_LOGIC_H
#define BIFURCATION_LOGIC_H

#include "main.h"
#include "randomGenerator.h"

enum bifurcationDirection { leftDirection, rightDirection, noneDirection };

class bifurcationLogic
{
	int bifurcationLevel;

	double chanceLeftY;
	double chanceRightY;

	double chanceLeftT;
	double chanceRightT;

	double chanceY;
	double chanceT;

	double chanceIncrementY;
	double chanceIncrementT;

	bool isEnoughtBifurcation();
	bool createYNewBranch();
	bool createTNewBranch();

public:
	bifurcationDirection calculateBifurcationBranchY();
	bifurcationDirection calculateBifurcationBranchT();

	bifurcationLogic(double chanceY, double chanceT);
	bifurcationLogic(double chanceY, double chanceT, double incrementY, double incrementT,int level);

	void setChanceY(double newChanceY,double incrementY);

	int getActualBifurcationLevel();

};

#endif