#ifndef BORDERLAYER_H
#define BORDERLAYER_H

#include "layer.h"

// vrstva obsahujici ohraniceni snimku sitnice 
class borderLayer : public layer {
private:
	int markQuadrant; // kvadrant, ve kterem je znacka 
	int markType; // typ znacky, 1 = pulkruh, 2 = obdelnik

	void makeTextureImage();

	// vrati hodnou alfa kanalu (pruhlednost) pro zadane souradnice
	int getAlphaChannelValue(double pointCoordX, double pointCoordY);

	// vrati vzdalenost od hrany kruhu, kladna = bod je uvnitr, zaporna = bod je vne
	double distanceFromCircle(double pointCoordX, double pointCoordY, double circleCenterCoordX, double circleCenterCoordY, double radius);

	// vrati vzdalenost od hrany ctverce, kladna = bod je uvnitr, zaporna = bod je vne
	double distanceFromSquareEdge(double pointCoordX, double pointCoordY, double squareCenterCoordX, double squareCenterCoordY, double sideLength);
public:
	borderLayer(int width, int height, double depth, int markQuadrant, int markType, double layerSize);
};

#endif