
#ifndef GENERATOR_H
#define GENERATOR_H

class generator {
private:
	static generator* instance;
	generator();
public:
	static generator* getGenerator();
	double getNextUniformValue();
	double getNextValue(double minValue, double maxValue);
};

#endif