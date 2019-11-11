#pragma once
#include <math.h>
//Vector 2

struct V2
{
	float x;
	float y;
};

V2 operator+(V2 val1, V2 val2);

V2 operator-(V2 val1);

V2 operator-(V2 val1, V2 val2);

V2 operator*(float val1, V2 val2);

V2 operator*(V2 val1, float val2);

V2 &operator*=(V2 &val1, float val2);

V2 &operator+=(V2 &val1, V2 val2);

float DotProduct(V2 val1, V2 val2);

float MagnitudeSqrd(V2 val);

float Magnitude(V2 val);
