#include "layer.h"

layer::~layer() {
	delete(this->textureImage);
}

void layer::setLayerBelow(layer *l) {
	this->layerBelow = l;
}

void layer::setLayerAbove(layer *l) {
	this->layerAbove = l;
}


int layer::drawLayer() {
	if (this->textureChange == true) {
		this->makeTextureImage(); // vytvori obraz textury
	}

	glShadeModel(GL_FLAT);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	GLuint texName;
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, this->textureWidth, this->textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, this->textureImage);

	// vytvori plochu vrstvy a namapuje souradnice textury
	glEnable(GL_TEXTURE_2D);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0, 0.0); glVertex3f(-this->layerSize * widthAR * this->textureSize, -this->layerSize * heightAR * this->textureSize, this->depth);
	glTexCoord2f(0.0, 1.0); glVertex3f(-this->layerSize * widthAR * this->textureSize, this->layerSize * heightAR * this->textureSize, this->depth);
	glTexCoord2f(1.0, 1.0); glVertex3f(this->layerSize * widthAR * this->textureSize, this->layerSize * heightAR * this->textureSize, this->depth);
	glTexCoord2f(1.0, 0.0); glVertex3f(this->layerSize * widthAR * this->textureSize, -this->layerSize * heightAR * this->textureSize, this->depth);
	glEnd();
	//glFlush();
	glDeleteTextures(1, &texName);
	glDisable(GL_TEXTURE_2D);

	return EXIT_SUCCESS;
}

void layer::createNewTexureImage(bool exist) {
	if (exist) {
		delete(this->textureImage);
	}
	this->textureWidth = (int)((this->windowWidth < this->windowHeight) ? this->windowWidth : this->windowHeight) * this->layerSize * this->textureSize;
	this->textureHeight = this->textureWidth;
	this->textureImage = new GLubyte[this->textureWidth * this->textureHeight * 4];
}
void layer::createNewTexureImageTmp(bool exist) {
	if (exist) {
		delete(this->textureImageTmp);
	}
	this->textureWidth = (int)((this->windowWidth < this->windowHeight) ? this->windowWidth : this->windowHeight) * this->layerSize * this->textureSize;
	this->textureHeight = this->textureWidth;
	this->textureImageTmp = new GLubyte[this->textureWidth * this->textureHeight * 4];
}

void layer::recalculateAspectRatio() {
	if (this->windowWidth > this->windowHeight) {
		this->widthAR = (1.0 / this->windowWidth) * this->windowHeight;
		this->heightAR = 1.0;
	}
	else {
		this->widthAR = 1.0;
		this->heightAR = (1.0 / this->windowHeight) * this->windowWidth;
	}
}

void layer::changeWindowSize(int width, int height) {
	this->windowWidth = width;
	this->windowHeight = height;
	this->createNewTexureImage(true);
	this->textureChange = true;

	this->recalculateAspectRatio();
}

void layer::changeLayerSize(double size) {
	this->layerSize = size;
	this->createNewTexureImage(true);
	this->textureChange = true;
}

void layer::changeTextureSize(double size) {
	this->textureSize = size;
	this->createNewTexureImage(true);
	this->textureChange = true;
}

double layer::convertCoordX(int coord) {
	return ((double)coord / this->textureWidth) * 2.0 - 1.0;
}

double layer::convertCoordY(int coord) {
	return ((double)coord / this->textureHeight) * 2.0 - 1.0;
}

int layer::convertCoordXback(double coord) {
	return ((coord + 1.0) * this->textureWidth) / 2;
}

int layer::convertCoordYback(double coord) {
	return ((coord + 1.0) * this->textureHeight) / 2;
}

void layer::rotationCoord(double coordX, double coordY, double *newCoordX, double *newCoordY, double rotationAngle) {
	rotationAngle = (rotationAngle * PI) / 180;
	*newCoordX = coordX * cos(rotationAngle) + coordY * sin(rotationAngle);
	*newCoordY = coordY * cos(rotationAngle) - coordX * sin(rotationAngle);
}

void layer::textureChangeAbove() {
	if (this->layerAbove){
		this->layerAbove->textureChangeAbove();
	}
	this->textureChange = true;
}

void layer::changeTextureShiftX(double shift) {
	this->textureShiftX = shift;
	this->textureChangeAbove();
}

void layer::changeTextureShiftY(double shift) {
	this->textureShiftY = shift;
	this->textureChangeAbove();
}

void layer::changeTextureRotation(double angle) {
	this->textureRotation = angle;
	this->textureChangeAbove();
}

void layer::getTextureShiftX(std::vector<double> *shiftX) {
	if (this->layerBelow)
		this->layerBelow->getTextureShiftX(shiftX);
	shiftX->push_back(this->textureShiftX);
}

void layer::getTextureShiftY(std::vector<double> *shiftY) {
	if (this->layerBelow)
		this->layerBelow->getTextureShiftY(shiftY);
	shiftY->push_back(this->textureShiftY);
}

void layer::getTextureRotation(std::vector<double> *rotation) {
	if (this->layerBelow)
		this->layerBelow->getTextureRotation(rotation);
	rotation->push_back(this->textureRotation);
}