
#ifndef BIFURCATIONPOINTLIGHT_H
#define BIFURCATIONPOINTLIGHT_H

#include <iostream>
#include <cmath>
#include "vessel.h"
#include "realisticExtensionConstant.h"

class bifurcationPointLight
{
private:
	int type; // 0 - pruchozi bod, 1 - koncovy bod, 2 - Y-deleni, 3 - T-deleni 
	double width; // pomer od zacatku
	double prevDistance; // prima vzdalenost od predchoziho bodu
	double positionX;
	double positionY;
	int directionAngle; // 0-360° smer od predchoziho k aktualnimu bodu
	int directionType; // 0 pocatek 1 nebo 2, 1 nejpravejsi silna ceva, 2 nejlevejsi silna ceva, 3 prava slaba, 4 leva slaba, 5 ostatni
	double widthRatio;
	double curveWeight1;
	double curveWeight2;
	int curveEndDeviation;
public:
	bifurcationPointLight();
	void setType(int type);
	void setWidth(double parentWidth);
	void setPrevDistance(double prevPointX, double prevPointY, double actualPointX, double actualPointY);
	void setDirectionAngle(double prevPointX, double prevPointY, double actualPointX, double actualPointY);
	void setDirectionType(int type);
	void setWidthRation(double widthRatio);
	void setAndConvertPositionX(double coordX);
	void setAndConvertPositionY(double coordY);

	int getType();
	double getWidth();
	double getPrevDistance();
	double getPositionX();
	double getPositionY();
	int getDirectionAngle();
	int getDirectionType();
	double getWidthRatio();
};

#endif