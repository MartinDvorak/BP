#ifndef RENDERWINDOW_H
#define RENDERWINDOW_H

#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Graphics.hpp>
#include <string>

#include "borderLayer.h"
#include "vascularityLayer.h"
#include "opticDiscLayer.h"
#include "backgroundLayer.h"

class renderWindow {
private:
	// rozmery okna
	int width; 
	int height;
	// seed pro inicializaci Perlinova sumu a jako nazev vytvoreneho snimku
	unsigned seed;
	// cesta ke slozce kam ulozit snimek
	std::string pathImage;
	// objekty vrstev
	borderLayer *layer1;
	vascularityLayer *layer2;
	opticDiscLayer *layer3;
	backgroundLayer *layer4;

public:
	renderWindow(int width, int height, unsigned seed, std::string path, double layerSize, double textureSize, int eye, int markQuadrant, int markType);
	~renderWindow();
	// beh vykreslovaciho okna
	int run();
};

#endif