#ifndef BACKGROUNDLAYER_H
#define BACKGROUNDLAYER_H

#include "layer.h"
#include "perlin.h"

// vrstva obsahujici pozadi
class backgroundLayer : public layer {
private:
	// Slozky zakladni barvy
	int baseColorR;
	int baseColorG;
	int baseColorB;

	Perlin *perlin; // Perlinuv sum

	void initRand(); // castecna randomizace nekterych vlastnosti
	void makeTextureImage();

	// vrati pro dany bod RGBA hodnoty
	void getColorIntensity(double x, double y, double *r, double *g, double *b, double *a);
public:
	backgroundLayer(int width, int height, double depth, double layerSize, double textureSize, int seed);
	~backgroundLayer();
};

#endif