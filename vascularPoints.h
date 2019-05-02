#ifndef VASCULAR_POINTS_H
#define VASCULAR_POINTS_H

class vascularPoint {
	double coordX;
	double coordY;
	double bifurcationRatio;
	
	bool branchT;
	vascularPoint* leftPointer;
	vascularPoint* rightPointer;
public:
	vascularPoint(double cordX,double cordY, double bifurcationRatio);
	vascularPoint* getLeftPoint();
	vascularPoint* getRightPoint();
	double getCordX();
	double getCordY();
	double getBifurcationRatio();
	bool getBranchT();
	void setLeftPoint(vascularPoint* leftPointer);
	void setRightPoint(vascularPoint* rightPointer);
	void setBranchT(bool isBranchT);
};

#endif