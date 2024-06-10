#pragma once

#include "Common.hpp"

namespace Math {
	double lerp(double a, double b, double t);

	int clamp(int min, int value, int max);
	int round(double value);
	int floor(double value);
	double sign(double value);
	double abs(double value);
	double pow(double base, double exponent);
	double sqrt(double value);

	int dist(int a, int b);
	int dist_vec(vec2f a, vec2f b);

	int mid(int a, int b);
	vec2f mid_vec(vec2f a, vec2f b);
};