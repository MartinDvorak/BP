#include "pch.h"
#include "functionControler.h"

void functionControler::incrementCorectionBreak()
{
	this->corectionPoint++;
	if (this->corectionPoint == 1)
	{
		this->corectionPoint = 0;
		this->calculateCorection();
	}
	
}

void functionControler::calculateCorection()
{
	double corection = 0.2;
	if (this->parentFunction != nullptr)
	{
		vector directParent = this->parentFunction->getDirectFunction(this->relativeActualCoordX);
		vector directChild = this->usedFunction->getDirectFunction(this->relativeActualCoordX);

		if (this->usedFunction->getCurveType() != functionCurve)
		{
			double vectorX = (directChild.vectorX + directParent.vectorX * corection) / std::abs(directChild.vectorX + directParent.vectorX * corection);
			double vectorY = (directChild.vectorY + directParent.vectorY * corection) / std::abs(directChild.vectorX + directParent.vectorX * corection);
			this->usedFunction->corectParam(
				vector(vectorX, vectorY));
			
		}
	}
}

functionControler::functionControler(double startCoordX, double startCoordY, double delta, typeOfVascular actualType,mathFunction* parentFunction, bifurcationLogic* usedBifurcationLogic, bool isTBranch, branchTSteps* status)
{
	this->startCoordX = startCoordX;
	this->startCoordY = startCoordY;
	this->usedFunction = new mathFunction(actualType);
	this->parentFunction = parentFunction;
	this->delta = delta;	
	this->relativeActualCoordX = 0.0;
	this->relativeActualCoordY = 0.0;
	this->corectionPoint = 0;
	this->usedBifurcationLogic = usedBifurcationLogic;
	this->isThisTBranch = isTBranch;
	if (status != nullptr)
		this->statusBranchT = status;
	else {
		this->statusBranchT = nullptr;
	}
}

double distance(double cordX1, double cordY1, double cordX2, double cordY2)
{
	double deltaX = std::abs(cordX1 - cordX2);
	double deltaY = std::abs(cordY1 - cordY2);

	return std::sqrt(deltaX*deltaX + deltaY*deltaY);
}

enum isInRange {underRange, inRange, aboveRange};

isInRange checkRange(double delta, double distance) {
	// 90% -- 110% delta
	if ((0.9 * delta < distance) && (distance < 1.1 * delta))
		return inRange;
	else if (0.9 * delta < distance)
		return aboveRange;
	return underRange;
}

void setIncrement(double* increment,isInRange status)
{
	if (status == aboveRange) {
		*increment -= *increment / 2;
	}
	else if (status == underRange) {
		*increment += *increment / 2;
	}
}

double randomCorection(double value, double corection)
{  
	double fivePercent = 0.05 * value;
	return value + fivePercent * corection;
}

void functionControler::doStep()
{
	double actualDistance;
	double tmpCordX, tmpCordY;
	double increment = this->delta;
	isInRange status;
	do{
		if (this->usedFunction->getCurveType() == vectorCurve)
		{
			this->usedFunction->getMathvalue(increment,&tmpCordX,&tmpCordY);
			tmpCordX += this->relativeActualCoordX;
			tmpCordY += this->relativeActualCoordY;
		}
		else { // this->usedFunction->getCurveType() == functionCurve
			tmpCordX = this->relativeActualCoordX + increment;
			tmpCordY = this->usedFunction->getMathvalue(tmpCordX);
		}

		actualDistance = distance(this->relativeActualCoordX, this->relativeActualCoordY, tmpCordX,tmpCordY);
		status = checkRange(this->delta, actualDistance);
		setIncrement(&increment, status);
	} while(status != inRange);
	
	generator* UniformDistribution = generator::getGenerator();
	this->relativeActualCoordX = randomCorection(tmpCordX,UniformDistribution->getNextUniformValue());
	this->relativeActualCoordY = randomCorection(tmpCordY,UniformDistribution->getNextUniformValue());
	this->incrementCorectionBreak();

}

void functionControler::getCoords(double * coordX, double * coordY)
{
	*coordX = this->relativeActualCoordX + this->startCoordX;
	*coordY = this->relativeActualCoordY + this->startCoordY;
}

mathFunction * functionControler::getParentFunciton()
{
	return this->parentFunction;
}

mathFunction * functionControler::getUsedFunciton()
{
	return this->usedFunction;
}

bifurcationLogic * functionControler::getUsedBifurcationLogic()
{
	return this->usedBifurcationLogic;
}

bool functionControler::getIsThisBranchT()
{
	return this->isThisTBranch;
}

bool functionControler::isStepsBranchTDoneAll()
{
	if (this->isThisTBranch)
		return statusBranchT->isEndOfVascular();
	return false;
}

branchTSteps * functionControler::getBranchTSteps()
{
	return this->statusBranchT;
}


void functionControler::setDelta(double actualDelta)
{
	this->delta = actualDelta;
}

void functionControler::setParentFunction(mathFunction * parentFunction)
{
	this->parentFunction = parentFunction;
}

void functionControler::setIsThisTBranch(bool isTBranch)
{
	this->isThisTBranch = isTBranch;
}

void functionControler::setBifurcationLogic(bifurcationLogic * newBifurcationLogic)
{
	this->usedBifurcationLogic = newBifurcationLogic;
}
