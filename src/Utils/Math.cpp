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

int Math::round(double value)
{
    return (int)(value + 0.5);
}

double Math::sign(double value)
{
    if (value > 0)
        return 1;
    else if (value < 0)
        return -1;
    else
        return 0;
}

double Math::abs(double value)
{
    if (value < 0)
        return -value;
    else
        return value;
}

double Math::pow(double base, double exponent)
{
    double result = 1;
    for (int i = 0; i < exponent; i++)
        result *= base;
    return result;
}

double Math::sqrt(double value)
{
    double result = 0;
    for (int i = 0; i < value; i++)
    {
        if (i * i == value)
        {
            result = i;
            break;
        }
    }
    return result;
}