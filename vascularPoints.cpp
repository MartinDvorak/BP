#include "pch.h"
#include "vascularPoints.h"

vascularPoint::vascularPoint(double cordX, double cordY,double bifurcationRatio)
{
	this->cordX = cordX;
	this->cordY = cordY;
	this->branchT = false;
	this->bifurcationRatio = bifurcationRatio;
	this->leftPointer = nullptr;
	this->rightPointer = nullptr;
}


vascularPoint * vascularPoint::getLeftPoint()
{
	if (this->leftPointer != nullptr)
		return this->leftPointer;
	return nullptr;
}

vascularPoint * vascularPoint::getRightPoint()
{
	if (this->rightPointer != nullptr)
		return this->rightPointer;
	return nullptr;
}

double vascularPoint::getCordX()
{
	return this->cordX;
}

double vascularPoint::getCordY()
{
	return this->cordY;
}

double vascularPoint::getBifurcationRatio()
{
	return this->bifurcationRatio;
}

bool vascularPoint::getBranchT()
{
	return this->branchT;
}

void vascularPoint::setLeftPoint(vascularPoint * leftPointer)
{
	this->leftPointer = leftPointer;
}

void vascularPoint::setRightPoint(vascularPoint * rightPointer)
{
	this->rightPointer = rightPointer;
}

void vascularPoint::setBranchT(bool isBranchT)
{
	this->branchT = isBranchT;
}

