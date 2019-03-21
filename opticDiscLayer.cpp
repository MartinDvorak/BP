#include "opticDiscLayer.h"

opticDiscLayer::opticDiscLayer(int width, int height, double depth, double layerSize, double textureSize, int eye) {
	this->windowWidth = width;
	this->windowHeight = height;
	this->depth = depth;
	this->recalculateAspectRatio();
	this->layerSize = layerSize;
	this->textureSize = textureSize;
	this->createNewTexureImage(false);
	this->textureImageTmp = nullptr;
	this->textureChange = true;

	this->baseColorR = 250;
	this->baseColorG = 250;
	this->baseColorB = 150;

	this->opticDiscX = 0.4;
	this->opticDiscY = 0;
	this->opticDiscAR = 1.1;

	this->textureShiftX = 0.0;
	this->textureShiftY = 0.0;
	if (eye == 0) { // obe oci
		if (randRange(0.0, 1.0) > 0.5) this->textureRotation = 0.0;
		else this->textureRotation = 180.0;
	}
	else if(eye == 1) this->textureRotation = 180.0; // leve oko
	else this->textureRotation = 0.0; // prave oko

	this->initRand();
}

void opticDiscLayer::initRand() {
	this->baseColorR = this->baseColorR * randRange(0.98, 1.02);
	this->baseColorG = this->baseColorG * randRange(0.98, 1.02);
	this->baseColorB = this->baseColorB * randRange(0.98, 1.02);
	this->textureRotation = this->textureRotation + randRange(-10.0, 10.0);
	this->textureShiftX = this->textureShiftX + randRange(-0.01, 0.01);
	this->textureShiftY = this->textureShiftY + randRange(-0.01, 0.01);
	this->opticDiscAR = this->opticDiscAR + randRange(-0.08, 0.08);
}

void opticDiscLayer::makeTextureImage() {
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

void opticDiscLayer::getColorIntensity(double x, double y, double *r, double *g, double *b, double *a) {
	double ellipse = (pow(x - this->opticDiscX, 2) / pow(0.11 * 1.0, 2)) + (pow(-y + this->opticDiscY, 2) / pow(0.11 * this->opticDiscAR, 2));
	ellipse = 1 - ellipse;

	double dc = 0, da = 0, t = 0;
	if (ellipse >= 0) { // uvnitr elipsy
		dc = 0.5 + 0.5 * pow(ellipse, 2); // parametr vzdalenosti od stredu pro barvu
		da = (0.4 - x) * 2.5; // parametr vzdalenosti od x-ove souradnice stredu pro barvu
		t = pow(ellipse, 0.8); // parametr vzdalenosti od stredu pro pruhlednost
	}
	*r = this->baseColorR * ((dc + abs(da) > 1) ? 1 : dc + abs(da));
	*g = 0.3 * this->baseColorG + 0.7 * this->baseColorG * ((dc + da > 1) ? 1 : dc + da) * ((ellipse + (da * 2) > 1) ? 1 : ellipse + (da * 2));
	*b = this->baseColorB * (dc + abs(da)) * ((-0.5 + ellipse < 0) ? 0 : -0.5 + ellipse);
	*a = 255 * t * 0.75;
}