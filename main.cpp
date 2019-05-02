#include "main.h"
#include "renderWindow.h"
#include "enableExtension.h"

int main(int argc, char *argv[]) {
	time_t t;

	int numberImages, width, height, eye, markQuadrant, markType;
	std::string path;
	double layerSize, textureSize;
	loadConfigFile(&numberImages, &width, &height, &path, &layerSize, &textureSize, &eye, &markQuadrant, &markType);

	while (numberImages-- > 0) {
		unsigned seed = (unsigned)time(&t);
		srand(seed);
		renderWindow *renderWin = new renderWindow(width, height, seed, path, layerSize, textureSize, eye, markQuadrant, markType);
		renderWin->run();
		delete(renderWin);
	}

	return EXIT_SUCCESS;
}

double randRange(double min, double max) {
	double r = (double)rand() / RAND_MAX;
	return min + r * (max - min);
}

double normalDistributionProb(double value, double min, double max) {
	double my = 3, sigma = 1, shiftY = -0.042;
	if (min < 0) value = (value + (-1 * min)) / ((max + (-1 * min)) / 6);
	else value = (value - min) / ((max - min) / 6);
	return (1.0 / (sigma * sqrt(2 * PI))) * pow(E, -(pow(value - my, 2) / pow(2 * sigma, 2))) + shiftY;
}

double randRangeND(double min, double max) {
	double n;
	do {
		n = randRange(min, max);
	} while (normalDistributionProb(n, min, max) > randRange(0.0, 1.0));
	return n;
}

void loadConfigFile(int *numberImages, int *width, int *height, std::string *path, double *layerSize, double *textureSize, int *eye, int *markQuadrant, int *markType) {
	std::ifstream fin("./RIGconfig.csv");
	std::string line, item;
	std::vector<std::string> valueline;
	std::getline(fin, line);
	std::istringstream in(line);

	while (getline(in, item, ',')) {
		valueline.push_back(item);
	}
	*numberImages = 1;
	*width = 1000;
	*height = 800;
	*path = "./";
	*layerSize = 1.0;
	*textureSize = 1.5;
	*eye = 0;
	*markQuadrant = 1;
	*markType = 1;


	int size = valueline.size();
	switch (size) {
		case 10:
			if ((valueline.at(9)).compare("true") == 0)
				enableExtensionOn = true;
			else
				enableExtensionOn = false;
		case 9:
			*markType = std::stoi(valueline.at(8));
			if (*markType < 1 || *markType > 2) *markType = 1;
		case 8:
			*markQuadrant = std::stoi(valueline.at(7));
			if (*markQuadrant < 1 || *markQuadrant > 4) *markQuadrant = 1;
		case 7:
			*eye = std::stoi(valueline.at(6));
			if (*eye < 0 || *eye > 2) *eye = 0;
		case 6:
			*textureSize = std::stod(valueline.at(5));
			if (*textureSize < 1.0 || *textureSize > 2.0) *textureSize = 1.5;
		case 5:
			*layerSize = std::stod(valueline.at(4));
			if (*layerSize < 0.7 || *layerSize > 1.3) *layerSize = 1.0;
		case 4:
			*path = valueline.at(3);
		case 3:
			*height = std::stoi(valueline.at(2));
			if (*height < 200) *height = 200;
			if (*height > 2160) *height = 2160;
		case 2:
			*width = std::stoi(valueline.at(1));
			if (*width < 200) *width = 200;
			if (*width > 3840) *width = 3840;
		case 1:
			*numberImages = std::stoi(valueline.at(0));
			if (*numberImages < 1) *numberImages = 1;
		default: break;
	}
	valueline.clear();
}