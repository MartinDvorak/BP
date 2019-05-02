#ifndef CREATE_VESSEL_H
#define CREATE_VESSEL_H
#include "adapter.h"
#include "deleteCrossingVascular.h"
class createVessel
{

public:
	createVessel();
	bifurcationPoint* createBranch(inferiorOrSuperior inferiorSuperior,nasalOrMacular nasalMacular,double legacyXq2, double legacyYq1);

	~createVessel();
};

#endif