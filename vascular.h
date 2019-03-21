
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


public:
	vascular(vascularPoint* root, double startCoordX, double startCoordY);
	void makeTreeStructure();
	vascularPoint* getRoot();
	void printTreeSructure();
};

#endif 