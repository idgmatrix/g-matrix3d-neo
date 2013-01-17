/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "matrix3.h"

Matrix3 operator*(const float scalar, const Matrix3 &m)
{
	Matrix3 temp;

	temp._11 = m._11 * scalar;
	temp._12 = m._12 * scalar;
	temp._13 = m._13 * scalar;

	temp._21 = m._21 * scalar;
	temp._22 = m._22 * scalar;
	temp._23 = m._23 * scalar;

	temp._31 = m._31 * scalar;
	temp._32 = m._32 * scalar;
	temp._33 = m._33 * scalar;

	return temp;
}
