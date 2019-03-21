#pragma once
#include "adapter.h"

class createVessel
{

public:
	createVessel();
	bifurcationPoint* createBranch(inferiorOrSuperior inferiorSuperior,nasalOrMacular nasalMacular,double legacyXq2, double legacyYq1);

	~createVessel();
};

