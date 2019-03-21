#include "pch.h"
#include "bifurcationPointLight.h"

bifurcationPointLight::bifurcationPointLight()
{
	//TODO predelat na random hodnoty
	this->curveWeight1 = 0.9; // 0.8 - 1.2
	this->curveWeight2 = 0.9; // 0.8 - 1.2
	this->curveEndDeviation = 70; // 50% <-90;-60> 50% <60;90>
}

void bifurcationPointLight::setType(int type)
{
	this->type = type;
}

void bifurcationPointLight::setWidth(double parentWidth)
{
	this->width = (parentWidth * std::sqrt(this->getWidthRatio()) ) - (parentWidth * (this->getPrevDistance()/10)) - (this->getPrevDistance() / 20);
}

void bifurcationPointLight::setPrevDistance(double prevPointX, double prevPointY, double actualPointX, double actualPointY)
{
	double deltaX = std::abs(prevPointX - actualPointX);
	double deltaY = std::abs(prevPointY - actualPointY);
	
	this->prevDistance = std::sqrt(deltaX * deltaX + deltaY * deltaY);
}

double conversionRadianToDegree(double radian)
{
	return radian * 180 / RE_PI;
}

double conversionDegreeToRadius(double degree)
{
	return degree * RE_PI / 180;
}

void bifurcationPointLight::setDirectionAngle(double prevPointX, double prevPointY, double actualPointX, double actualPointY)
{ 
	this->directionAngle = (((int)(atan2(actualPointY - prevPointY, actualPointX - prevPointX) * 180 / RE_PI) + 360) % 360);
}

void bifurcationPointLight::setDirectionType(int type)
{
	this->directionType = type;
}

void bifurcationPointLight::setWidthRation(double widthRatio)
{
	this->widthRatio = widthRatio;
}

void bifurcationPointLight::setAndConvertPositionX(double coordX)
{
	double coordXMax = X_COORDINATE_MAX;
	double coordXMin = X_COORDINATE_MIN;
	this->positionX = (-2.0* ((coordX - coordXMin) / (coordXMax - coordXMin)) + 1.0) + X_COORDINATE_SHIFT;
}

void bifurcationPointLight::setAndConvertPositionY(double coordY)
{
	double coordYMax = Y_COORDINATE_MAX;
	this->positionY = (-1.0* coordY / coordYMax);
}

int bifurcationPointLight::getType()
{
	return this->type;
}

double bifurcationPointLight::getWidth()
{
	return this->width;
}

double bifurcationPointLight::getPrevDistance()
{
	return this->prevDistance;
}

double bifurcationPointLight::getPositionX()
{
	return this->positionX;
}

double bifurcationPointLight::getPositionY()
{
	return this->positionY;
}

int bifurcationPointLight::getDirectionAngle()
{
	return this->directionAngle;
}

int bifurcationPointLight::getDirectionType()
{
	return this->directionType;
}

double bifurcationPointLight::getWidthRatio()
{
	return this->widthRatio;
}
