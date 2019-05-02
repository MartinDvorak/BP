#include "deleteCrossingVascular.h"

void deleteCrossingVascular::clearCrossing(vascularPoint * actualRoot)
{
	vascularPoint* selectedChild = this->selectChild(actualRoot);
	while (selectedChild != nullptr)
	{
		if(!this->markPath(actualRoot,selectedChild))
		{ 
			this->setThisChildToNull(actualRoot, selectedChild);
			selectedChild = nullptr;
		}
		else {
			actualRoot = selectedChild;
			selectedChild = this->selectChild(actualRoot);
		}
	}
}

vascularPoint * deleteCrossingVascular::selectChild(vascularPoint * actualRoot)
{
	vascularPoint* leftPoint = actualRoot->getLeftPoint();
	vascularPoint* rightPoint = actualRoot->getRightPoint();
	if ((leftPoint == nullptr) && (rightPoint == nullptr))
	{
		return nullptr;
	}
	else if ((leftPoint == nullptr) && (rightPoint != nullptr))
	{
		return rightPoint;
	}
	else if ((leftPoint != nullptr) && (rightPoint == nullptr))
	{
		return leftPoint;
	}
	else {
		if (leftPoint->getBifurcationRatio() > rightPoint->getBifurcationRatio())
		{
			this->queue.push_back(rightPoint);
			return leftPoint;
		}
		else {
			this->queue.push_back(leftPoint);
			return rightPoint;
		}
	}
}

bool isEnd(double actualPointX, double signX, double pointToX)
{
	if ((actualPointX+ROUND)*signX > (pointToX+ROUND)*signX)
		return true;
	return false;
}

std::vector< std::tuple<int, int> > deleteCrossingVascular::DDAAlgorithm(int coordStardX, int coordStardY, int coordEndX, int coordEndY)
{
	std::vector< std::tuple<int, int> > pointToMask;
	double derivate = (coordEndY - coordStardY) / (coordEndX - coordStardX);
	double actualY = coordStardY;
	for (int actualX = coordStardX; actualX < coordEndX; actualX++)
	{
		pointToMask.push_back(std::make_tuple(actualX,(int)actualY));
		actualY += derivate;
	}
	return pointToMask;
}
void swap(double* valueA, double* valueB)
{
	double tmp = *valueA;
	*valueA = *valueB;
	*valueB = tmp;
}

std::vector<std::tuple<int, int>> swapPointsAxis(std::vector<std::tuple<int, int>> swapedAxies)
{
	std::vector<std::tuple<int, int>> axiesInRightOrder;
	std::tuple<int, int> actualPoint;
	int pointX, pointY;
	while (!swapedAxies.empty())
	{
		actualPoint = swapedAxies.front();
		swapedAxies.erase(swapedAxies.begin());
		pointY = std::get<0>(actualPoint);
		pointX = std::get<1>(actualPoint);	
		axiesInRightOrder.push_back(std::make_tuple(pointX, pointY));
	}
	return axiesInRightOrder;
}

bool deleteCrossingVascular::markPath(vascularPoint * pointFrom, vascularPoint * pointTo)
{
	double pointFromX = pointFrom->getCordX();
	double pointFromY = pointFrom->getCordY();

	double pointToX = pointTo->getCordX();
	double pointToY = pointTo->getCordY();

	double vectorX = pointToX - pointFromX;
	double vectorY = pointToY - pointFromY;
	
	bool isSwapedAxis = false;
	
	if (std::abs(vectorX) < std::abs(vectorY))
	{
		swap(&pointFromX, &pointFromY);
		swap(&pointToX, &pointToY);
		isSwapedAxis = true;
	}
	if (pointFromX > pointToX)
	{
		swap(&pointFromX, &pointToX);
		swap(&pointFromY, &pointToY);
	}
	std::vector<std::tuple<int,int>> pointToMask = DDAAlgorithm((int)pointFromX, (int)pointFromY, (int)pointToX, (int)pointToY);
	if (isSwapedAxis)
		pointToMask = swapPointsAxis(pointToMask);
	return setVisitedPoints(pointToMask);
}

bool isTheSamePoint(vascularPoint * pattern, vascularPoint * candidate)
{
	if ((candidate == nullptr) || (pattern == nullptr))
		return false;
	double patternX = pattern->getCordX();
	double patternY = pattern->getCordY();

	double candidateX = candidate->getCordX();
	double candidateY = candidate->getCordY();

	if ((patternX == candidateX) && (patternY == candidateY))
	{
		return true;
	}
	return false;
}

void deleteCrossingVascular::setThisChildToNull(vascularPoint * actualRoot, vascularPoint * child)
{
	vascularPoint* leftPoint = actualRoot->getLeftPoint();
	vascularPoint* rightPoint = actualRoot->getRightPoint();
	if (isTheSamePoint(rightPoint, child))
	{
		actualRoot->setRightPoint(nullptr);
	}
	else if (isTheSamePoint(leftPoint, child))
	{
		actualRoot->setLeftPoint(nullptr);
	}
}

bool deleteCrossingVascular::setVisitedPoints(std::vector<std::tuple<int, int>> turnToTrue)
{
	std::tuple<int, int> actualPoint;
	int pointX, pointY;
	while (!turnToTrue.empty())
	{
		actualPoint = turnToTrue.front();
		turnToTrue.erase(turnToTrue.begin());
		pointX = std::get<0>(actualPoint);
		pointY = std::get<1>(actualPoint);
		if (this->mask[pointX][pointY] == false)
			this->mask[pointX][pointY] = true;
		else
		{
			this->log << pointX << "\t" << pointY << "\n";
			return false;
		}
	}
	return true;
}

deleteCrossingVascular::deleteCrossingVascular()
{
	this->log.open("cross.txt");
	for (int i = 0; i < 100; i++)
	{
		for (int j = 0; j < 70; j++)
		{
			this->mask[i][j] = false;
		}
	}
}

void deleteCrossingVascular::clearCrossingControler(vascularPoint* root)
{
	while (root != nullptr)
	{
		root = selectChild(root);
	}

	while (!this->queue.empty())
	{
		vascularPoint* actualRoot = this->queue.front();
		this->queue.erase(this->queue.begin());
		clearCrossing(actualRoot);
	}
	this->log.close();
}


deleteCrossingVascular::~deleteCrossingVascular()
{
}
