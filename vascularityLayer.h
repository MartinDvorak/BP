#ifndef VASCULARITYLAYER_H
#define VASCULARITYLAYER_H

#include "layer.h"
#include "vessel.h"

// vrstva obsahujici cevni reciste
class vascularityLayer : public layer
{
private:
	vessel *arteryQ1;
	vessel *veinQ1;
	vessel *arteryQ2;
	vessel *veinQ2;
	vessel *arteryQ3;
	vessel *veinQ3;
	vessel *arteryQ4;
	vessel *veinQ4;

	// Slozky zakladni barvy tepen a zil
	int arteryBaseColorR;
	int arteryBaseColorG;
	int arteryBaseColorB;
	int veinBaseColorR;
	int veinBaseColorG;
	int veinBaseColorB;

	void initRand(); // castecna randomizace nekterych vlastnosti
	void makeTextureImage();

	// vybira nejvetsi z nevykreslenych podstromu (s nejvetsi pocatecni sirkou cevy) a nechava je vykreslit
	void drawVessel(vessel *v, double *transformation, int transformSize);

	// vykresluje krivku mezi silnejsimi cevami v podstromu, slabsi vetveni uklada jako koreny novych podstromu
	void drawVesselTree(bifurcationPoint *point, std::vector<bifurcationPoint*> *points, int type, double width, double *transformation, int transformSize, int prevCurveAngle, bool first);

	// vypocita kontrolni body Bezierovy krivky mezi vstupnim bodem a jeho predchozim bodem a necha ji vzkreslit
	int drawCurve(bifurcationPoint *point, int type, double width, double *transformation, int transformSize, int prevCurveAngle, bool first);

	// vypocita body Bezierovy krivky, mezi nimi necha vykreslit usecky 
	void cubicBezierCurve(double x1, double y1, double x2, double y2, double x3, double y3, double x4, double y4, double width, double widthR1, double widthR2, int type);
	// interpolacni funkce pro vzpocet Bezierovz krivky
	double getPt(double n1, double n2, double perc);
	
	// vykresluje na zadane usecce  cevu dane sirky
	void drawLine(double x1, double y1, double x2, double y2, double width, double widthR1, double widthR2, int type, double A, double B, double C);
	
	// vypocet barvy cevy v zavislosti na vzydalenosti od stredu cevy
	void getColorIntensity(int x, int y, int *r, int *g, int *b, int *a, double actDist, double width);
public:
	vascularityLayer(int width, int height, double depth, double layerSize, double textureSize);
	~vascularityLayer();
};

#endif
