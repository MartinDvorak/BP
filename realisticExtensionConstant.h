#ifndef REALISTIC_EXTENSION_CONSTANT_H
#define REALISTIC_EXTENSION_CONSTANT_H

#define Y_COORDINATE_MAX_CONVERSION 75.0
#define Y_COORDINATE_MIN_CONVERSION -75.0

#define X_COORDINATE_MAX_CONVERSION 100.0
#define X_COORDINATE_MIN_CONVERSION -50.0
#define X_COORDINATE_SHIFT_CONVERSION 0.08

//##########################################
#define Y_COORDINATE_MAX 60.0
#define Y_COORDINATE_MIN -1.0

#define X_COORDINATE_MAX_MACULAR 80.0
#define X_COORDINATE_MAX_NASAL 30.0

#define X_COORDINATE_MIN -10.0

#define X_MACULAR_POINT 30.0
#define Y_MACULAR_POINT 0.0
#define RADIUS_MACULAR_POINT 12.0

const double RE_PI = 3.14159265358979323846;
const double RE_E  = 2.71828182845904523536;

enum inferiorOrSuperior { inferior, superior };
enum nasalOrMacular { nasal, macular };

#endif