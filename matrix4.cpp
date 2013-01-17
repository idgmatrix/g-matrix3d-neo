/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "matrix4.h"

Matrix4 operator*(const float scalar, const Matrix4 &m)
{
	Matrix4 temp;

	temp._11 = m._11 * scalar;
	temp._12 = m._12 * scalar;
	temp._13 = m._13 * scalar;
	temp._14 = m._14 * scalar;

	temp._21 = m._21 * scalar;
	temp._22 = m._22 * scalar;
	temp._23 = m._23 * scalar;
	temp._24 = m._24 * scalar;

	temp._31 = m._31 * scalar;
	temp._32 = m._32 * scalar;
	temp._33 = m._33 * scalar;
	temp._34 = m._34 * scalar;

	temp._41 = m._41 * scalar;
	temp._42 = m._42 * scalar;
	temp._43 = m._43 * scalar;
	temp._44 = m._44 * scalar;

	return temp;
}
