#include "borderLayer.h"

borderLayer::borderLayer(int width, int height, double depth, int markQuadrant, int markType, double layerSize) {
	this->windowWidth = width;
	this->windowHeight = height;
	this->depth = depth;
	this->recalculateAspectRatio();
	this->markQuadrant = markQuadrant;
	this->markType = markType;
	this->layerSize = layerSize;
	this->textureSize = 2.0; // nemìnit
	this->createNewTexureImage(false);
	this->textureChange = true;

	this->textureShiftX = 0.0;
	this->textureShiftY = 0.0;
	this->textureRotation = 0.0;
}

void borderLayer::makeTextureImage() {
	double xn, yn;
	for (int y = 0; y < this->textureHeight; y++) {
		for (int x = 0; x < this->textureWidth; x++) {
			xn = this->convertCoordX(x);
			yn = this->convertCoordY(y);

			this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)this->getAlphaChannelValue(xn, yn);
		}
	}
	this->textureChange = false;
}

int borderLayer::getAlphaChannelValue(double pointCoordX, double pointCoordY) {
	// hlavni hruh
	double distanceA = this->distanceFromCircle(pointCoordX, pointCoordY, 0.0, 0.0, 1.0 / this->textureSize);

	// znacka
	double distanceB;
	int sx, sy;
	if (this->markQuadrant == 1) {
		sx = 1; sy = 1;
	}else if (this->markQuadrant == 2) {
		sx = -1; sy = 1;
	}else if (this->markQuadrant == 3) {
		sx = -1; sy = -1;
	}else if (this->markQuadrant == 4) {
		sx = 1; sy = -1;
	}
	// pozice a velikost znacky
	double markCenterCoordX = 0 + (sx * 1.0 / this->textureSize * cos(PI / 4));
	double markCenterCoordY = 0 + (sy * 1.0 / this->textureSize * sin(PI / 4));
	double markRadius = 0.05 / this->textureSize;
	if (this->markType == 1) {
		distanceB = this->distanceFromCircle(pointCoordX, pointCoordY, markCenterCoordX, markCenterCoordY, markRadius);
	}
	else { // this->markType == 2
		distanceB = this->distanceFromSquareEdge(pointCoordX, pointCoordY, markCenterCoordX, markCenterCoordY, markRadius);
	}

	// vne hlavniho kruhu a znacky nepruhledne
	if (distanceA < 0 && distanceB < 0) return 255;
	// postupne se zvetsujici pruhlednost na hrane
	if (distanceA < 0.004 && distanceB < 0.004) return (distanceA > distanceB) ? 255 * (1 - distanceA * 250) : 255 * (1 - distanceB * 250);
	// uvnitr pruhledne
	return 0;
}

double borderLayer::distanceFromCircle(double pointCoordX, double pointCoordY, double circleCenterCoordX, double circleCenterCoordY, double radius) {
	return radius - sqrt(pow(abs(pointCoordX - circleCenterCoordX), 2) + pow(abs(pointCoordY - circleCenterCoordY), 2));
}

double borderLayer::distanceFromSquareEdge(double pointCoordX, double pointCoordY, double squareCenterCoordX, double squareCenterCoordY, double sideHalfLength) {
	pointCoordX = pointCoordX - squareCenterCoordX;
	pointCoordY = pointCoordY - squareCenterCoordY;
	double rotationAngle = PI / 4;
	if (this->markQuadrant == 2 || this->markQuadrant == 4) {
		rotationAngle = -rotationAngle;
	}
	double newPointCoordX = pointCoordX * cos(rotationAngle) + pointCoordY * sin(rotationAngle);
	double newPointCoordY = pointCoordY * cos(rotationAngle) - pointCoordX * sin(rotationAngle);

	return (sideHalfLength - abs(newPointCoordX) < sideHalfLength - abs(newPointCoordY)) ? sideHalfLength - abs(newPointCoordX) : sideHalfLength - abs(newPointCoordY);
}
