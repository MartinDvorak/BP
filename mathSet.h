#ifndef MATHFUNCTION_H
#define MATHFUNCTION_H

#include <iostream>
#include "vector.h"
#include "realisticExtensionConstant.h"
#include "randomGenerator.h"
#include "main.h"

double macularArteriolyI(double param, double x);
double macularArteriolyII(double param, double x);
double macularArteriolyIII(double param, double x);
double nasalArteriolyI(double param, double x);
double nasalArteriolyII(double param, double x);
double nasalArteriolyIII(double param, double x);


enum typeOfVascular { UperiorMacularVasculary, VascularyFromUperiorMacular, UperiorNasalVasculary};
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