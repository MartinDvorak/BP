
#ifndef CONTROLER_H
#define CONTROLER_H

#include "mathSet.h"
#include "randomGenerator.h"
#include <iostream>
#include "bifurcationLogic.h"
#include "branchTSteps.h"

enum typeOfFuntion {functionParentFunction, vectorParentFunction, vectorParentVector};

class functionControler {
	double startCoordX;
	double startCoordY;

	mathFunction* usedFunction;
	mathFunction* parentFunction;
	double relativeActualCoordX;
	double relativeActualCoordY;
	double delta;
	
	int corectionPoint;
	void incrementCorectionBreak();
	void calculateCorection();

	bool isThisTBranch;
	branchTSteps* statusBranchT;

	bifurcationLogic* usedBifurcationLogic;

public:
	functionControler(double startCoordX, double startCoordY, double delta, typeOfVascular actualType, mathFunction* parentFunction, bifurcationLogic* usedBifurcationLogic, bool isTBranch, branchTSteps* status );
	void doStep();
	void getCoords(double* coordX, double* coordY);
	mathFunction* getParentFunciton();
	mathFunction* getUsedFunciton();
	bifurcationLogic* getUsedBifurcationLogic();

	bool getIsThisBranchT();
	bool isStepsBranchTDoneAll();
	branchTSteps* getBranchTSteps();

	void setDelta(double actualDelta);
	void setParentFunction(mathFunction* parentFunction);
	void setIsThisTBranch(bool isTBranch);
	void setBifurcationLogic(bifurcationLogic* newBifurcationLogic);
};

#endif