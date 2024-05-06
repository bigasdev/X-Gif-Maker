#pragma once

namespace Math {
	double lerp(double a, double b, double t);

	int clamp(int min, int value, int max);
	int round(double value);
	double sign(double value);
	double abs(double value);
	double pow(double base, double exponent);
	double sqrt(double value);
};