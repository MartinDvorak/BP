#pragma once
enum returnValues { savedFunction, theSameFunction, anotherFunction };
#include "randomGenerator.h";

class parameterCorectioner
{
	int iterator;
	double(*functionQ1)(double, double);
	double(*functionQ2)(double, double);
	double(*functionQ3)(double, double);
	double(*functionQ4)(double, double);

	double paramQ1;
	double paramQ2;
	double paramQ3;
	double paramQ4;

	static parameterCorectioner* instance;
public:
	static parameterCorectioner* getCorector();
	parameterCorectioner();
	~parameterCorectioner();
	returnValues setFunction(double (*func)(double, double));
	double setAndCheckParam(double param);



};

