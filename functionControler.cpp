#include "pch.h"
#include "functionControler.h"

void functionControler::incrementCorectionBreak()
{
	this->corectionPoint++;
	if (this->corectionPoint == 2)
	{
		this->corectionPoint = 0;
		this->calculateCorection();
	}
	
}

void functionControler::calculateCorection()
{
	double corection = 0.4;
	if (this->parentFunction != nullptr)
	{
		vector directParent = this->parentFunction->getDirectFunction(this->relativeActualCordX);
		vector directChild = this->usedFunction->getDirectFunction(this->relativeActualCordX);

		if (this->usedFunction->getCurveType() != functionCurve)
		{
			double vectorX = (directChild.vectorX + directParent.vectorX * corection) / std::abs(directChild.vectorX + directParent.vectorX * corection);
			double vectorY = (directChild.vectorY + directParent.vectorY * corection) / std::abs(directChild.vectorX + directParent.vectorX * corection);
			this->usedFunction->corectParam(
				vector(vectorX, vectorY));
			
			std::cout << "old vectorX: " << directChild.vectorX << "old vectorY: "  << directChild.vectorY  << std::endl;
			std::cout << "new vectorX: " << vectorX << "new vectorY: " << vectorY << std::endl;
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
	this->relativeActualCordX = 0.0;
	this->relativeActualCordY = 0.0;
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
{  // TODO random strelene = dodìlat relativni zachveni s minulým krokem
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
			tmpCordX += this->relativeActualCordX;
			tmpCordY += this->relativeActualCordY;
		}
		else { // this->usedFunction->getCurveType() == functionCurve
			tmpCordX = this->relativeActualCordX + increment;
			tmpCordY = this->usedFunction->getMathvalue(tmpCordX);
		}

		actualDistance = distance(this->relativeActualCordX, this->relativeActualCordY, tmpCordX,tmpCordY);
		status = checkRange(delta, actualDistance);
		setIncrement(&increment, status);
	} while(status != inRange);
	
	generator* UniformDistribution = generator::getGenerator();
	this->relativeActualCordX = randomCorection(tmpCordX,UniformDistribution->getNextUniformValue());
	this->relativeActualCordY = randomCorection(tmpCordY,UniformDistribution->getNextUniformValue());
	this->incrementCorectionBreak();

}

void functionControler::getCoords(double * coordX, double * coordY)
{
	*coordX = this->relativeActualCordX + this->startCoordX;
	*coordY = this->relativeActualCordY + this->startCoordY;
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
