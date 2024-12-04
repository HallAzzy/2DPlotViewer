#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

double					// исходные значения параметров X0, Y0, px, py
X0 = 200,
Y0 = 200,
px = 50,
py = 100;

double Parabola(double x)
{
	return x * x - 2;
}

double Sinusoid(double x)
{
	return sin(x);
}

double Cardioid(double theta)
{
	return 1 + cos(theta);
}

double Astroid(double theta)
{
	return cos(5 * theta) + cos(10 * theta);
}

double Infinity(double theta)
{
	return cos(2 * theta);
}

double Spiral(double theta)
{
	return 2 * theta;
}