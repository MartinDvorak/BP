
#ifndef BRANCH_T_LIST_H
#define BRANCH_T_LIST_H

#include <iostream>
#include <vector>
#include "vessel.h"

class branchTList {
private:
	static branchTList* instance;
	std::vector<bifurcationPoint*> branchT;
	branchTList();
public:
	static branchTList* getBranchTList();
	std::vector<bifurcationPoint*> getVector();
	bifurcationPoint* getFirst();
	void insertItem(bifurcationPoint* insertItem);
	bool isEmpty();
};

#endif