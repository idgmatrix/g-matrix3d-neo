/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "vector2.h"

Vector2 operator*(const float scalar, const Vector2 &rhs)
{
	return Vector2(scalar * rhs.x, scalar * rhs.y);
}

void Vector2::Normalize()
{
	float invlength = 1 / sqrtf(x * x + y * y);

	x *= invlength;
	y *= invlength;
}
