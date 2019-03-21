#include "pch.h"
#include "vascular.h"
#include <fstream> //todo
#include <cmath>
#include "main.h"
#include "realisticExtensionConstant.h"
#include "randomGenerator.h"

vascular::vascular(vascularPoint * root, double startCoordX, double startCoordY)
{
	this->root = root;
	this->startCoordX = startCoordX;
	this->startCoordY = startCoordY;
}

double calculateBifurcationRatio()
{
	generator* uniformDistribution = generator::getGenerator();
	return uniformDistribution->getNextValue(0.55,0.7);
	//return randRangeND(0.5,0.99);
}

bool isBeyondTheEdge(double coordX, double coordY)
{
	if (coordX > X_COORDINATE_MAX -20)
		return true;
	if (coordX < X_COORDINATE_MIN + 10)
		return true;
	if (coordY > Y_COORDINATE_MAX - 15)
		return true;
	if (coordY < 0)
		return true;
	return false;
}

bifurcationDirection getReverseDirection(bifurcationDirection direction)
{
	if (direction == rightDirection)
		return leftDirection;
	if (direction == leftDirection)
		return rightDirection;
	return noneDirection;
}


// in radian
double rotatingByAngleCoordX(double angle, vector direction)
{
	return std::cos(angle) * direction.vectorX - std::sin(angle) * direction.vectorY;
}

double rotatingByAngleCoordY(double angle, vector direction)
{
	return std::cos(angle) * direction.vectorY + std::sin(angle) * direction.vectorX;
}

vector normaliseCoordXToOne(vector vectorBeforeNormalise)
{ 
	vectorBeforeNormalise.vectorX = vectorBeforeNormalise.vectorX / std::abs(vectorBeforeNormalise.vectorX);
	vectorBeforeNormalise.vectorY = vectorBeforeNormalise.vectorY / std::abs(vectorBeforeNormalise.vectorX);
	return vectorBeforeNormalise;
}

// pi/2 -> 0
// pi/4 -> 0.5
// 0	-> 1
double convertBifurcationRatioToRadian(double bifurcationRatio, double openCoefficient)
{
	if (bifurcationRatio < 0.10)
		return RE_PI / 2;
	return (RE_PI / 2 - RE_PI / 2 * bifurcationRatio)/ openCoefficient;
}

double covertBifurcationDirectiontoSign(bifurcationDirection newDirection)
{
	if (newDirection == leftDirection)
		return 1.0;
	if(newDirection == rightDirection)
		return -1.0;
	return 1.0;
}

vector calculateDirectionToMacula(vector actualPosition)
{
	vector positionMacula(actualPosition.vectorX, 0);
	
	actualPosition.vectorY = positionMacula.vectorY - actualPosition.vectorY;
	actualPosition.vectorY /= std::abs(actualPosition.vectorY);
	
	return actualPosition;
}

vector computateRotation(vector direction, double angleInRadian, double sign)
{
	vector rotated;
	rotated.vectorX = rotatingByAngleCoordX(sign * angleInRadian, direction);
	rotated.vectorY = rotatingByAngleCoordY(sign * angleInRadian, direction);
	return rotated;
}

vector calculateRotation(double bifurcationRatio, vector direction, bifurcationDirection newDirection,double openCoefficient)
{
	double angleInRadian = convertBifurcationRatioToRadian(bifurcationRatio,openCoefficient);
	double sign = covertBifurcationDirectiontoSign(newDirection);

	vector rotatedVector = computateRotation(direction,angleInRadian,sign);
	vector normalisedVector = normaliseCoordXToOne(rotatedVector);

	return normalisedVector;
}


functionControler* startBranchControler(functionControler* vascularBranchControler, double actualCoordX, double actualCoordY,double bifurcationRatio, bifurcationDirection newDirection)
{
	vector parentDirection = vascularBranchControler->getUsedFunciton()->getDirectFunction(actualCoordX);
	vector oldParentDirection = vascularBranchControler->getUsedFunciton()->getDirectFunction(actualCoordX-10);
	int childBifuracationLevel = 1 + vascularBranchControler->getUsedBifurcationLogic()->getActualBifurcationLevel();

	bifurcationLogic* branchBifurcationLogic = new bifurcationLogic(0.05, 0.05, 0.05, 0.02, childBifuracationLevel);
	
	double delta = 3.0;
	int remainSteps;
	if (vascularBranchControler->getBranchTSteps() == nullptr)
		remainSteps = 8;
	else {
		remainSteps = vascularBranchControler->getBranchTSteps()->numberOfRemainSteps();
	}
	functionControler* branchControler = 
		new functionControler(actualCoordX,actualCoordY,delta,
			typeOfVascular::VascularyFromUperiorMacular,vascularBranchControler->getUsedFunciton(),branchBifurcationLogic,vascularBranchControler->getIsThisBranchT(),new branchTSteps(remainSteps));
	
	if (vascularBranchControler->getUsedFunciton()->getTypeOfVascular() == typeOfVascular::UperiorMacularVasculary)
	{
		if (bifurcationRatio < 0.1)
		{
			vector actualDirection = computateRotation(parentDirection, RE_PI / 2, covertBifurcationDirectiontoSign(newDirection));
			branchControler->getUsedFunciton()->corectParam(normaliseCoordXToOne(actualDirection));
			branchControler->setParentFunction(new mathFunction(vector(0,-5)));
			branchControler->setIsThisTBranch(true);
		}
		else {
			branchControler->getUsedFunciton()->corectParam(vector(1.0, oldParentDirection.vectorY + (oldParentDirection.vectorY - parentDirection.vectorY)));
		}		
	}
	else {
		double openCoefficient = 1.5;
		if (vascularBranchControler->getIsThisBranchT()) {
			openCoefficient = 1.1;
		}
		vector directionBranch = calculateRotation(bifurcationRatio, parentDirection, newDirection,openCoefficient);
		branchControler->getUsedFunciton()->corectParam(directionBranch);
		
		vector parentDirectionBranch = calculateRotation(1-bifurcationRatio, parentDirection, getReverseDirection(newDirection),openCoefficient);
		vascularBranchControler->getUsedFunciton()->corectParam(parentDirectionBranch);
	}
	return branchControler;
}

vascularPoint* generateVascularBranch(double coordX, double coordY, double bifurcationRatio, functionControler* vascularBranchControler)
{
	if (isBeyondTheEdge(coordX,coordY))
		return nullptr;
	vascularPoint* actualRoot = nullptr;
	vascularPoint* actualParent = nullptr;
	vascularPoint* actualPoint;
	double actualCoordX, actualCoordY;
	do {
		vascularBranchControler->doStep();
		vascularBranchControler->getCoords(&actualCoordX, &actualCoordY);
		double newBifrurcationRatio;
		
		bifurcationDirection directionForNewBranchY = vascularBranchControler->getUsedBifurcationLogic()->calculateBifurcationBranchY();
		bifurcationDirection directionForNewBranchT = vascularBranchControler->getUsedBifurcationLogic()->calculateBifurcationBranchT();
		
		if (directionForNewBranchY != noneDirection)
		{
			newBifrurcationRatio = calculateBifurcationRatio();
			actualPoint = new vascularPoint(actualCoordX, actualCoordY, newBifrurcationRatio);
			actualPoint->setLeftPoint(
				generateVascularBranch(actualCoordX,actualCoordY,1-newBifrurcationRatio,
					startBranchControler(vascularBranchControler,actualCoordX,actualCoordY,1-newBifrurcationRatio,directionForNewBranchY)));
		}
		else if ((directionForNewBranchT != noneDirection)&&(vascularBranchControler->getUsedFunciton()->getCurveType() == UperiorMacularVasculary))
		{
			newBifrurcationRatio = 0.95;
			actualPoint = new vascularPoint(actualCoordX, actualCoordY, newBifrurcationRatio);
			actualPoint->setLeftPoint(
				generateVascularBranch(actualCoordX, actualCoordY, 1.0 - newBifrurcationRatio,
					startBranchControler(vascularBranchControler, actualCoordX, actualCoordY, 1.0 - newBifrurcationRatio, directionForNewBranchT)));

		}
		else { // continue without bifurcation point
			actualPoint = new vascularPoint(actualCoordX, actualCoordY, bifurcationRatio);
			bifurcationRatio = 1.0;
		}
		
		if (actualParent != nullptr)
		{
			if(actualParent->getRightPoint() == nullptr)
				actualParent->setRightPoint(actualPoint);
			else {
				actualParent->setLeftPoint(actualPoint);
			}
		}
		actualParent = actualPoint;
		if (actualRoot == nullptr)
			actualRoot = actualPoint;

		if (vascularBranchControler->isStepsBranchTDoneAll())
			break;
	} while (!isBeyondTheEdge(actualCoordX, actualCoordY));

	return actualRoot;
}

void vascular::makeTreeStructure()
{
	double actualPointCordX = this->startCoordX;
	double actualPointCordY = this->startCoordY;
	
	bifurcationLogic* macularArteriolyBifurcationLogic = new bifurcationLogic(0.15, 0.0, 0.1, 0.2, 0);
	functionControler* UperiolMacularArteriolyFunction = 
		new functionControler(this->startCoordX, this->startCoordY, 6.0,typeOfVascular::UperiorMacularVasculary,nullptr,macularArteriolyBifurcationLogic,false,nullptr);

	this->root->setRightPoint(generateVascularBranch(this->startCoordX, this->startCoordY, 1.0, UperiolMacularArteriolyFunction));

}

vascularPoint * vascular::getRoot()
{
	if (this->root != nullptr)
		return this->root;
	return nullptr;
}

void vascular::printTreeSructure()
{
	// todo
	std::ofstream myfile;
	myfile.open("example.txt");

	vascularPoint* actualPointer = this->root;
	std::vector <vascularPoint*> queue;
	queue.push_back(root);
	while (!queue.empty())
	{
		actualPointer = queue.front();
		queue.erase(queue.begin());
		myfile << actualPointer->getCordX() << "\t" << actualPointer->getCordY() << "\n";
		if (actualPointer->getLeftPoint() != nullptr)
			queue.push_back(actualPointer->getLeftPoint());
		if (actualPointer->getRightPoint() != nullptr)
			queue.push_back(actualPointer->getRightPoint());
	
	}
	myfile.close();
}
