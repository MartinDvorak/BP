#include "parameterCorectioner.h"
#include <iostream>

parameterCorectioner* parameterCorectioner::instance = nullptr;

parameterCorectioner * parameterCorectioner::getCorector()
{
	return instance;
}

parameterCorectioner::parameterCorectioner()
{
	this->iterator = 0;
	this->functionQ1 = nullptr;
	this->functionQ2 = nullptr;
	this->functionQ3 = nullptr;
	this->functionQ4 = nullptr;
	instance = this;
}


parameterCorectioner::~parameterCorectioner()
{
	instance = nullptr;
}

returnValues compare(double (*paternFunc)(double, double), double (*func)(double, double))
{
	if (paternFunc == func)
		return theSameFunction;
	return anotherFunction;
}

double compareParam(double parentParam, double param)
{
	if ((0.8* parentParam < param) && (param < 1.2* parentParam))
	{
		if(param > parentParam)
			return 0.8* parentParam;
		return 1.2* parentParam;
	}
	return param;
}

returnValues parameterCorectioner::setFunction(double (*func)(double, double))
{
	this->iterator++;
	switch (this->iterator)
	{
	case 1: 
		this->functionQ1 = func;
		return savedFunction;
	case 2:
		this->functionQ2 = func;
		return savedFunction;
	case 3:
		this->functionQ3 = func;
		return savedFunction;
	case 4:
		this->functionQ4 = func;
		return savedFunction;
	case 5:
		return compare(this->functionQ1, func);
	case 6:
		return compare(this->functionQ2, func);
	case 7:
		return compare(this->functionQ3, func);
	case 8:
		return compare(this->functionQ4, func);
	}
}

double parameterCorectioner::setAndCheckParam(double param)
{
	switch (this->iterator)
	{
	case 1:
		this->paramQ1 = param;
		return param;
	case 2:
		this->paramQ2 = param;
		return param;
	case 3:
		this->paramQ3 = param;
		return param;
	case 4:
		this->paramQ4 = param;
		return param;
	case 5:
		return compareParam(this->paramQ1, param);
	case 6:
		return compareParam(this->paramQ2, param);
	case 7:
		return compareParam(this->paramQ3, param);
	case 8:
		return compareParam(this->paramQ4, param);
	}
}

