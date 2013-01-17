/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef MATRIX3_H
#define MATRIX3_H

class Matrix3 {
public:
	float _11, _12, _13;
	float _21, _22, _23;
	float _31, _32, _33;

	Matrix3(){};
	Matrix3(float m11, float m12, float m13,
			float m21, float m22, float m23,
			float m31, float m32, float m33)
	{
		_11 = m11;
		_12 = m12;
		_13 = m13;

		_21 = m21;
		_22 = m22;
		_23 = m23;

		_31 = m31;
		_32 = m32;
		_33 = m33;
	};
/*	Matrix3(const Matrix4 &m)
	{
		_11 = m._11;
		_12 = m._12;
		_13 = m._13;

		_21 = m._21;
		_22 = m._22;
		_23 = m._23;

		_31 = m._31;
		_32 = m._32;
		_33 = m._33;
	};
*/
	~Matrix3(){};

	void operator+=(const Matrix3 &m)
	{
		_11 += m._11;
		_12 += m._12;
		_13 += m._13;

		_21 += m._21;
		_22 += m._22;
		_23 += m._23;

		_31 += m._31;
		_32 += m._32;
		_33 += m._33;
	};

	void operator-=(const Matrix3 &m)
	{
		_11 -= m._11;
		_12 -= m._12;
		_13 -= m._13;

		_21 -= m._21;
		_22 -= m._22;
		_23 -= m._23;

		_31 -= m._31;
		_32 -= m._32;
		_33 -= m._33;
	};

	Matrix3 operator+(const Matrix3 &m) const
	{
		return Matrix3(_11 + m._11, _12 + m._12, _13 + m._13, 
						_21 + m._21, _22 + m._22, _23 + m._23,
						_31 + m._31, _32 + m._32, _33 + m._33);
	};

	Matrix3 operator-(const Matrix3 &m) const
	{
		return Matrix3(_11 - m._11, _12 - m._12, _13 - m._13, 
						_21 - m._21, _22 - m._22, _23 - m._23,
						_31 - m._31, _32 - m._32, _33 - m._33);
	};

	void operator*=(const Matrix3 &m) 
	{
		Matrix3 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33;

		_11 = temp._11;
		_12 = temp._12;
		_13 = temp._13;

		_21 = temp._21;
		_22 = temp._22;
		_23 = temp._23;

		_31 = temp._31;
		_32 = temp._32;
		_33 = temp._33;
	};

	Matrix3 operator*(const Matrix3 &m) const
	{
		Matrix3 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33;

		return temp;
	};

	void operator*=(const float scalar)
	{
		Matrix3 temp;

		_11 *= scalar;
		_12 *= scalar;
		_13 *= scalar;

		_21 *= scalar;
		_22 *= scalar;
		_23 *= scalar;

		_31 *= scalar;
		_32 *= scalar;
		_33 *= scalar;
	};

	Matrix3 operator*(const float scalar) const
	{
		Matrix3 temp;

		temp._11 = _11 * scalar;
		temp._12 = _12 * scalar;
		temp._13 = _13 * scalar;

		temp._21 = _21 * scalar;
		temp._22 = _22 * scalar;
		temp._23 = _23 * scalar;

		temp._31 = _31 * scalar;
		temp._32 = _32 * scalar;
		temp._33 = _33 * scalar;

		return temp;
	};

	friend Matrix3 operator*(const float scalar, const Matrix3 &m);

	void Identity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;

		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
	};

	void Transpose()
	{
		float temp;
		
		temp = _21; 
		_21 = _12;
		_12 = temp;

		temp = _31; 
		_31 = _13;
		_13 = temp;

		temp = _23; 
		_23 = _32;
		_32 = temp;
	};

	void Inverse()
	{
		float Det = _11 * (_22 * _33 - _23 * _32)
				- _12 * (_21 * _33 - _23 * _31)
				+ _13 * (_21 * _32 - _22 * _31);
		float InvDet = 1.0f / Det;
		
		Matrix3 temp;
		
		temp._11 = (_22 * _33 - _23 * _32) * InvDet; 
		temp._12 = (_13 * _32 - _12 * _33) * InvDet; 
		temp._13 = (_12 * _23 - _13 * _22) * InvDet; 
		
		temp._21 = (_23 * _31 - _21 * _33) * InvDet; 
		temp._22 = (_11 * _33 - _13 * _31) * InvDet; 
		temp._23 = (_13 * _21 - _11 * _23) * InvDet; 
		
		temp._31 = (_21 * _32 - _22 * _31) * InvDet; 
		temp._32 = (_12 * _31 - _11 * _32) * InvDet; 
		temp._33 = (_11 * _22 - _12 * _21) * InvDet;
		
		*this = temp; 
	};


};

#endif 
