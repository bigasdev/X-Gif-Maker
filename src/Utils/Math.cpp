#include "Math.hpp"

double Math::lerp(double a, double b, double t)
{
    if (t <= 0.5)
        return a + (b - a) * t;
    else
        return b - (b - a) * (1.0 - t);
}

int Math::clamp(int min, int value, int max)
{
    if (value < min)
        return min;
    else if (value > max)
        return max;
    else
        return value;
}
