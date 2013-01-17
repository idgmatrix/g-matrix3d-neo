/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "vector3.h"

Vector3 operator*(const float scalar, const Vector3 &rhs)
{
	return Vector3(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z);
}

void Vector3::Normalize()
{
	float invlength = 1 / sqrtf(x * x + y * y + z * z);

	x *= invlength;
	y *= invlength;
	z *= invlength;
}
