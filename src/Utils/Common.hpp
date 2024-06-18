#ifndef __COMMON_HPP__
#define __COMMON_HPP__
#pragma once
#include <iostream>
//enums
enum State{IDLE,UPDATE,UNAVALIABLE};

//important stuff to entities
struct vec2f
{
	float x, y;
	vec2f()
		:x(0.0f), y(0.0f)
	{}

	vec2f(float p_x, float p_y)
		:x(p_x), y(p_y)
	{}

	void print()
	{
		std::cout << x << ", " << y << std::endl;
	}
	bool operator==(const vec2f& other) const {
        return (x == other.x) && (y == other.y);
    }

    bool operator!=(const vec2f& other) const {
        return !(*this == other);
    }

	vec2f operator+(const vec2f& other) const {
		return vec2f(x + other.x, y + other.y);
	}
};
struct vec3f {
	float x, y, z;
	vec3f()
		:x(0.0f), y(0.0f), z(0.0f)
	{}
    vec3f(float p_x, float p_y, float p_z)
		:x(p_x), y(p_y), z(p_z)
	{}
	void print()
	{
		std::cout << x << ", " << y << ", " << z << std::endl;
	}
};
/// <summary>
/// Structure that will have position and radius
/// </summary>
struct Area {
	float x, y, w, h;
	Area(float pX, float pY, float pW, float pH):x(pX), y(pY), w(pW), h(pH) {}

	bool intersects(const Area pArea) {
		return (x < pArea.x + pArea.w) &&
			(pArea.x < x + w) &&
			(y < pArea.y + pArea.h) &&
			(pArea.y < y + h);
	}
};

struct Stat{
	float value;
	float max;
	Stat():value(0), max(0){}
	Stat(float _value, float _max):value(_value), max(_max){}

	void add(float _value){
		value += _value;
		if(value > max){
			value = max;
		}
	}
};

struct Life : public Stat{
	Life():Stat(){}
	Life(float _value, float _max):Stat(_value, _max){}

	bool is_alive(){
		return value > 0;
	}
};
#endif