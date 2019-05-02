#pragma once
#include "vascularPoints.h"
#include <vector>
#include <iostream>
#include <tuple>
#include <fstream>

#define ROUND 0.5

class deleteCrossingVascular
{
	std::vector<vascularPoint*> queue;

	bool mask[100][70];

	std::ofstream log;

	void clearCrossing(vascularPoint* actualRoot);
	vascularPoint* selectChild(vascularPoint* actualRoot);
	bool markPath(vascularPoint* pointFrom, vascularPoint* pointTo);
	void setThisChildToNull(vascularPoint* actualRoot, vascularPoint* child);
	bool setVisitedPoints(std::vector< std::tuple<int, int> > turnToTrue);
	std::vector< std::tuple<int, int> > DDAAlgorithm(int coordStardX, int coordStardY, int coordEndX, int coordEndY);

public:
	deleteCrossingVascular();
	void clearCrossingControler(vascularPoint* root);
	~deleteCrossingVascular();
};

