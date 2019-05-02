#include "mathSet.h"
#include "parameterCorectioner.h"

double corection(double param, double func(double,double))
{
	parameterCorectioner* corector = parameterCorectioner::getCorector();
	returnValues result = corector->setFunction(func);
	if (result == anotherFunction)
		return param;
	return corector->setAndCheckParam(param);
}

mathFunction::mathFunction(typeOfVascular vascularType)
{

	if (vascularType == UperiorMacularVasculary)
	{
		this->actualVascularType = UperiorMacularVasculary;
		this->directionVector = vector(0.0, 0.0);
		this->curveType = functionCurve;

		generator* uniformDistribution = generator::getGenerator();
		double randomValue = uniformDistribution->getNextUniformValue();
		if (randomValue < 0.7)
		{
			this->mathfunction = macularArteriolyI;
			this->param = randRange(2.1345,3.8235);
		}
		else if (randomValue < 1.1)
		{
			this->mathfunction = macularArteriolyII;
			this->param = randRange(0.013754,0.042782);

		}
		else {
			this->mathfunction = macularArteriolyIII;
			this->param = randRange(39.143,55.876);
		}
		this->param = corection(this->param, this->mathfunction);
	}
	else if (vascularType == UperiorNasalVasculary)
	{
		generator* uniformDistribution = generator::getGenerator();
		double randomValue = uniformDistribution->getNextUniformValue();

		this->actualVascularType = UperiorNasalVasculary;
		this->directionVector = vector(0.0, 0.0);
		this->curveType = functionCurve;

		if (randomValue < 0.5)
		{
			this->mathfunction = nasalArteriolyI;
			this->param = randRange(22.45632, 110.4456);
		}
		else if (randomValue < 1.1)
		{
			this->mathfunction = nasalArteriolyII;
			this->param = randRange(1.323423,8.743423);
		}
		else
		{
			this->mathfunction = nasalArteriolyIII;
			this->param = randRange(0.4124436,2.22323);
		}
		this->param = corection(this->param, this->mathfunction);
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
	a = 4.4181646; //4.42
	b = param; // 2.26 -- 3.55
	c = 0.03776856; //0.037768
	d = 0.0003901618; //0.00039
	e = 0.91;
	return (a + b * (x - e)) / (1.0 + c * x + d * x*x);
}

// secont UperiorMacularyArterioly
//y\ = \ \sqrt{ \frac{x}{a_3} }
double macularArteriolyII(double param, double x)
{
	double a = param;  // 0.0148 -- 0.0388
	return std::sqrt(x/a);
}

double macularArteriolyIII(double param, double x)
{
	double a = param;  
	double b = 0.5806228; //0.6
	double c = 0.1275869; //0.12758
 
	return a * std::pow(RE_E, std::pow(-1.0 * RE_E, b - c * x));
}

//f(x) = \frac{ ax }{x - b}
double nasalArteriolyI(double param, double x)
{
	double b = 26.83796; //22.0;
	return (param * x) / (x + b);
}

//f(x) = a(-x)^{b}
double nasalArteriolyII(double param, double x)
{
	double b = 0.5336715;
	return param* std::pow(x,b);
}

//f(x) =  c\left(\frac{bx}{a}\right)^{(\frac{1}{b})}
double nasalArteriolyIII(double param, double x)
{
	double a = 1.723413;
	double b = 0.1964321;
	return param * std::pow((b*x/a),(1/b));
}