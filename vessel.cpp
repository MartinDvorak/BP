#include "vessel.h"

vessel::vessel(int type, int quadrant, double width, double startX, double startY, double xq, double yq, double directionFceType) {
	this->type = type;
	this->quadrant = quadrant;
	this->width = width;
	this->startX = startX;
	this->startY = startY;
	this->startPoint = new bifurcationPoint(0, 1.0, 0.0, 1.0, nullptr, 0.8, true, 0, 0); // vytvoreni stromu vetveni
	
	// pomocne pole pro vzkreslovani vypocitanych pozic bodu a cev mezi nimi
	double *ra = new double[RA_DIM *RA_DIM];
	for (int i = 0; i < RA_DIM; i++) {
		for (int j = 0; j < RA_DIM; j++) {
			ra[i * RA_DIM + j] = 0.0;
		}
	}
	std::vector<double*> points;
	// vykresleni hranic kvadrantu
	if (this->quadrant == 1) {
		this->getPointInLine(this->convertCoordX(this->startX) - 1, this->convertCoordY(this->startY) - 1, this->convertCoordX(this->startX + xq), this->convertCoordY(1.0), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->getPointInLine(this->convertCoordX(this->startX) - 1, this->convertCoordY(this->startY) - 1, this->convertCoordX(1.0), this->convertCoordY(this->startY + yq), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->startPoint->setDirectionAngle(45);
	}else if (this->quadrant == 2) {
		this->getPointInLine(this->convertCoordX(this->startX) + 1, this->convertCoordY(this->startY) - 1, this->convertCoordX(this->startX + xq), this->convertCoordY(1.0), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->getPointInLine(this->convertCoordX(this->startX) + 1, this->convertCoordY(this->startY) - 1, this->convertCoordX(-1.0), this->convertCoordY(this->startY + yq), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->startPoint->setDirectionAngle(135);
	}else if (this->quadrant == 3) {	
		this->getPointInLine(this->convertCoordX(this->startX) + 1, this->convertCoordY(this->startY) + 1, this->convertCoordX(this->startX + xq), this->convertCoordY(-1.0), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->getPointInLine(this->convertCoordX(this->startX) + 1, this->convertCoordY(this->startY) + 1, this->convertCoordX(-1.0), this->convertCoordY(this->startY + yq), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->startPoint->setDirectionAngle(225);
	}else if (this->quadrant == 4) {
		this->getPointInLine(this->convertCoordX(this->startX) - 1, this->convertCoordY(this->startY) + 1, this->convertCoordX(this->startX + xq), this->convertCoordY(-1.0), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->getPointInLine(this->convertCoordX(this->startX) - 1, this->convertCoordY(this->startY) + 1, this->convertCoordX(1.0), this->convertCoordY(this->startY + yq), ra, &points, false);
		this->drawPoints(&points, 2.0, 2.0);
		this->startPoint->setDirectionAngle(315);
	}

	this->startPoint->setPositionX(this->startX);
	std::cout << "startX: " << this->startX << "xq: " << xq << std::endl;
	std::cout << "startY: " << this->startY << "yq: " << yq << std::endl;

	this->startPoint->setPositionY(this->startY);
	std::vector<bifurcationPoint*> pointsQue;
	pointsQue.push_back(this->startPoint->getNextPointRight());
	// vzpocitani pozic vetveni
	this->genBifurcationPointCoord(&pointsQue, ra, this->quadrant, directionFceType);
	delete(ra);
}
vessel::vessel(int type, int quadrant, double width, double startX, double startY, double xq, double yq)
{
	this->type = type;
	this->quadrant = quadrant;
	this->width = width;
	this->startX = startX;
	this->startY = startY;
}

vessel::~vessel() {
	delete(this->startPoint);
}

bifurcationPoint* vessel::getStartPoint() {
	return this->startPoint;
}

int vessel::getType() {
	return this->type;
}
double vessel::getWidth() {
	return this->width;
}

void vessel::setStartPoint(bifurcationPoint * rootPoint)
{
	this->startPoint = rootPoint;
}

void vessel::genBifurcationPointCoord(std::vector<bifurcationPoint*> *pointsQue, double *ra, int quadrant, int directionFceType) {
	if (pointsQue->size() == 0) return;
	bifurcationPoint *point = pointsQue->back();
	pointsQue->pop_back();
	bifurcationPoint *prevPoint = point->getPrevPoint();

	std::vector<double*> points;
	double prevX = prevPoint->getPositionX();
	double prevY = prevPoint->getPositionY();
	if (prevX < -1.0 || prevY < -1.0 || prevX > 1.0 || prevY > 1.0) { // zrusit bod - mimo rozsah textury
		this->deletePoint(point);
		this->genBifurcationPointCoord(pointsQue, ra, quadrant, directionFceType);
		return;
	}

	// rozsah uhlù, kterými mùže pokraèovat, + jejich váhy
	int startAngle, endAngle;
	std::vector<double> weightAngle;
	if(point->getDirectionType() == 5) this->getAngleSpan(point, &startAngle, &endAngle, &weightAngle, true);
	else this->getAngleSpan(point, &startAngle, &endAngle, &weightAngle, false);
	
	int cx = -1, cy = -1;
	std::vector<std::vector<double>> weight;
	for (int angle = startAngle; angle <= endAngle; angle++) {
		double wa = weightAngle.back();
		weightAngle.pop_back();
		double x = prevX + (point->getPrevDistance() * cos((angle * PI) / 180));
		double y = prevY + (point->getPrevDistance() * sin((angle * PI) / 180));
		if (cx == convertCoordX(x) && cy == convertCoordY(y)) continue;
		cx = convertCoordX(x); cy = convertCoordY(y);
		
		if ((sqrt(pow(abs(x), 2) + pow(abs(y), 2)) - 0.06) < 0.0) continue; // bod je ve fovealni avascularni zone
		this->getPointInLine(this->convertCoordX(prevX), this->convertCoordY(prevY), cx, cy, ra, &points, false);
		if (checkPoints(&points, prevPoint->getWidth())) {
			// okoli bodu
			double wp = 0.0;
			for (int angle2 = 1; angle2 <= 360; angle2++) {
				if (this->convertCoordX(x + (0.02 * cos(((angle2)* PI) / 180))) == this->convertCoordX(x + (0.02 * cos(((angle2 - 1) * PI) / 180))) &&
					this->convertCoordY(y + (0.02 * sin(((angle2)* PI) / 180))) == this->convertCoordY(y + (0.02 * sin(((angle2 - 1) * PI) / 180)))) continue;
				this->getPointInLine(cx, cy, this->convertCoordX(x + (0.02 * cos((angle2 * PI) / 180))), this->convertCoordY(y + (0.02 * sin((angle2 * PI) / 180))), ra, &points, true);
				wp = weightPoints(&points);
				if (wp > 0) break;
			}
			if (wp > 0) continue;

			// vaha znacici blizkost okolnich cev
			double w1 = 0.0;
			for (int angle2 = angle + 360 - 90; angle2 <= angle + 360 + 90; angle2 = angle2 + 10) {
				this->getPointInLine(cx, cy, this->convertCoordX(x + (1.0 * cos((angle2 * PI) / 180))), this->convertCoordY(y + (1.0 * sin((angle2 * PI) / 180))), ra, &points, true);
				w1 = w1 + weightPoints(&points);
			}
			w1 = (w1 / 18);

			// vaha pro okrajove cevy urcujici jejich smer
			double w2 = 0.0;
			if (point->getDirectionType() != 5) { 
				w2 = this->getWeightDirectionType(x, y, point, quadrant, directionFceType); 
				if (w2 == DBL_MAX) continue;
			}
			
			std::vector<double> p = { 0, x, y, w1, w2, wa};
			weight.push_back(p);
		}
	}
	// normalizace vah 0 - 1
	double max = 0.000001;
	for (int i = 0; i < weight.size(); i++) if (weight[i][3] > max) max = weight[i][3];
	for (int i = 0; i < weight.size(); i++) weight[i][3] = 1 - (weight[i][3] / max);
	max = 0.001;
	for (int i = 0; i < weight.size(); i++) if (weight[i][4] > max) max = weight[i][4];
	for (int i = 0; i < weight.size(); i++) weight[i][4] = 1 - (weight[i][4] / max);

	// vypocet vysledne vahy
	for (int i = 0; i < weight.size(); i++) {
		if (point->getDirectionType() == 5) {
			if (weight[i][3] == 0.0 || weight[i][5] == 0.0) weight[i][0] = 0.0;
			else weight[i][0] = 0.6 * weight[i][3] + 0.4 * weight[i][5];
		}
		else {
			if (weight[i][3] == 0.0 || weight[i][4] == 0.0 || weight[i][5] == 0.0) weight[i][0] = 0.0;
			else weight[i][0] = 0.2 * weight[i][3] + 0.8 * weight[i][4];
		}
	}
	
	// kumulativni soucet celkove vahy
	std::sort(weight.begin(), weight.end(), [](std::vector<double> lhs, std::vector<double> rhs) { return lhs[0] < rhs[0];});
	for (int i = 1; i < weight.size(); i++) weight[i][0] = weight[i - 1][0] + weight[i][0];

	if (weight.size() == 0 || weight[weight.size() - 1][0] == 0) { // nelze vytvorit - zrusit
		this->deletePoint(point);
		this->genBifurcationPointCoord(pointsQue, ra, quadrant, directionFceType);
		return;
	} else { // nove souradnice
		double r = randRange(weight[weight.size() - 1][0] * 0.5, weight[weight.size() - 1][0]);
		for (int i = 0; i < weight.size(); i++) {
			if (weight[i][0] >= r) {
				this->getPointInLine(this->convertCoordX(prevX), this->convertCoordY(prevY), this->convertCoordX(weight[i][1]), this->convertCoordY(weight[i][2]), ra, &points, false);
				this->drawPoints(&points, prevPoint->getWidth(), point->getWidth());
				point->setPositionX(weight[i][1]);
				point->setPositionY(weight[i][2]);
				point->setDirectionAngle(((int)(atan2(weight[i][2] - prevY, weight[i][1] - prevX) * 180 / PI) + 360) % 360);
				break;
			}
		}
	}

	// pridani potomku do fronty a setrideni
	bifurcationPoint *nextPoint = point->getNextPointRight();
	if(nextPoint != nullptr) pointsQue->push_back(nextPoint);
	nextPoint = point->getNextPointLeft();
	if (nextPoint != nullptr) pointsQue->push_back(nextPoint);
	std::sort(pointsQue->begin(), pointsQue->end(), [](bifurcationPoint *lhs, bifurcationPoint *rhs) {
		if (lhs->getDirectionType() != rhs->getDirectionType())
			return lhs->getDirectionType() > rhs->getDirectionType();
		return lhs->getWidth() < rhs->getWidth();
	});
	this->genBifurcationPointCoord(pointsQue, ra, quadrant, directionFceType);
}
void vessel::getPointInLine(int x1, int y1, int x2, int y2, double *ra, std::vector<double*> *points, bool toFirstLine) {
	// inspirovano:
	// http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
	int w = x2 - x1;
	int h = y2 - y1;
	int dx1 = 0, dy1 = 0, dx2 = 0, dy2 = 0;
	if (w < 0) dx1 = -1; else if (w > 0) dx1 = 1;
	if (h < 0) dy1 = -1; else if (h > 0) dy1 = 1;
	if (w < 0) dx2 = -1; else if (w > 0) dx2 = 1;
	int longest = abs(w);
	int shortest = abs(h);
	if (!(longest > shortest)) {
		longest = abs(h);
		shortest = abs(w);
		if (h < 0) dy2 = -1; else if (h > 0) dy2 = 1;
		dx2 = 0;
	}
	int numerator = longest >> 1;
	int xp = x1, yp = y1;
	for (int i = 0; i <= longest; i++) {
		if (y1 < RA_DIM  && x1 < RA_DIM && y1  >= 0 && x1 >= 0) {
			if (xp != x1 && yp != y1) {
				if (xp < x1) {
					if (xp + 1 < RA_DIM && xp + 1 >= 0) {
						points->push_back(&ra[yp * RA_DIM + (xp + 1)]);
					}
				}
				else if (x1 < xp) {
					if (x1 + 1 < RA_DIM && x1 + 1 >= 0) {
						points->push_back(&ra[y1 * RA_DIM + (x1 + 1)]);
					}
				}
			}
			points->push_back(&ra[y1 * RA_DIM + x1]);
			xp = x1;
			yp = y1;
			if (toFirstLine) {
				if (*points->back() > 0.0) break;
			}
		}
		else break;
		numerator += shortest;
		if (!(numerator < longest)) {
			numerator -= longest;
			x1 += dx1;
			y1 += dy1;
		}
		else {
			x1 += dx2;
			y1 += dy2;
		}
	}
}
void vessel::drawPoints(std::vector<double*> *points, double widthA, double widthB) {
	double wk = (widthA - widthB) / points->size();
	int i = 0;
	while (points->size() != 0) {
		double *point = points->back();
		points->pop_back();
			*point = widthA;
		if(i++ == 0) *point = widthB;
	}
}
bool vessel::checkPoints(std::vector<double*> *points, double startWeight) {
	bool ret = true;
	while (points->size() != 0) {
		double *point = points->back();
		points->pop_back();
		if (*point != 0.0 && points->size() > 0) {
			if (points->size() == 1) {
				if(*point != startWeight) ret = false;
			}
			else ret = false;
		}
	}
	return ret;
}
double vessel::weightPoints(std::vector<double*> *points) {
	double weight = 0.0;
	if (points->size() > 0) weight = *points->at(points->size() - 1) / points->size();
	if (weight > 1.0) weight = 0.0;
	points->clear();
	return weight;
}
void vessel::deletePoint(bifurcationPoint *point) {
	bifurcationPoint *prevPoint = point->getPrevPoint();
	if (prevPoint->getNextPointRight() == point) {
		prevPoint->setNextPointRight(nullptr);
		if (prevPoint->getNextPointLeft() == nullptr) {
			prevPoint->setType(1);
			prevPoint->setWidth(0.0); // nemelo by byt pri ruseni kvuli pozici mimo texturu
		} else prevPoint->setType(0);
	}
	else {
		prevPoint->setNextPointLeft(nullptr);
		if (prevPoint->getNextPointRight() == nullptr) {
			prevPoint->setType(1);
			prevPoint->setWidth(0.0); // nemelo by byt pri ruseni kvuli pozici mimo texturu
		} else prevPoint->setType(0);
	}
	delete(point);
}
void vessel::getAngleSpan(bifurcationPoint *point, int *startAngle, int *endAngle, std::vector<double> *weightAngle, bool weightCo) {
	bifurcationPoint *prevPoint = point->getPrevPoint();
	bifurcationPoint *rightPoint = prevPoint->getNextPointRight();
	bifurcationPoint *leftPoint = prevPoint->getNextPointLeft();
	int type = prevPoint->getType();
	int directionAnglePrev = prevPoint->getDirectionAngle();
	if (rightPoint == point) { // aktualni je pravy potomek
		if (leftPoint == nullptr || leftPoint->getPositionX() == 0.0 && leftPoint->getPositionY() == 0.0) { // generuje se jako prvni
			switch (type) {
			case 0:
				*startAngle = directionAnglePrev + 360 - 45; *endAngle = directionAnglePrev + 360 + 45;
				break;
			case 2:
			case 3:
				if (weightCo) { *startAngle = directionAnglePrev + 360 - 60; *endAngle = directionAnglePrev + 360 + 10; }
				else { *startAngle = directionAnglePrev + 360 - 70; *endAngle = directionAnglePrev + 360 + 30; }
				break;
			default: break;
			}
		}
		else { // generuje se jako druhy
			int directionAngleFirst = leftPoint->getDirectionAngle();
			switch (type) {
			case 2:
			case 3:
				if (directionAngleFirst - directionAnglePrev > 40) { *startAngle = directionAngleFirst + 360 - 110; *endAngle = directionAngleFirst + 360 - 60; }
				else { *startAngle = directionAnglePrev + 360 - 90; *endAngle = directionAnglePrev + 360 - 30; }
				if(!weightCo){ *startAngle = directionAngleFirst + 360 - 110; *endAngle = directionAngleFirst + 360 - 50; }
				break;
			default: break;
			}
		}
	}
	else { // aktualni je levy potomek
		if (rightPoint == nullptr || rightPoint->getPositionX() == 0.0 && rightPoint->getPositionY() == 0.0) { // generuje se jako prvni
			switch (type) {
			case 0:
				*startAngle = directionAnglePrev + 360 - 45; *endAngle = directionAnglePrev + 360 + 45;
				break;
			case 2:
			case 3:
				if (weightCo) { *startAngle = directionAnglePrev + 360 - 10; *endAngle = directionAnglePrev + 360 + 60; }
				else { *startAngle = directionAnglePrev + 360 - 30; *endAngle = directionAnglePrev + 360 + 70; }
				break;
			default: break;
			}
		}
		else { // generuje se jako druhy
			int directionAngleFirst = rightPoint->getDirectionAngle();
			switch (type) {
			case 2:
			case 3:
				if (directionAnglePrev - directionAngleFirst > 40) { *startAngle = directionAngleFirst + 360 + 60; *endAngle = directionAngleFirst + 360 + 110; }
				else { *startAngle = directionAnglePrev + 360 + 30; *endAngle = directionAnglePrev + 360 + 90; }
				if (!weightCo) { *startAngle = directionAngleFirst + 360 + 50; *endAngle = directionAngleFirst + 360 + 110; }
				break;
			default: break;
			}
		}
	}
	for (int i = *startAngle; i <= *endAngle; i++) {
		weightAngle->push_back(normalDistributionProb(i, *startAngle, *endAngle));
	}
}
double vessel::getWeightDirectionType(double x, double y, bifurcationPoint *point, int quadrant, int vesselType) {
	double w = DBL_MAX;
	double prevX = point->getPrevPoint()->getPositionX();
	double prevY = point->getPrevPoint()->getPositionY();
	switch (quadrant) {
	case 1 :
		if ((point->getDirectionType() == 0 || point->getDirectionType() == 1) && x > prevX)
			w = abs(y - nasalDirectionFceNear(x, vesselType));
		else if (point->getDirectionType() == 2 && y > prevY)
			w = abs(y - nasalDirectionFceFar(x, vesselType));
		else if (point->getDirectionType() == 3) w = abs(y - 0);
		else if (point->getDirectionType() == 4) w = abs(x - 0.4);
		break;
	case 2:
		if ((point->getDirectionType() == 0 || point->getDirectionType() == 2) && x < prevX)
			w = abs(y - temporalDirectionFceNear(x, vesselType));
		else if (point->getDirectionType() == 1 && y > prevY)
			w = abs(y - temporalDirectionFceFar(x, vesselType));
		else if (point->getDirectionType() == 4) {
			if (x < -0.4) w = abs(y - 0);
			else {
				w = sqrt(pow(abs(x), 2) + pow(abs(y), 2)); // vzdalenost od fovey
				double prevDistFovea = sqrt(pow(abs(prevX), 2) + pow(abs(prevY), 2));
				if (w > prevDistFovea) w = prevDistFovea;
				if((x > 0 && prevX < 0) || (x < 0 && prevX > 0)) w = prevDistFovea;
			}
		} else if (point->getDirectionType() == 3) w = abs(x - 0.4);
		break;
	case 3:
		if ((point->getDirectionType() == 0 || point->getDirectionType() == 1) && x < prevX)
			w = abs(y + temporalDirectionFceNear(x, vesselType));
		else if (point->getDirectionType() == 2 && y < prevY)
			w = abs(y + temporalDirectionFceFar(x, vesselType));
		else if (point->getDirectionType() == 3) {
			if (x < -0.4) w = abs(y - 0);
			else {
				w = sqrt(pow(abs(x), 2) + pow(abs(y), 2)); // vzdalenost od fovey
				double prevDistFovea = sqrt(pow(abs(prevX), 2) + pow(abs(prevY), 2));
				if (w > prevDistFovea) w = prevDistFovea;
				if ((x > 0 && prevX < 0) || (x < 0 && prevX > 0)) w = prevDistFovea;
			}
		}
		else if (point->getDirectionType() == 4) w = abs(x - 0.4);
		break;
	case 4:
		if ((point->getDirectionType() == 0 || point->getDirectionType() == 2) && x > prevX)
			w = abs(y + nasalDirectionFceNear(x, vesselType));
		else if (point->getDirectionType() == 1 && y < prevY)
			w = abs(y + nasalDirectionFceFar(x, vesselType));
		else if (point->getDirectionType() == 4) w = abs(y - 0);
		else if (point->getDirectionType() == 3) w = abs(x - 0.4);
		break;
	default:
		break;
	}
	return w;
}

double vessel::temporalDirectionFceNear(double x, int directionFceType) {
	if (directionFceType == 0) {
		if (x > -0.1 && x <= 0.4) return sqrt(0.16 - (x * x));
		if (x <= -0.1) return 0.1 * pow(x + 1, 2) + 0.306298;
		return DBL_MAX;
	} else {
		if (x > -0.1 && x <= 0.4) return sqrt(0.16 - (x * x)) * 0.9;
		if (x <= -0.1) return 0.2 * pow(x + 1.2, 2) + 0.106569;
		return DBL_MAX;
	}
}
double vessel::temporalDirectionFceFar(double x, int directionFceType) {
	if (directionFceType == 0) {
		if (x > 0.32 && x <= 0.4) return sqrt(0.16 - (x * x));
		if (x <= 0.32) return -3 * (x - 0.4);
		return DBL_MAX;
	}
	else {
		if (x > 0.28 && x <= 0.4) return sqrt(0.16 - (x * x)) * 0.9;
		if (x <= 0.28) return -2.142 * (x - 0.4);
		return DBL_MAX;
	}
}
double vessel::nasalDirectionFceNear(double x, int directionFceType) {
	if (directionFceType == 0) {
		if (x >= 0.4) return 0.25 * (x - 0.4);
		return DBL_MAX;
	}
	else {
		if (x >= 0.4) return 0.25 * (x - 0.4) * 0.7;
		return DBL_MAX;
	}
}
double vessel::nasalDirectionFceFar(double x, int directionFceType) {
	if (directionFceType == 0) {
		if (x >= 0.4) return 1.75 * (x - 0.4);
		return DBL_MAX;
	}
	else {
		if (x >= 0.4) return 1.75 * (x - 0.4) * 0.7;
		return DBL_MAX;
	}
}
int vessel::convertCoordX(double coord) {
	return ((coord + 1.0) * RA_DIM) / 2;
}
int vessel::convertCoordY(double coord) {
	return ((coord + 1.0) * RA_DIM) / 2;
}

bifurcationPoint::bifurcationPoint(double positionX, double positionY, int type, double width, double prevDistance, double widthRatio, bifurcationPoint * prevPoint, int directionType,int directionAngle)
{
	this->positionX = positionX;
	this->positionY = positionY;
	this->type = type;
	this->width = width;
	this->prevDistance = prevDistance;
	this->widthRatio = widthRatio;
	this->prevPoint = prevPoint;
	this->directionType = directionType;
	this->directionAngle = directionAngle;
	this->curveWeight1 = randRange(0.8, 1.2);
	this->curveWeight2 = randRange(0.8, 1.2);
	this->curveEndDeviation = (randRange(0.0, 1.0) > 0.5) ? randRange(-90.0, -60.0) : randRange(60.0, 90.0);
}

bifurcationPoint::bifurcationPoint(int type, double width, double prevDistance, double widthRatio, bifurcationPoint *prevPoint, double bifurcationProb, bool strongest, int directionType, int direction) {
	this->positionX = 0.0;
	this->positionY = 0.0;
	this->type = type;
	this->width = width;
	this->prevDistance = prevDistance;
	this->widthRatio = widthRatio;
	this->prevPoint = prevPoint;
	this->directionType = directionType;
	this->curveWeight1 = randRange(0.8, 1.2);
	this->curveWeight2 = randRange(0.8, 1.2);
	this->curveEndDeviation = (randRange(0.0, 1.0) > 0.5) ? randRange(-90.0, -60.0): randRange(60.0, 90.0);

	// vzdalenost od predchoziho
	double nextDistanceR, nextDistanceL;
	if(this->width > 0.15){
		nextDistanceR = randRange(0.15, 0.05);
		nextDistanceL = randRange(0.15, 0.05);
	}else {
		nextDistanceR = randRange(0.05, 0.02);
		nextDistanceL = randRange(0.05, 0.02);
	}
	// pomer deleni
	double nextRatio, sideProb;
	switch (this->type) {
	case 2:
		nextRatio = randRange(0.45, 0.55);
		break;
	case 3:
		sideProb = 0.5;
		if (strongest) {
			if (directionType == 1) sideProb = sideProb - (0.2 + (0.1 * direction));
			if (directionType == 2) sideProb = sideProb + (0.2 + (0.1 * direction));
		}
		if (randRange(0.0, 1.0) > sideProb) nextRatio = randRange(0.01, 0.05);
		else nextRatio = randRange(0.95, 0.99);
		break;
	default:
		nextRatio = 1.0;
		break;
	}
	int nextTypeR, nextTypeL;
	double nextWidthR, nextWidthL, bifurcationProbR = 0.6, bifurcationProbL = 0.6;
	// sirka cevy v bode vetveni
	double widthIncR = sqrt(nextRatio);
	nextWidthR = (this->width * widthIncR) - (this->width * nextDistanceR / 10) - nextDistanceR / 20;
	// typ vetveni
	if (nextWidthR > 0.0 && nextWidthR < 0.01) nextWidthR = 0.0;
	if (nextWidthR < 0.0) {
		nextDistanceR = nextDistanceR * (1 - ((-1 * nextWidthR) / (this->width - nextWidthR)));
		nextWidthR = 0.0;
		nextTypeR = 1;
	} else if (randRange(0.0, 1.0) > bifurcationProb) {
		nextTypeR = 2;
		bifurcationProbR = 1.0;
	} else if (nextRatio < 0.25) {
		nextTypeR = 3;
		bifurcationProbR = 0.6;
	} else {
		nextTypeR = 3;
		bifurcationProbR = bifurcationProb - 0.25;
	}

	// sirka cevy v bode vetveni
	double widthIncL = sqrt(1 - nextRatio);
	nextWidthL = (this->width * widthIncL) - (this->width * nextDistanceL / 10) - nextDistanceL / 20;
	// typ vetveni
	if (nextWidthL > 0.0 && nextWidthL < 0.01) nextWidthR = 0.0;
	if (nextWidthL < 0.0) {
		nextDistanceL = nextDistanceL * (1 - ((-1 * nextWidthL) / (this->width - nextWidthL)));
		nextWidthL = 0.0;
		nextTypeL = 1;
	} else if (randRange(0.0, 1.0) > bifurcationProb) {
		nextTypeL = 2;
		bifurcationProbL = 1.0;
	} else if (1 - nextRatio < 0.25) {
		nextTypeL = 3;
		bifurcationProbL = 0.6;
	} else {
		nextTypeL = 3;
		bifurcationProbL = bifurcationProb - 0.25;
	}
	//TODO
	switch (this->type) {
	case 0:
		this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, true, 0, 0);
		this->nextPointLeft = nullptr;
		break;
	case 1:
	//default:
		this->nextPointRight = nullptr;
		this->nextPointLeft = nullptr;
		break;
	case 2:
		if (strongest && (directionType == 0 || directionType == 1))
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, true, 1, 0);
		else if (directionType == 3 || directionType == 4)
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, false, directionType, 0);
		else
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, false, 5, 0);

		if (strongest && (directionType == 0 || directionType == 2))
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, true, 2, 0);
		else if (directionType == 3 || directionType == 4)
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, false, directionType, 0);
		else
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, false, 5, 0);
		break;
	case 3:
		if (strongest && nextRatio > 0.5)
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, true, directionType, direction + 1);
		else if(nextRatio > 0.5 && (directionType == 3 || directionType == 4))
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, false, directionType, 0);
		else if (strongest && (directionType == 0 || directionType == 1))
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, false, 3, 0);
		else
			this->nextPointRight = new bifurcationPoint(nextTypeR, nextWidthR, nextDistanceR, widthIncR, this, bifurcationProbR, false, 5, 0);

		if (strongest && 1 - nextRatio > 0.5)
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, true, directionType, direction - 1);
		else if (1 - nextRatio > 0.5 && (directionType == 3 || directionType == 4))
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, false, directionType, 0);
		else if(strongest && (directionType == 0 || directionType == 2))
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, false, 4, 0);
		else
			this->nextPointLeft = new bifurcationPoint(nextTypeL, nextWidthL, nextDistanceL, widthIncL, this, bifurcationProbL, false, 5, 0);
		break;
	default:
		break;
	}
}


bifurcationPoint::~bifurcationPoint() {
	delete(this->nextPointRight);
	delete(this->nextPointLeft);
}
int bifurcationPoint::getType() {
	return this->type;
}
double bifurcationPoint::getWidth() {
	return this->width;
}
double bifurcationPoint::getPrevDistance() {
	return this->prevDistance;
}
double bifurcationPoint::getPositionX() {
	return this->positionX;
}
double bifurcationPoint::getPositionY() {
	return this->positionY;
}
int bifurcationPoint::getDirectionAngle() {
	return this->directionAngle;
}
int bifurcationPoint::getDirectionType() {
	return this->directionType;
}
double bifurcationPoint::getWidthRatio() {
	return this->widthRatio;
}
double bifurcationPoint::getCurveWeight1() {
	return this->curveWeight1;
}
double bifurcationPoint::getCurveWeight2() {
	return this->curveWeight2;
}
int bifurcationPoint::getCurveEndDeviation() {
	return this->curveEndDeviation;
}
bifurcationPoint* bifurcationPoint::getNextPointRight() {
	return this->nextPointRight;
}
bifurcationPoint* bifurcationPoint::getNextPointLeft() {
	return this->nextPointLeft;
}
bifurcationPoint* bifurcationPoint::getPrevPoint() {
	return this->prevPoint;
}
void bifurcationPoint::setType(int type) {
	this->type = type;
}
void bifurcationPoint::setWidth(double width) {
	this->width = width;
}
void bifurcationPoint::setDirectionType(int directionType)
{
	this->directionType = directionType;
}
void bifurcationPoint::setPositionX(double positionX) {
	this->positionX = positionX;
}
void bifurcationPoint::setPositionY(double positionY) {
	this->positionY = positionY;
}
void bifurcationPoint::setDirectionAngle(int directionAngle) {
	this->directionAngle = directionAngle;
}
void bifurcationPoint::setNextPointRight(bifurcationPoint *point) {
	this->nextPointRight = point;
}
void bifurcationPoint::setNextPointLeft(bifurcationPoint *point) {
	this->nextPointLeft = point;
}