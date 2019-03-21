#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include <iostream>
#include "vector.h"

double macularArteriolyI(double param, double x);
double macularArteriolyII(double param, double x);
double simpleVector(double param, double x);

enum typeOfVascular { UperiorMacularVasculary, VascularyFromUperiorMacular};
enum typeOfCurve {functionCurve, vectorCurve};

class mathFunction {

	typeOfVascular actualVascularType;
	typeOfCurve curveType;
	
	double(*mathfunction)(double, double);
	double param;
	vector directionVector;

public:
	mathFunction(typeOfVascular quartal);
	mathFunction(vector directionVector);
	double getMathvalue(double x);
	void getMathvalue(double increment, double *vectorX, double *vectorY);

	void corectParam(double newValueOfParam);
	void corectParam(vector directionVector);
	
	vector getDirectFunction(double coordX);

	typeOfVascular getTypeOfVascular();
	typeOfCurve getCurveType();

};


#endif