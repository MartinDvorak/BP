#include "renderWindow.h"

renderWindow::renderWindow(int width, int height, unsigned seed, std::string path, double layerSize, double textureSize, int eye, int markQuadrant, int markType) {
	this->width = width;
	this->height = height;
	this->seed = seed;
	this->pathImage = path;
	this->layer1 = new borderLayer(width, height, 0.0, markQuadrant, markType, layerSize);
	this->layer2 = new vascularityLayer(width, height, 0.1, layerSize, textureSize);
	this->layer3 = new opticDiscLayer(width, height, 0.2, layerSize, textureSize, eye);
	this->layer4 = new backgroundLayer(width, height, 0.3, layerSize, textureSize, seed);

	// ukazatele na vrstvy nad a pod
	this->layer1->setLayerBelow(nullptr);
	this->layer1->setLayerAbove(nullptr);
	this->layer2->setLayerBelow(this->layer3);
	this->layer2->setLayerAbove(nullptr);
	this->layer3->setLayerBelow(this->layer4);
	this->layer3->setLayerAbove(this->layer2);
	this->layer4->setLayerBelow(nullptr);
	this->layer4->setLayerAbove(this->layer3);
}

renderWindow::~renderWindow() {
	delete(this->layer1);
	delete(this->layer2);
	delete(this->layer3);
	delete(this->layer4);
}

int renderWindow::run() {
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 8;
	settings.majorVersion = 3;
	settings.minorVersion = 1;
	settings.attributeFlags = sf::ContextSettings::Core;

	sf::RenderWindow window(sf::VideoMode(this->width, this->height, 32), "", 0);
	window.setVerticalSyncEnabled(true);

	//window.setActive(true);
	sf::Image Screenshot;
	bool running = true;
	while (running) {
		// events check
		/*sf::Event windowEvent;
		while (window.pollEvent(windowEvent)) {
			switch (windowEvent.type) {
			case sf::Event::Closed:
				running = false;
				break;
			case sf::Event::Resized:
				glViewport(0, 0, windowEvent.size.width, windowEvent.size.height);
				break;
			default:
				break;
			}
		}*/

		window.clear();
		// draw OpenGL
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_DEPTH_TEST);

		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		// vykresleni vrstev
		layer4->drawLayer();
		layer3->drawLayer();
		layer2->drawLayer();
		layer1->drawLayer();

		// zkopirovani obsahu renderovaciho okna pro ulozeni
		BYTE* pixels = new BYTE[3 * this->width];
		Screenshot.create(this->width, this->height, sf::Color(0, 0, 0));
		for (int y = 0, yn = this->height - 1; y < this->height; y++, yn--){
			glReadPixels(0, y, this->width, 1, GL_RGB, GL_UNSIGNED_BYTE, pixels);
			for (int x = 0; x < this->width; x++) {
				Screenshot.setPixel(x, yn, sf::Color((int)pixels[x * 3 + 0], (int)pixels[x * 3 + 1], (int)pixels[x * 3 + 2]));
			}
		}
		delete(pixels);

		running = false;
	}
	// ulozeni snimku
	std::string imageName;
	imageName = this->pathImage + std::to_string(seed) + ".png";
	if (Screenshot.saveToFile(imageName))
	{
		std::cout << "Image: " + imageName + " was successfully saved\n";
	}

	return EXIT_SUCCESS;
}

