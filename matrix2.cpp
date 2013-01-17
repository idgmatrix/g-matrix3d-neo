/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "matrix2.h"

Matrix2 operator*(const float scalar, const Matrix2 &m)
{
	Matrix2 temp;

	temp._11 = m._11 * scalar;
	temp._12 = m._12 * scalar;

	temp._21 = m._21 * scalar;
	temp._22 = m._22 * scalar;

	return temp;
}
