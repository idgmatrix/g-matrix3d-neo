/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef MATRIX2_H
#define MATRIX2_H

class Matrix2 {
public:
	float _11, _12;
	float _21, _22;

	Matrix2(){};
	Matrix2(float m11, float m12, float m21, float m22)
	{
		_11 = m11;
		_12 = m12;

		_21 = m21;
		_22 = m22;
	};

	~Matrix2(){};

	void operator+=(const Matrix2 &m)
	{
		_11 += m._11;
		_12 += m._12;

		_21 += m._21;
		_22 += m._22;
	};

	void operator-=(const Matrix2 &m)
	{
		_11 -= m._11;
		_12 -= m._12;

		_21 -= m._21;
		_22 -= m._22;
	};

	Matrix2 operator+(const Matrix2 &m) const
	{
		return Matrix2(_11 + m._11, _12 + m._12, _21 + m._21, _22 + m._22);
	};

	Matrix2 operator-(const Matrix2 &m) const
	{
		return Matrix2(_11 - m._11, _12 - m._12, _21 - m._21, _22 - m._22);
	};

	void operator*=(const Matrix2 &m) 
	{
		Matrix2 temp;

		temp._11 = _11 * m._11 + _12 * m._21;
		temp._12 = _11 * m._12 + _12 * m._22;

		temp._21 = _21 * m._11 + _22 * m._21;
		temp._22 = _21 * m._12 + _22 * m._22;

		_11 = temp._11;
		_12 = temp._12;

		_21 = temp._21;
		_22 = temp._22;
	};

	Matrix2 operator*(const Matrix2 &m) const
	{
		Matrix2 temp;

		temp._11 = _11 * m._11 + _12 * m._21;
		temp._12 = _11 * m._12 + _12 * m._22;

		temp._21 = _21 * m._11 + _22 * m._21;
		temp._22 = _21 * m._12 + _22 * m._22;

		return temp;
	};

	void operator*=(const float scalar)
	{
		Matrix2 temp;

		_11 *= scalar;
		_12 *= scalar;

		_21 *= scalar;
		_22 *= scalar;
	};

	Matrix2 operator*(const float scalar) const
	{
		Matrix2 temp;

		temp._11 = _11 * scalar;
		temp._12 = _12 * scalar;

		temp._21 = _21 * scalar;
		temp._22 = _22 * scalar;

		return temp;
	};

	friend Matrix2 operator*(const float scalar, const Matrix2 &m);

	void Identity()
	{
		_11 = 1.0f;
		_12 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
	};

	void Transpose()
	{
		float temp;
		
		temp = _21; 
		_21 = _12;
		_12 = temp;
	};

	void Inverse()
	{
		float Det = _11 * _22 - _12 * _21;
		float InvDet = 1.0f / Det;

		float temp_11 = _11;

		_11 = _22 * InvDet;
		_12 = -_12 * InvDet;
		_21 = -_21 * InvDet;
		_22 = temp_11 * InvDet;
	};


};

#endif 
