#ifndef VESSEL_H
#define VESSEL_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <vector>
#include "main.h"
//#include "adapter.h"


#define RA_DIM 400

class bifurcationPoint
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
	bifurcationPoint *nextPointRight;
	bifurcationPoint *nextPointLeft;
	bifurcationPoint *prevPoint;

public:
	// generuje strom vetveni
	bifurcationPoint(int type, double width, double prevDistance, double widthRatio, bifurcationPoint *prevPoint, double bifurcationProb, bool strongest, int directionType, int direction);
	
	
	bifurcationPoint(double positionX, double positionY, int type, double width, double prevDistance, double widthRatio, bifurcationPoint* prevPoint, int directionType, int directionAngle);
	~bifurcationPoint();
	
	int getType();
	double getWidth();
	double getPrevDistance();
	double getPositionX();
	double getPositionY();
	int getDirectionAngle();
	int getDirectionType();
	double getWidthRatio();
	double getCurveWeight1();
	double getCurveWeight2();
	int getCurveEndDeviation();
	bifurcationPoint* getNextPointRight();
	bifurcationPoint* getNextPointLeft();
	bifurcationPoint* getPrevPoint();
	void setType(int type);
	void setWidth(double width);
	void setDirectionType(int directionType);
	void setPositionX(double positionX);
	void setPositionY(double positionY);
	void setDirectionAngle(int directionAngle);
	void setNextPointRight(bifurcationPoint *point);
	void setNextPointLeft(bifurcationPoint *point);
};

class vessel
{
private:
	int type; // 0 - tepna; 1 - zila
	int quadrant;
	double width;
	double startX;
	double startY;
	bifurcationPoint *startPoint;

	// vypocita pozce bodu vetveni
	void genBifurcationPointCoord(std::vector<bifurcationPoint*> *pointsQue, double *ra, int quadrant, int vesselType);
	// vrati body na usecce z pomocneho pole
	void getPointInLine(int x1, int y1, int x2, int y2, double *ra, std::vector<double*> *points, bool toFirstLine);
	// vykresli body do pomecneho pole
	void drawPoints(std::vector<double*> *points, double widthA, double widthB);
	// zkontroluje jestli neni na bodech neco vykresleno
	bool checkPoints(std::vector<double*> *points, double startWeight);
	// vaha na zaklade blizkosti cizi cevy ve smeru usecky
	double weightPoints(std::vector<double*> *points);
	// odstrani bod vetveni i s jeho podstromem
	void deletePoint(bifurcationPoint *point);
	// vrati rozsah uhlu ve kterem se bude hledat nova pozice
	void getAngleSpan(bifurcationPoint *point, int *startAngle, int *endAngle, std::vector<double> *weightAngle, bool weightCo);
	// vaha bodu okrajovych cev (directionType != 5)
	double getWeightDirectionType(double x, double y, bifurcationPoint *point, int quadrant, int vedirectionFceTypesselType);
	// funkce ktere maji nasledovat cevy (directionType 0, 1 a 2)
	double temporalDirectionFceNear(double x, int directionFceType);
	double temporalDirectionFceFar(double x, int directionFceType);
	double nasalDirectionFceNear(double x, int directionFceType);
	double nasalDirectionFceFar(double x, int directionFceType);
	// mapuje souradnice z vnitrniho souradnicoveho systemu do souradnic pomocneho pole
	int convertCoordX(double coord);
	int convertCoordY(double coord);
public:
	vessel(int type, int quadrant, double width, double startX, double startY, double xq, double yq, double directionFceType);
	vessel(int type, int quadrant, double width, double startX, double startY, double xq, double yq);
	~vessel();
	bifurcationPoint* getStartPoint();
	int getType();
	double getWidth();
	void setStartPoint(bifurcationPoint* rootPoint);
};

#endif