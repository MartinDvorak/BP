#include "createVessel.h"



createVessel::createVessel()
{
}

bifurcationPoint * createVessel::createBranch(inferiorOrSuperior inferiorSuperior, nasalOrMacular nasalMacular, double legacyXq2, double legacyYq1)
{
	vascular* speriorMacularArterioly = new vascular(new vascularPoint(0.0, 0.0, 1.0), 0.0, 0.0);
	speriorMacularArterioly->makeTreeStructure();
	//TODO
	speriorMacularArterioly->printTreeSructure();

	bifurcationPoint* newTree = nullptr;
	adapter* newInterface = new adapter(speriorMacularArterioly, newTree, legacyXq2, legacyYq1, inferiorSuperior, nasalMacular);
	newInterface->controlPassageTree();
	//TODO	
	newInterface->printBifurcationTree();
	
	return newInterface->getBifurcationPointRoot();
}


createVessel::~createVessel()
{
}
