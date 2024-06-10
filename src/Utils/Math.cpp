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

int Math::floor(double value)
{
    return (int)value;
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

int Math::dist(int a, int b)
{
    return abs(a - b);
}

int Math::dist_vec(vec2f a, vec2f b)
{
    return dist(a.x, b.x) + dist(a.y, b.y);
}

int Math::mid(int a, int b)
{
    return (a-b) / 2; 
}

vec2f Math::mid_vec(vec2f a, vec2f b)
{
    b.x = b.x/2;
    b.y = b.y/2;
    return vec2f(mid(a.x, b.x), mid(a.y, b.y));
}
