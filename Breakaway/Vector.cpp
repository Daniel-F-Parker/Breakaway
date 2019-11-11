#pragma once
#include "Vector.h"
#include <math.h>
//Vector 2

V2 operator+(V2 val1, V2 val2)
{
	V2 result;
	result.x = val1.x + val2.x;
	result.y = val1.y + val2.y;

	return result;
}
V2 operator-(V2 val1)
{
	V2 result;
	result.x = -val1.x;
	result.y = -val1.y;

	return result;
}
V2 operator-(V2 val1, V2 val2)
{
	V2 result;
	result.x = val1.x - val2.x;
	result.y = val1.y - val2.y;

	return result;
}
V2 operator*(float val1, V2 val2)
{
	V2 result;
	result.x = val1 * val2.x;
	result.y = val1 * val2.y;

	return result;
}
V2 operator*(V2 val1, float val2)
{
	V2 result;
	result.x = val1.x * val2;
	result.y = val1.y * val2;

	return result;
}

V2 &operator*=(V2 &val1, float val2)
{
	val1 = val2 * val1;

	return val1;
}
V2 &operator+=(V2 &val1, V2 val2)
{
	val1 = val2 + val1;

	return val1;
}

float DotProduct(V2 val1, V2 val2)
{
	float result = val1.x*val2.x + val1.y*val2.y;
	return result;
}

float MagnitudeSqrd(V2 val)
{
	float result = DotProduct(val, val);
	return result;
}

float Magnitude(V2 val)
{
	float result = sqrtf(MagnitudeSqrd(val));
	return result;
}
