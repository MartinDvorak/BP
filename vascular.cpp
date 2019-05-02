	#include "pch.h"
#include "vascular.h"
#include <fstream> //todo
#include <cmath>
#include "main.h"
#include "realisticExtensionConstant.h"
#include "randomGenerator.h"

vascular::vascular(vascularPoint * root, double startCoordX, double startCoordY, nasalOrMacular specificMaximumX)
{
	this->root = root;
	this->startCoordX = startCoordX;
	this->startCoordY = startCoordY;
	this->specificMaximumX = specificMaximumX;
}

double calculateBifurcationRatio()
{
	generator* uniformDistribution = generator::getGenerator();
	return uniformDistribution->getNextValue(0.55,0.7);
}

double distanceBetweenPoint(vector firstPoint, vector secondPoint)
{
	double distanceX = std::pow(std::abs(firstPoint.vectorX - secondPoint.vectorX), 2);
	double distanceY = std::pow(std::abs(firstPoint.vectorY - secondPoint.vectorY), 2);

	return std::sqrt(distanceX + distanceY);
}

bool isPointInMacularArea(double coordX, double coordY)
{
	return distanceBetweenPoint(vector(coordX, coordY), vector(X_MACULAR_POINT,Y_MACULAR_POINT)) <
		RADIUS_MACULAR_POINT;
}

bool isBeyondTheEdge(double coordX, double coordY,nasalOrMacular nasalOrMacular)
{	
	if ((coordX > X_COORDINATE_MAX_MACULAR)&&(nasalOrMacular == macular))
		return true;
	if ((coordX > X_COORDINATE_MAX_NASAL) && (nasalOrMacular == nasal))
		return true;
	if (coordX < X_COORDINATE_MIN)
		return true;
	if (coordY > Y_COORDINATE_MAX)
		return true;
	if (coordY < Y_COORDINATE_MIN)
		return true;
	if (isPointInMacularArea(coordX, coordY))
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
	double normalizedValue = std::abs(vectorBeforeNormalise.vectorX);
	vectorBeforeNormalise.vectorX = vectorBeforeNormalise.vectorX / normalizedValue;
	vectorBeforeNormalise.vectorY = vectorBeforeNormalise.vectorY / normalizedValue;
	return vectorBeforeNormalise;
}

double convertBifurcationRatioToRadian(double bifurcationRatio, double openCoefficient)
{
	if (bifurcationRatio < 0.10)
		return RE_PI / 2;
	return (RE_PI / 2 - RE_PI / 2 * bifurcationRatio) / openCoefficient;
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
vector directionToMacularArea(double coordX, double coordY)
{
	if(coordX < 45.0)
	{
		vector tmp(X_MACULAR_POINT - coordX, Y_MACULAR_POINT - coordY);
		tmp.vectorY /= std::abs(tmp.vectorX);
		tmp.vectorX /= std::abs(tmp.vectorX);
		return tmp;
	}
	return {0, -1};
}

functionControler* startBranchControler(functionControler* vascularBranchControler, double actualCoordX, double actualCoordY,double bifurcationRatio, bifurcationDirection newDirection)
{
	vector parentDirection = vascularBranchControler->getUsedFunciton()->getDirectFunction(actualCoordX);
	vector oldParentDirection = vascularBranchControler->getUsedFunciton()->getDirectFunction(actualCoordX-10);
	int childBifuracationLevel = 1 + vascularBranchControler->getUsedBifurcationLogic()->getActualBifurcationLevel();
	int childBifuracationLevelMax = vascularBranchControler->getUsedBifurcationLogic()->getMaximalBifurcationLevel();

	bifurcationLogic* branchBifurcationLogic = new bifurcationLogic(0.05, 0.00, 0.1, 0.00, childBifuracationLevel, childBifuracationLevelMax);
	if (vascularBranchControler->getIsThisBranchT()) {
		branchBifurcationLogic = new bifurcationLogic(0.3, 0.00, 0.15, 0.00, childBifuracationLevel, childBifuracationLevelMax);
	}
	
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
			branchControler->setParentFunction(new mathFunction(directionToMacularArea(actualCoordX,actualCoordY)));
			branchControler->setIsThisTBranch(true);
			branchControler->setBifurcationLogic(new bifurcationLogic(0.3, 0.00, 0.15, 0.00, childBifuracationLevel, childBifuracationLevelMax));
		}
		else {
			branchControler->getUsedFunciton()->corectParam(vector(1.0, oldParentDirection.vectorY + (oldParentDirection.vectorY - parentDirection.vectorY)));
			branchControler->getUsedBifurcationLogic()->setMaxBifurcationLevel(2);
		}		
	}
	else {
		double openCoefficient = 1.5; // =1.5;
		if (vascularBranchControler->getIsThisBranchT()) {
			openCoefficient = 1.2;
		}
		vector directionBranch = calculateRotation(bifurcationRatio, parentDirection, newDirection,openCoefficient);
		branchControler->getUsedFunciton()->corectParam(directionBranch);
		
		vector parentDirectionBranch = calculateRotation(1-bifurcationRatio, parentDirection, getReverseDirection(newDirection),openCoefficient);
		
		vascularBranchControler->getUsedFunciton()->corectParam(parentDirectionBranch);
	}
	return branchControler;
}

vascularPoint* generateVascularBranch(double coordX, double coordY, double bifurcationRatio, functionControler* vascularBranchControler,nasalOrMacular specificMaxX)
{
	if (isBeyondTheEdge(coordX,coordY,specificMaxX))
		return nullptr;
	vascularPoint* actualRoot = nullptr;
	vascularPoint* actualParent = nullptr;
	vascularPoint* actualPoint;
	double actualCoordX, actualCoordY;
	do {
		vascularBranchControler->doStep();
		vascularBranchControler->getCoords(&actualCoordX, &actualCoordY);
		double newBifrurcationRatio;
		
		bifurcationDirection directionForNewBranchY;
		bifurcationDirection directionForNewBranchT;
		if (vascularBranchControler->getUsedFunciton()->getCurveType() == UperiorMacularVasculary)
		{
			directionForNewBranchY = vascularBranchControler->getUsedBifurcationLogic()->calculateBifurcationBranchY(actualCoordX);
			directionForNewBranchT = vascularBranchControler->getUsedBifurcationLogic()->calculateBifurcationBranchT(actualCoordX);
		}
		else {
			directionForNewBranchY = vascularBranchControler->getUsedBifurcationLogic()->calculateBifurcationBranchY();
		}
		
		if (directionForNewBranchY != noneDirection)
		{
			vascularBranchControler->getUsedBifurcationLogic()->deleteFirstBifurcationPointY();
			newBifrurcationRatio = calculateBifurcationRatio();
			actualPoint = new vascularPoint(actualCoordX, actualCoordY, newBifrurcationRatio);
			actualPoint->setLeftPoint(
				generateVascularBranch(actualCoordX,actualCoordY,1-newBifrurcationRatio,
					startBranchControler(vascularBranchControler,actualCoordX,actualCoordY,1-newBifrurcationRatio,directionForNewBranchY),specificMaxX));
		}
		else if ((directionForNewBranchT != noneDirection)&&(vascularBranchControler->getUsedFunciton()->getCurveType() == UperiorMacularVasculary))
		{
			vascularBranchControler->getUsedBifurcationLogic()->deleteFirstBifurcationPointT();
			newBifrurcationRatio = randRange(0.92, 0.96);
			actualPoint = new vascularPoint(actualCoordX, actualCoordY, newBifrurcationRatio);
			actualPoint->setLeftPoint(
				generateVascularBranch(actualCoordX, actualCoordY, 1.0 - newBifrurcationRatio,
					startBranchControler(vascularBranchControler, actualCoordX, actualCoordY, 1.0 - newBifrurcationRatio, directionForNewBranchT),specificMaxX));

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
	} while (!isBeyondTheEdge(actualCoordX, actualCoordY,specificMaxX));

	return actualRoot;
}

void vascular::makeTreeStructure(nasalOrMacular nasalMacular)
{
	double actualPointCordX = this->startCoordX;
	double actualPointCordY = this->startCoordY;
	
	bifurcationLogic* macularArteriolyBifurcationLogic = new bifurcationLogic(0,3, nasalMacular);
	typeOfVascular whichVascular = UperiorMacularVasculary;
	if(nasalMacular == nasal)
	{ 
		whichVascular = UperiorNasalVasculary;
	}
	functionControler* UperiolMacularArteriolyFunction = 
		new functionControler(this->startCoordX, this->startCoordY, 3.0,whichVascular,nullptr,macularArteriolyBifurcationLogic,false,nullptr);

	this->root->setRightPoint(generateVascularBranch(this->startCoordX, this->startCoordY, 1.0, UperiolMacularArteriolyFunction,this->specificMaximumX));

}

vascularPoint * vascular::getRoot()
{
	if (this->root != nullptr)
		return this->root;
	return nullptr;
}

void vascular::printTreeSructure()
{
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
