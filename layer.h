#ifndef LAYER_H
#define LAYER_H

#include <SFML/OpenGL.hpp>
#include <iostream>
#include <vector>
#include <cmath> 

#include "main.h"

class layer {
protected:
	layer *layerBelow; // pod
	layer *layerAbove; // nad

	int windowWidth;
	int windowHeight;
	double depth;
	double widthAR;
	double heightAR;

	double layerSize; // zoom ve vykreslovacím oknì
	double textureSize; // zoom textur za okulárem
	int textureWidth;
	int textureHeight;
	GLubyte *textureImage;
	GLubyte *textureImageTmp;
	bool textureChange;

	void recalculateAspectRatio(); // pomer stran obrazu
	// alokovani pameti pro texturu
	void createNewTexureImage(bool exist);
	void createNewTexureImageTmp(bool exist);
	// vztvoreni obrazu textury
	virtual void makeTextureImage() = 0;

	double textureShiftX;
	double textureShiftY;
	double textureRotation;

	// prevod souradnic textury do vnitrniho souradnicoveho systemu 
	double convertCoordX(int coord);
	double convertCoordY(int coord);
	// prevod souradnic vnitrniho souradnicoveho systemu do souradnic textury
	int convertCoordXback(double coord);
	int convertCoordYback(double coord);

	// rotace souradnic
	void rotationCoord(double coordX, double coordY, double *newCoordX, double *newCoordY, double rotationAngle);

	// aby se  prepocitaly i textury vyse pri zmene transformacnich hodnot (nepouzito)
	void textureChangeAbove();

	// vrati trasformacni parametry i z vrstev pod aktualni
	void getTextureShiftX(std::vector<double> *shiftX);
	void getTextureShiftY(std::vector<double> *shiftY);
	void getTextureRotation(std::vector<double> *rotation);

public:
	~layer();
	// nastaveni ukazatele na nizsi a vyssi vrstvu
	void setLayerBelow(layer *l);
	void setLayerAbove(layer *l);
	// vykresli vrstvu s texturou do vykreslovaciho okna
	int drawLayer();

	// zmena nekterych parametru vrstvy pri behu programu (nepouzito)
	void changeWindowSize(int width, int height);
	void changeLayerSize(double size);
	void changeTextureSize(double size);
	void changeTextureShiftX(double shift);
	void changeTextureShiftY(double shift);
	void changeTextureRotation(double angle);

};

#endif