#ifndef VASCULAR_H
#define VASCULAR_H

#include "vascularPoints.h"
#include "functionControler.h"
#include <iostream>
#include <vector>
#include "realisticExtensionConstant.h"

class vascular {
	vascularPoint* root;
	double startCoordX;
	double startCoordY;
	nasalOrMacular specificMaximumX;

public:
	vascular(vascularPoint* root, double startCoordX, double startCoordY,nasalOrMacular specificMaximumX);
	void makeTreeStructure(nasalOrMacular nasalMacular);
	vascularPoint* getRoot();
	void printTreeSructure();
};

#endif 