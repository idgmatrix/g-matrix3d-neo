/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "vector4.h"

Vector4 operator*(const float scalar, const Vector4 &rhs)
{
	return Vector4(scalar * rhs.x, scalar * rhs.y, scalar * rhs.z, scalar * rhs.w);
}

void Vector4::PerspectiveDivide()
{
	rhw = 1 / w;
	
	x *= rhw;
	y *= rhw;
	z *= rhw;
}
