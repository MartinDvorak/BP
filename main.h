#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

const double PI = 3.14159265358979323846;
const double E = 2.71828182845904523536;

// nahodna hodnota ze zadaneho intervalu s rovnomernym rozlozenim pravdepodobnosti
double randRange(double min, double max);
// vrati pro zadanou hodnotu hodnotu pravdepodobnosti normalniho rozlozeni pro zadany interval
double normalDistributionProb(double value, double min, double max);
// nahodna hodnota ze zadaneho intervalu s normalnim rozlozenim pravdepodobnosti
double randRangeND(double min, double max);
// nacteni konfiguracniho souboru
void loadConfigFile(int *numberImages, int *width, int *height, std::string *path, double *layerSize, double *textureSize, int *eye, int *markQuadrant, int *markType);

#endif