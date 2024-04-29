#include "Math.hpp"

double Math::lerp(double a, double b, double t)
{
    if (t <= 0.5)
        return a + (b - a) * t;
    else
        return b - (b - a) * (1.0 - t);
}
