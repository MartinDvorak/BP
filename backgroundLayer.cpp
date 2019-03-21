#include "backgroundLayer.h"


backgroundLayer::backgroundLayer(int width, int height, double depth, double layerSize, double textureSize, int seed){
	this->windowWidth = width;
	this->windowHeight = height;
	this->depth = depth;
	this->recalculateAspectRatio();
	this->layerSize = layerSize;
	this->textureSize = textureSize;
	this->createNewTexureImage(false);
	this->textureImageTmp = nullptr;
	this->textureChange = true;

	this->baseColorR = 200;
	this->baseColorG = 60;
	this->baseColorB = 40;

	this->textureShiftX = 0.0;
	this->textureShiftY = 0.0;
	this->textureRotation = 0.0;
	this->perlin = new Perlin(3, 5, 1, seed);
	this->initRand();
}

backgroundLayer::~backgroundLayer() {
	delete(this->perlin);
}

void backgroundLayer::initRand() {
	this->baseColorR = this->baseColorR * randRange(0.95, 1.05);
	this->baseColorG = this->baseColorG * randRange(0.95, 1.05);
	this->baseColorB = this->baseColorB * randRange(0.95, 1.05);
}

void backgroundLayer::makeTextureImage() {
	// ziskani transformacnich udaju z nizsich vrstev
	std::vector<double> shiftX, shiftY, rotation;
	this->getTextureShiftX(&shiftX);
	this->getTextureShiftY(&shiftY);
	this->getTextureRotation(&rotation);
	int transformSize = shiftX.size();
	double *transformation = new double[3 * shiftX.size()];
	for (int i = 0; i < transformSize; i++) {
		transformation[0 * transformSize + i] = shiftX.at(i);
		transformation[1 * transformSize + i] = shiftY.at(i);
		transformation[2 * transformSize + i] = rotation.at(i);
	}

	double xn, yn, r, g, b, a;
	for (int y = 0; y < this->textureHeight; y++) {
		for (int x = 0; x < this->textureWidth; x++) {
			// transformace souradnic (rotace a posuv)
			xn = this->convertCoordX(x);
			yn = this->convertCoordY(y);
			for (int i = 0; i < transformSize; i++) {
				this->rotationCoord(xn, yn, &xn, &yn, transformation[2 * transformSize + i]);
				xn = xn + transformation[0 * transformSize + i];
				yn = yn + transformation[1 * transformSize + i];
			}

			getColorIntensity(xn, yn, &r, &g, &b, &a);
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)r;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)g;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)b;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)a;
		}
	}
	this->textureChange = false;
	delete(transformation);
}

void backgroundLayer::getColorIntensity(double x, double y, double *r, double *g, double *b, double *a) {
	// tmavsi fovea, svetlejsi zbytek makularni oblasti
	double c = (sin(4.5 * sqrt((x * x) + (y * y)) + 0.01) + 1) / 2;
	
	// od urcite vzdalenosti jina funkce, kdy textura se vzdalenosti postupne tmavne
	double dist = sqrt((x * x) + (y * y)) - (1 / (1.7));
	double xn, yn;
	if (dist > 0) {
		xn = 1 / 1.7;
		yn = 0;
		c = (sin(4.5 * xn + 0.01) + 1) / 2 - (dist / 3);
	}
	// pridani Perlinova sumu
	c = c * 0.9 + 0.1 * (this->perlin->Get(x, y));
	*r = this->baseColorR * c;
	*g = this->baseColorG * c;
	*b = this->baseColorB * c;
	*a = 255;
}