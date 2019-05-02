#include "pch.h"
#include "adapter.h"

adapter::adapter(vascular * newTechlogie, bifurcationPoint * oldTechnologie, double moveCoordX, double moveCoordY, inferiorOrSuperior signY, nasalOrMacular signX)
{
	this->vascularPointsTree = newTechlogie;
	this->bifurcationPointTreeRoot = oldTechnologie;
	this->bifurcationMoveCoordX = moveCoordX;
	this->bifurcationMoveCoordY = moveCoordY;
	this->signCoordX = signX;
	this->signCoordY = signY;
}

bifurcationPoint * adapter::getBifurcationPointRoot()
{
	if (this->bifurcationPointTreeRoot != nullptr)
		return this->bifurcationPointTreeRoot;
	return nullptr;
}

double mirorCoordXInSection(double positionX, nasalOrMacular vascularSector)
{
	if (vascularSector == nasal)
		return -1 * positionX;
	if (vascularSector == macular)
		return positionX;
}

double mirorCoordYInSection(double positionY,inferiorOrSuperior vascularSector)
{
	if (vascularSector == inferior)
		return -1 * positionY;
	if (vascularSector == superior)
		return positionY;
}


bifurcationPoint* copyToBifurcationPointData(vascularPoint* readPoint,bifurcationPoint* parentNod, double moveCoordX, double moveCoordY, inferiorOrSuperior signY, nasalOrMacular signX)
{
	if (readPoint == nullptr)
		return nullptr;
	bifurcationPointLight* tmpPoint = new bifurcationPointLight();
	tmpPoint->setAndConvertPositionX(mirorCoordXInSection(readPoint->getCordX()-moveCoordX, signX));
	tmpPoint->setAndConvertPositionY(mirorCoordYInSection(readPoint->getCordY()+moveCoordY, signY));
	tmpPoint->setType(1);
	tmpPoint->setWidthRation(readPoint->getBifurcationRatio());
	tmpPoint->setPrevDistance(parentNod->getPositionX(),parentNod->getPositionY(),tmpPoint->getPositionX(),tmpPoint->getPositionY());
	tmpPoint->setDirectionType(0);
	tmpPoint->setDirectionAngle(parentNod->getPositionX(),parentNod->getPositionY(),tmpPoint->getPositionX(),tmpPoint->getPositionY());
	tmpPoint->setWidth(parentNod->getWidth());

	if (tmpPoint->getWidth() < 0.01)
		return nullptr;
	
	bifurcationPoint* actualBifurcationPoint =
		new bifurcationPoint(tmpPoint->getPositionX(), tmpPoint->getPositionY(), tmpPoint->getType(), tmpPoint->getWidth(),
			tmpPoint->getPrevDistance(), tmpPoint->getWidthRatio(), parentNod, tmpPoint->getDirectionType(),tmpPoint->getDirectionAngle());

	actualBifurcationPoint->setNextPointLeft(copyToBifurcationPointData(readPoint->getLeftPoint(), actualBifurcationPoint,moveCoordX,moveCoordY,signY,signX));
	actualBifurcationPoint->setNextPointRight(copyToBifurcationPointData(readPoint->getRightPoint(), actualBifurcationPoint, moveCoordX, moveCoordY,signY,signX));
	return actualBifurcationPoint;
}

void adapter::adapteeTree()
{
	bifurcationPoint* preRoot = new bifurcationPoint(this->bifurcationMoveCoordX, this->bifurcationMoveCoordY,0,1.0,0.0,1.0,nullptr,1,45);
	this->bifurcationPointTreeRoot = 
		copyToBifurcationPointData(this->vascularPointsTree->getRoot(),preRoot,this->bifurcationMoveCoordX,this->bifurcationMoveCoordY,this->signCoordY,this->signCoordX);

}

void adapter::printBifurcationTree()
{
	bifurcationPoint* actualPointer = this->bifurcationPointTreeRoot;
	if (actualPointer == nullptr)
		return;
	std::vector <bifurcationPoint*> queue;
	queue.push_back(actualPointer);
	while (!queue.empty())
	{
		actualPointer = queue.front();
		queue.erase(queue.begin());
		std::cout << "#############################"<<std::endl;
		std::cout << "X: " <<actualPointer->getPositionX() << "\tY: " << actualPointer->getPositionY() << std::endl;
		std::cout << "width: " << actualPointer->getWidth() << "\twidthRatio: " << actualPointer->getWidthRatio() << std::endl;
		if (actualPointer->getNextPointLeft() != nullptr)
			queue.push_back(actualPointer->getNextPointLeft());
		if (actualPointer->getNextPointRight() != nullptr)
			queue.push_back(actualPointer->getNextPointRight());

	}
}
