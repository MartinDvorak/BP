#ifndef ADAPTER_H
#define ADAPTER_H

#include "vascular.h"
#include "vessel.h"
#include "bifurcationPointLight.h"
#include "branchTList.h"


enum inferiorOrSuperior{inferior, superior};
enum nasalOrMacular {nasal, macular};


class adapter {
	vascular* vascularPointsTree;
	bifurcationPoint *bifurcationPointTreeRoot;
	double bifurcationMoveCoordX;
	double bifurcationMoveCoordY;
	inferiorOrSuperior signCoordY;
	nasalOrMacular signCoordX;

public:
	adapter(vascular* newTechlogie, bifurcationPoint* oldTechnologie,double moveCoordX, double moveCoordY, inferiorOrSuperior signY, nasalOrMacular signX);
	bifurcationPoint* getBifurcationPointRoot();
	void controlPassageTree();
	void printBifurcationTree();
};

#endif