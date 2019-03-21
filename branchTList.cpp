#include "branchTList.h"

branchTList* branchTList::instance = nullptr;

branchTList::branchTList()
{
}

branchTList * branchTList::getBranchTList()
{
	if (instance == nullptr)
	{
		instance = new branchTList();
	}
	return instance;
}

std::vector<bifurcationPoint*> branchTList::getVector()
{
	return this->branchT;
}

bifurcationPoint * branchTList::getFirst()
{
	return this->branchT.front();
}

void branchTList::insertItem(bifurcationPoint* insertItem)
{
	this->branchT.push_back(insertItem);
}

bool branchTList::isEmpty()
{
	return this->branchT.empty();
}
