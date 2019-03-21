#include "vascularityLayer.h"
#include "adapter.h"
#include "createVessel.h"

int tmpX(double x) {
	return (int)((x + 1.0) * 1000) / 2;
}

int tmpY(double y) {
	return (int)((y + 1.0) * 800) / 2;
}

int count = 0;

vascularityLayer::vascularityLayer(int width, int height, double depth, double layerSize, double textureSize) {
	this->windowWidth = width;
	this->windowHeight = height;
	this->depth = depth;
	this->recalculateAspectRatio();
	this->layerSize = layerSize;
	this->textureSize = textureSize;
	this->createNewTexureImage(false);
	this->textureImageTmp = nullptr;
	this->textureChange = true;

	this->textureShiftX = 0.0;
	this->textureShiftY = 0.0;
	this->textureRotation = 0.0;

	this->arteryBaseColorR = 160;
	this->arteryBaseColorG = 15;
	this->arteryBaseColorB = 15;
	this->veinBaseColorR = 150;
	this->veinBaseColorG = 5;
	this->veinBaseColorB = 15;

	// posun hranice kvadrantu
	double xq1 = randRange(-0.1, 0.1);
	double yq1 = randRange(-0.05, 0.05);
	double xq2 = randRange(-0.1, 0.1);
	double yq2 = randRange(-0.05, 0.05);

	//// vytvoreni tepen a zil pro vsechny kvadranty
	//double r = randRange(0.0, 1.0);
	//this->arteryQ1 = new vessel(0, 1, 0.015, 0.42, 0.0, xq1, yq2, (r > 0.5) ? 0 : 1);
	//this->veinQ1 = new vessel(1, 1, 0.015, 0.42, 0.0, xq1, yq2, (r > 0.5) ? 1 : 0);
	//
	//r = randRange(0.0, 1.0);
	//this->arteryQ2 = new vessel(0, 2, 0.015, 0.42, 0.0, xq1, yq1, (r > 0.5) ? 0 : 1);
	//this->veinQ2 = new vessel(1, 2, 0.015, 0.42, 0.0, xq1, yq1, (r > 0.5) ? 1 : 0);
	//
	//r = randRange(0.0, 1.0);
	//this->arteryQ3 = new vessel(0, 3, 0.015, 0.42, 0.0, xq2, yq1, (r > 0.5) ? 0 : 1);
	//this->veinQ3 = new vessel(1, 3, 0.015, 0.42, 0.0, xq2, yq1, (r > 0.5) ? 1 : 0);
	//
	//r = randRange(0.0, 1.0);
	// this->arteryQ4 = new vessel(0, 4, 0.015, 0.42, 0.0, xq2, yq2, (r > 0.5) ? 0 : 1);
	//this->veinQ4 = new vessel(1, 4, 0.015, 0.42, 0.0, xq2, yq2, (r > 0.5) ? 1 : 0);


	//TODO test 
	//this->arteryQ1 = new vessel(0, 1, 0.015, 0.42, 0.0, xq1, yq2);
	//this->arteryQ2 = new vessel(0, 2, 0.015, 0.42, 0.0, xq1, yq1);
	this->arteryQ3 = new vessel(0, 3, 0.015, 0.42, 0.0, xq2, yq1);
	//this->arteryQ4 = new vessel(0, 4, 0.015, 0.42, 0.0, xq2, yq2);

	createVessel* vesselCreator;
	//arteryQ1->setStartPoint(vesselCreator->createBranch(inferior, nasal, xq1, yq2));
	//arteryQ2->setStartPoint(vesselCreator->createBranch(inferior, macular, xq1, yq1));
	arteryQ3->setStartPoint(vesselCreator->createBranch(superior, macular, xq2, yq1));
	//arteryQ4->setStartPoint(vesselCreator->createBranch(superior, nasal, xq2, yq2));


	//TODO test 
	std::vector<bifurcationPoint*> queue;
	queue.push_back(this->arteryQ3->getStartPoint());
	bifurcationPoint* Root;
	while (!queue.empty())
	{
		Root = queue.front();
		queue.erase(queue.begin());
		count++;
	//	std::cout << "#########################################" << std::endl;
	//	std::cout << "pozice bodu X: " << Root->getPositionX() << std::endl;
	//	std::cout << "pozice bodu X: " << tmpX(Root->getPositionX()) << std::endl;

	//	std::cout << "pozice bodu Y: " << Root->getPositionY() << std::endl;
	//	std::cout << "pozice bodu Y: " << tmpY(Root->getPositionY()) << std::endl;

	//	std::cout << "PrevDistance: " << Root->getPrevDistance() << std::endl;
	//	std::cout << "Width: " << Root->getWidth() << std::endl;
	//	std::cout << "DirectionAngle: " << Root->getDirectionAngle() << std::endl;
	//	std::cout << "WidthRatio: " << Root->getWidthRatio() << std::endl;
	//	std::cout << "CurveEndDeviation: " << Root->getCurveEndDeviation() << std::endl;
	//	std::cout << "DirectionType: " << Root->getDirectionType() << std::endl;
	//	std::cout << "Type: " << Root->getType() << std::endl;


		if(Root->getNextPointRight() != NULL)
			queue.push_back(Root->getNextPointRight());
		if (Root->getNextPointLeft() != NULL)
			queue.push_back(Root->getNextPointLeft());
	}
	std::cout << "pocet bodu vetveni ve stromu: "<<count << std::endl;
	// end |TODO

	this->initRand();
}

vascularityLayer::~vascularityLayer() {
	//delete(this->arteryQ1);
	//delete(this->arteryQ2);
	//delete(this->arteryQ3);
	//delete(this->arteryQ4);
	//delete(this->veinQ1);
	//delete(this->veinQ2);
	//delete(this->veinQ3);
	//delete(this->veinQ4);
}

void vascularityLayer::initRand() {
	this->arteryBaseColorR = this->arteryBaseColorR * randRange(0.99, 1.01);
	this->arteryBaseColorG = this->arteryBaseColorG * randRange(0.99, 1.01);
	this->arteryBaseColorB = this->arteryBaseColorB * randRange(0.99, 1.01);
	this->veinBaseColorR = this->veinBaseColorR * randRange(0.99, 1.01);
	this->veinBaseColorG = this->veinBaseColorG * randRange(0.99, 1.01);
	this->veinBaseColorB = this->veinBaseColorB * randRange(0.99, 1.01);
}

void vascularityLayer::makeTextureImage() {
	// ziskani transformacnich udaju z nizsich vrstev
	std::vector<double> shiftX, shiftY, rotation;
	this->getTextureShiftX(&shiftX);
	this->getTextureShiftY(&shiftY);
	this->getTextureRotation(&rotation);
	int transformSize = shiftX.size();
	double *transformation = new double[3 * shiftX.size()];
	for (int i = 0; i < transformSize; i++) { // nefunguje moc kombinace posuvu a rotaci ve vice vrstvach
		transformation[0 * transformSize + i] = -shiftX.at(i);
		transformation[1 * transformSize + i] = -shiftY.at(i);
		transformation[2 * transformSize + i] = -rotation.at(i);
	}
	// pruhledna textura
	for (int y = 0; y < this->textureHeight; y++) {
		for (int x = 0; x < this->textureWidth; x++) {
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)0;
			this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)0;
		}
	}
	//TODO - Zde jsem si to zakomentoval
	// vykresleni tepen do textury
	//this->drawVessel(this->arteryQ1, transformation, transformSize);	
	//this->drawVessel(this->arteryQ2, transformation, transformSize);
	this->drawVessel(this->arteryQ3, transformation, transformSize);
	//this->drawVessel(this->arteryQ4, transformation, transformSize);
	// docasna textura pro vykresleni zil
	this->createNewTexureImageTmp(false);
	// pruhledna docasna textura
	for (int y = 0; y < this->textureHeight; y++) {
		for (int x = 0; x < this->textureWidth; x++) {
			this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)0;
			this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)0;
			this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)0;
			this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)0;
		}
	}
	// vykresleni zil do docasne textury 
	//TODO - Zde jsem si to zakomentoval
	//this->drawVessel(this->veinQ1, transformation, transformSize);
	//this->drawVessel(this->veinQ2, transformation, transformSize);
	//this->drawVessel(this->veinQ3, transformation, transformSize);
	//this->drawVessel(this->veinQ4, transformation, transformSize);

	// vykresleni zil do textury s tepnami (zily budou lezet nad tepnami)
	for (int y = 0; y < this->textureHeight; y++) {
		for (int x = 0; x < this->textureWidth; x++) {
			// pokud je na danych souradnicich tepna i zila zkombinuji se
			if (this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] != 0 && this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] != 0) {
				double w1 = (double)this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] / (this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] + this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3]);
				double w2 = (double)this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] / (this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] + this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3]);
				double w = this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] / 255.0;
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)((1 - w) * (w1 * this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] + 
					w2 * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0]) + w * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0]);
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)((1 - w) * (w1 * (int)this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] +
					w2 * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1]) + w * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1]);
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)((1 - w) * (w1 * (int)this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] +
					w2 * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2]) + w * this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2]);
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] > this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3]) ? 
					this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] : this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3];
			}
			// pokud na danych souradnicich pouze zila vykresli se do textury s tepnami
			else if (this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] != 0) {
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0];
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1];
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2];
				this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3];
			}
			// pokud je na danych souradnicich tepna nebo nic, textura se nemeni
		}
	}

	this->textureChange = false;
	delete(transformation);
	delete(this->textureImageTmp); // smazani docasne texury
}

void vascularityLayer::drawVessel(vessel *v, double *transformation, int transformSize) {
	std::vector<bifurcationPoint*> points;
	points.push_back(v->getStartPoint()->getNextPointRight());
	// vybere nejvetsi z podstromu (s nejvetsi pocatecni sirkou cevy) a zahaji jeho vykreslovani
	while (points.size() != 0) {
		bifurcationPoint *point = points.back();
		points.pop_back();
		this->drawVesselTree(point, &points, v->getType(), v->getWidth(), transformation, transformSize, point->getDirectionAngle(), true);
		std::sort(points.begin(), points.end(), [](bifurcationPoint *lhs, bifurcationPoint *rhs) { return lhs->getWidth() < rhs->getWidth(); });
	}
}

void vascularityLayer::drawVesselTree(bifurcationPoint *point, std::vector<bifurcationPoint*> *points, int type, double width, double *transformation, int transformSize, int prevCurveAngle, bool first) {
		bifurcationPoint *nextPointRight = point->getNextPointRight();
		bifurcationPoint *nextPointLeft = point->getNextPointLeft();

		// necha vykreslit krivku od predchoziho bodu do aktualniho
		prevCurveAngle = this->drawCurve(point, type, width, transformation, transformSize, prevCurveAngle, first);

		// aktualni bod ma oba potomky
		if (nextPointRight != nullptr && nextPointLeft != nullptr) {
			// necha vykreslit ten s vetsi sirkou cevy, mensi si ulozi jako koren noveho podstromu
			if(nextPointRight->getWidth() > nextPointLeft->getWidth()) {
				points->push_back(nextPointLeft);
				drawVesselTree(nextPointRight, points, type, width, transformation, transformSize, prevCurveAngle, false);
			}
			else {
				points->push_back(nextPointRight);
				drawVesselTree(nextPointLeft, points, type, width, transformation, transformSize, prevCurveAngle, false);
			}
		}
		// pokud ma jenom jednoho z potomku necha ho vykreslit
		else if (nextPointRight != nullptr)
			drawVesselTree(nextPointRight, points, type, width, transformation, transformSize, prevCurveAngle, false);
		else if (nextPointLeft != nullptr)
			drawVesselTree(nextPointLeft, points, type, width, transformation, transformSize, prevCurveAngle, false);
		// pokud se jedna o koncovy bod, vykreslovani tohoto podstromu konci a vraci se do drawVessel vykreslovat dalsi podstromy
}

int vascularityLayer::drawCurve(bifurcationPoint *point, int type, double width, double *transformation, int transformSize, int prevCurveAngle, bool first) {
	bifurcationPoint *prevPoint = point->getPrevPoint();
	bifurcationPoint *nextPointRight = point->getNextPointRight();
	bifurcationPoint *nextPointLeft = point->getNextPointLeft();

	// vypocet jakym smerem bude kontrolni bod Bezierovy krivky od aktualniho bodu
	double prevDA = point->getDirectionAngle(); // uhel znacici smer od predchoziho bodu k aktualnimu bodu
	double actDA; // ziskam uhel, znacici smer od aktualniho bodu k nasledujicimu bodu
	if (nextPointRight != nullptr && nextPointLeft != nullptr)
		if (nextPointRight->getWidth() > nextPointLeft->getWidth()) actDA = nextPointRight->getDirectionAngle();
		else actDA = nextPointLeft->getDirectionAngle();
	else if (nextPointRight != nullptr) actDA = nextPointRight->getDirectionAngle();
	else if (nextPointLeft != nullptr) actDA = nextPointLeft->getDirectionAngle();
	else actDA = (int)(prevDA + 360 + point->getCurveEndDeviation()) % 360; // aktualni je koncovy = jakoby byl nasledujici bod v primce + odchylka

	// upravim uhly pro vypocet stredniho uhlu mezi nimi
	if (prevDA <= 90 && actDA >= 270 - 90 + (90 - prevDA)) prevDA = prevDA + 360;
	else if (prevDA <= 90 + 90 - (360 - actDA) && actDA >= 270) prevDA = prevDA + 360;
	else if (actDA <= 90 && prevDA >= 270 - 90 + (90 - actDA)) actDA = actDA + 360;
	else if (actDA <= 90 + 90 - (360 - prevDA) && prevDA >= 270) actDA = actDA + 360;
	int newCurveAngle = actDA - (actDA - prevDA) / 2; // vypocet stredniho uhlu
	newCurveAngle = ((newCurveAngle % 360) + 180) % 360; // kontroli bod smerem k predchozimu bodu

	// vypocet pozic kontrolnich bodu Bezierovy krivky
	double dist = point->getPrevDistance() / 4;
	double prevX = prevPoint->getPositionX(), prevY = prevPoint->getPositionY();
	double xp1 = prevX + (prevPoint->getCurveWeight2() * dist * cos((prevCurveAngle * PI) / 180)), yp1 = prevY + (dist * sin((prevCurveAngle * PI) / 180));
	double x = point->getPositionX(), y = point->getPositionY();
	double xp2 = x + (point->getCurveWeight1() * dist * cos((newCurveAngle * PI) / 180)), yp2 = y + (dist * sin((newCurveAngle * PI) / 180));
	
	// transformace souradnic
	for (int i = 0; i < transformSize; i++) {
		prevX = prevX + transformation[0 * transformSize + i];
		prevY = prevY + transformation[1 * transformSize + i];
		this->rotationCoord(prevX, prevY, &prevX, &prevY, transformation[2 * transformSize + i]);

		xp1 = xp1 + transformation[0 * transformSize + i];
		yp1 = yp1 + transformation[1 * transformSize + i];
		this->rotationCoord(xp1, yp1, &xp1, &yp1, transformation[2 * transformSize + i]);

		xp2 = xp2 + transformation[0 * transformSize + i];
		yp2 = yp2 + transformation[1 * transformSize + i];
		this->rotationCoord(xp2, yp2, &xp2, &yp2, transformation[2 * transformSize + i]);

		x = x + transformation[0 * transformSize + i];
		y = y + transformation[1 * transformSize + i];
		this->rotationCoord(x, y, &x, &y, transformation[2 * transformSize + i]);
	}
	// Vykresleni krivky mezi aktualnim a predchozim bodem
	if(first)
		cubicBezierCurve(prevX, prevY, xp1, yp1, xp2, yp2, x, y, width, prevPoint->getWidth() * point->getWidthRatio(), point->getWidth(), type);
	else
		cubicBezierCurve(prevX, prevY, xp1, yp1, xp2, yp2, x, y, width, prevPoint->getWidth(), point->getWidth(), type);
	
	newCurveAngle = (newCurveAngle + 180) % 360; // smer kontrolniho bodu aktualniho bodu pro dalsi cast krivky
	return newCurveAngle;
}

void vascularityLayer::cubicBezierCurve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double width, double widthR1, double widthR2, int type) {
	// inspirovano:
	// http://stackoverflow.com/questions/785097/how-do-i-implement-a-b%C3%A9zier-curve-in-c/11435243#11435243
	
	double px = x1, py = y1;
	// velikost kroku v zavislosti na vzdalenosti bodu vetveni
	double step = 1.0 / sqrt(pow(abs(this->convertCoordXback(x1) - this->convertCoordXback(x4)), 2) + pow(abs(this->convertCoordYback(y1) - this->convertCoordYback(y4)), 2)) * 2;
	double wd = widthR1 - widthR2;
	double i;
	// vypocet bodu krivky
	for (i = step; i < 1; i += step) {
		double xa = getPt(x1, x2, i);
		double ya = getPt(y1, y2, i);
		double xb = getPt(x2, x3, i);
		double yb = getPt(y2, y3, i);
		double xc = getPt(x3, x4, i);
		double yc = getPt(y3, y4, i);

		double xm = getPt(xa, xb, i);
		double ym = getPt(ya, yb, i);
		double xn = getPt(xb, xc, i);
		double yn = getPt(yb, yc, i);

		double x = getPt(xm, xn, i);
		double y = getPt(ym, yn, i);
		if (px != x || py != y) {
			// rovnice primky
			double A = -((py - y) / (px - x));
			double B = 1;
			double C = -(py - (-A * px));
			// vykresleni usecky
			this->drawLine(px, py, x, y, width, widthR1 - (i - step) * wd, (widthR1 - i * wd), type, A, B, C);
		}
		px = x; py = y;
	}
	if (px != x4 || py != y4) {
		double A = -((py - y4) / (px - x4));
		double B = 1;
		double C = -(py - (-A * px));
		this->drawLine(px, py, x4, y4, width, widthR1 - (i - step) * wd, widthR2, type, A, B, C);
	}
}

double vascularityLayer::getPt(double n1, double n2, double perc)
{
	double diff = n2 - n1;
	return n1 + (diff * perc);
}

void vascularityLayer::drawLine(double x1, double y1, double x2, double y2, double width, double widthR1, double widthR2, int type, double A, double B, double C) {
	// inspirovano:
	// http://tech-algorithm.com/articles/drawing-line-using-bresenham-algorithm/
	if (this->convertCoordXback(x1) == this->convertCoordXback(x2) && this->convertCoordYback(y1) == this->convertCoordYback(y2)) return; // jenom bod
	int x = this->convertCoordXback(x1);
	int y = this->convertCoordYback(y1);
	int w = this->convertCoordXback(x2) - x;
	int h = this->convertCoordYback(y2) - y;
	
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
	double wk = (widthR1 - widthR2) / longest;
	int numerator = longest >> 1;

	int r, g, b, a;
	if (type == 0) { r = this->arteryBaseColorR; g = this->arteryBaseColorG; b = this->arteryBaseColorB; }
	else { r = this->veinBaseColorR; g = this->veinBaseColorG; b = this->veinBaseColorB; }

	for (int i = 0; i <= longest; i++) {
		if (y < 0 || x < 0 || y >= this->textureWidth || x >= this->textureWidth) break;
		else {
			double xn, yn, xnprev = DBL_MAX, ynprev = DBL_MAX;
			// body ridici usecky mezi dvema body krivky
			if (widthR1 >= 0) {
				int directionAngle = ((int)(atan2(y2 - y1, x2 - x1) * 180 / PI) + 360) % 360; // smer usecky
				double hw = 0.5 * width * (widthR1 - (i * wk)) + 0.003; // polovina sirky cevy
				// vykresli pulkruh okolo aktualniho bodu ridici usecky ve smeru ridici usecky
				for (int angle = directionAngle + 360 - 90; angle < directionAngle + 360 + 91; angle = angle + 1) {
					xn = this->convertCoordX(x) + (1.1 * hw * cos((angle * PI) / 180));
					yn = this->convertCoordY(y) + (1.1 * hw * sin((angle * PI) / 180));

					if (xn != xnprev || yn != ynprev) {
						this->drawLine(this->convertCoordX(x), this->convertCoordY(y), xn, yn, width, -hw, (widthR1 - (i * wk)), type, A, B, C);
					}
					xnprev = xn, ynprev = yn;
				}
			}
			// body vykreslujici usecky 
			else { // 
				double dist = abs(A * this->convertCoordX(x) + B * this->convertCoordY(y) + C) / sqrt(A * A + B * B); // vzdalenost bodu od ridici usecky (stredu cevy)
				this->getColorIntensity(x, y, &r, &g, &b, &a, dist / -widthR1, widthR2); // barva bodu
				if (type == 0){ // vykresleni do textury tepny
					if (this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] < (GLubyte)r)
						this->textureImage[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)r;
					if (this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] < (GLubyte)g)
						this->textureImage[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)g;
					if (this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] < (GLubyte)b)
						this->textureImage[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)b;
					if (this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] < (GLubyte)a)
						this->textureImage[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)a;
				}
				else { // vykresleni do textury zily
					if (this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0] < (GLubyte)r)
						this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 0] = (GLubyte)r;
					if (this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1] < (GLubyte)g)
						this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 1] = (GLubyte)g;
					if (this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2] < (GLubyte)b)
						this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 2] = (GLubyte)b;
					if (this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] < (GLubyte)a)
						this->textureImageTmp[y * this->textureWidth * 4 + x * 4 + 3] = (GLubyte)a;
				}
			}
		}
		numerator += shortest;
		if (!(numerator < longest)) {
			numerator -= longest;
			x += dx1;
			y += dy1;
		}
		else {
			x += dx2;
			y += dy2;
		}
	}
}

void vascularityLayer::getColorIntensity(int x, int y, int *r, int *g, int *b, int *a, double actDist, double width) {
	if (actDist > 1.0) actDist = 1.0;
	actDist = 1 - actDist;
	// vypocet barvy
	*r = 0.9 * *r + 0.1 * *r * sqrt(actDist);
	*g = 0.9 * *g + 0.1 * *g * sqrt(actDist);
	*b = 0.9 * *b + 0.1 * *b * sqrt(actDist);
	// vypocet pruhlednosti, pruhlednejsi u okraje cevy, uzsi cevy take pruhlednejsi
	double q, qa, qb;
	qa = pow(width, 0.2);
	qb = sqrt(width);
	q = pow(actDist, 1.35 - qb) * pow(width, 0.2);
	double ap;
	if (actDist > 1.5 - qb) ap = 255 * qa;
	else if (actDist > 1.4 - qb) ap = 205 + 50 * q;
	else if (actDist > 1.3 - qb) ap = 155 + 100 * q;
	else if (actDist > 1.2 - qb) ap = 105 + 150 * q;
	else if (actDist > 1.1 - qb) ap = 55 + 200 * q;
	else ap = 255 * q;
	*a = ap;
}


