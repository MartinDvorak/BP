#ifndef BIFURCATION_LOGIC_H
#define BIFURCATION_LOGIC_H

#include "main.h"
#include "randomGenerator.h"
#include "vector.h"
#include "realisticExtensionConstant.h"

enum bifurcationDirection { leftDirection, rightDirection, noneDirection };

class bifurcationLogic
{
	int bifurcationLevel;
	int bifurcationMaxLevel;

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

	std::vector<double> bifurcationAreaY;
	std::vector<double> bifurcationAreaT;

public:
	bifurcationDirection calculateBifurcationBranchY();
	bifurcationDirection calculateBifurcationBranchT();

	bifurcationDirection calculateBifurcationBranchY(double actualPointX);
	bifurcationDirection calculateBifurcationBranchT(double actualPointX);

	void deleteFirstBifurcationPointY();
	void deleteFirstBifurcationPointT();

	bifurcationLogic(int bifurcationLevel, int maxbifurcationLevel, nasalOrMacular nasalOrMacular);

	bifurcationLogic(double chanceY, double chanceT, double incrementY, double incrementT,int bifurcationLevel,int maxbifurcationLevel);

	void setMaxBifurcationLevel(int newMaxLevel);

	int getActualBifurcationLevel();
	int getMaximalBifurcationLevel();

	double getActualChanceY();
};

#endif