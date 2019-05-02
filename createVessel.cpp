#include "createVessel.h"

createVessel::createVessel()
{
}

bifurcationPoint * createVessel::createBranch(inferiorOrSuperior inferiorSuperior, nasalOrMacular nasalMacular, double legacyXq2, double legacyYq1)
{
	vascular* actualVasscular = new vascular(new vascularPoint(0.0, 0.0, 1.0), 0.0, 0.0,nasalMacular);
	actualVasscular->makeTreeStructure(nasalMacular);

	deleteCrossingVascular* clearCrossingVasscular = new deleteCrossingVascular();
	clearCrossingVasscular->clearCrossingControler(actualVasscular->getRoot());

	//actualVasscular->printTreeSructure();

	bifurcationPoint* newTree = nullptr;
	adapter* newInterface = new adapter(actualVasscular, newTree, legacyXq2, legacyYq1, inferiorSuperior, nasalMacular);
	newInterface->adapteeTree();
	
	return newInterface->getBifurcationPointRoot();
}


createVessel::~createVessel()
{
}
