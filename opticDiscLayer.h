#ifndef OPTICDISCLAYER_H
#define OPTICDISCLAYER_H

#include "layer.h"

// vrstva obsahujici terc zrakoveho nervu
class opticDiscLayer : public layer
{
private:
	// Slozky zakladni barvy
	int baseColorR;
	int baseColorG;
	int baseColorB;

	// pozice optickeho disku
	double opticDiscX; 
	double opticDiscY;
	// pomer stran optickeho disku - 1 : opticDiscAR (horizontalne : vertikalne)
	double opticDiscAR;

	void initRand(); // castecna randomizace nekterych vlastnosti
	void makeTextureImage();

	// vrati pro dany bod RGBA hodnoty
	void getColorIntensity(double x, double y, double *r, double *g, double *b, double *a);
public:
	opticDiscLayer(int width, int height, double depth, double layerSize, double textureSize, int eye);
};

#endif
