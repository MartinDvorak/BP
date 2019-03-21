#include "mathSet.h"

mathFunction::mathFunction(typeOfVascular vascularType)
{
	if (vascularType == UperiorMacularVasculary)
	{
		this->mathfunction = macularArteriolyII;
		this->actualVascularType = UperiorMacularVasculary;
		this->mathfunction = macularArteriolyI;
		//this->param = 3.586;
		this->param = 2.5;
		this->directionVector = vector(0.0, 0.0);
		this->curveType = functionCurve;
	}
	else if (vascularType == VascularyFromUperiorMacular)
	{
		this->mathfunction = nullptr;
		this->directionVector = vector(1.0, 1.0);
		this->actualVascularType = VascularyFromUperiorMacular;
		this->curveType = vectorCurve;
	}

}

mathFunction::mathFunction(vector directionVector)
{
	this->mathfunction = nullptr;
	this->directionVector = directionVector;
	this->actualVascularType = VascularyFromUperiorMacular;
	this->curveType = vectorCurve;
}


double mathFunction::getMathvalue(double x)
{
	return this->mathfunction(this->param,x);
}

void mathFunction::getMathvalue(double increment, double *vectorX, double *vectorY)
{
	*vectorX = this->directionVector.vectorX * increment;
	*vectorY = this->directionVector.vectorY * increment;
}

void mathFunction::corectParam(double newValueOfParam)
{
	if(this->actualVascularType == VascularyFromUperiorMacular)
		this->param = newValueOfParam;
}

void mathFunction::corectParam(vector corectVector)
{
	vector normalizedVector = vector(corectVector.vectorX / std::abs(corectVector.vectorX),
		corectVector.vectorY / std::abs(corectVector.vectorX));
	this->directionVector = normalizedVector;
}

vector mathFunction::getDirectFunction(double coordX)
{
	if (this->getCurveType() == functionCurve)
	{
		if (coordX < 0)
			coordX = 0;
		return vector(1.0, this->getMathvalue(coordX + 1) - this->getMathvalue(coordX) );
	}
	return this->directionVector;
}

typeOfVascular mathFunction::getTypeOfVascular()
{
	return this->actualVascularType;
}

typeOfCurve mathFunction::getCurveType()
{
	return this->curveType;
}


// first UperiorMacularyArterioly
//y\ = \ \frac{ \left(a_1 + b_1\left(x - e_1\right)\right) }{1 + c_1x + d_1x ^ 2}

double macularArteriolyI(double param, double x)
{
	double a, b, c, d, e;
	a = 4.42;
	b = param;
	c = 0.037768;
	d = 0.00039;
	e = 0.91;
	return (a + b * (x - e)) / (1.0 + c * x + d * x*x);
}

// secont UperiorMacularyArterioly
//y\ = \ \sqrt{ \frac{x}{a_3} }
double macularArteriolyII(double param, double x)
{
	return std::sqrt(x/param);
}


double simpleVector(double param, double x)
{
	return param * x;
}
