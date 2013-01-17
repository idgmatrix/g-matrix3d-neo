/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef MATRIX4_H
#define MATRIX4_H

class Matrix4 {
public:
	float _11, _12, _13, _14;
	float _21, _22, _23, _24;
	float _31, _32, _33, _34;
	float _41, _42, _43, _44;

	Matrix4(){};
	Matrix4(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44)
	{
		_11 = m11;
		_12 = m12;
		_13 = m13;
		_14 = m14;

		_21 = m21;
		_22 = m22;
		_23 = m23;
		_24 = m24;

		_31 = m31;
		_32 = m32;
		_33 = m33;
		_34 = m34;

		_41 = m41;
		_42 = m42;
		_43 = m43;
		_44 = m44;
	};

	~Matrix4(){};

	void operator+=(const Matrix4 &m)
	{
		_11 += m._11;
		_12 += m._12;
		_13 += m._13;
		_14 += m._14;
		
		_21 += m._21;
		_22 += m._22;
		_23 += m._23;
		_24 += m._24;

		_31 += m._31;
		_32 += m._32;
		_33 += m._33;
		_34 += m._34;

		_41 += m._41;
		_42 += m._42;
		_43 += m._43;
		_44 += m._44;
	};

	void operator-=(const Matrix4 &m)
	{
		_11 -= m._11;
		_12 -= m._12;
		_13 -= m._13;
		_14 -= m._14;
		
		_21 -= m._21;
		_22 -= m._22;
		_23 -= m._23;
		_24 -= m._24;

		_31 -= m._31;
		_32 -= m._32;
		_33 -= m._33;
		_34 -= m._34;

		_41 -= m._41;
		_42 -= m._42;
		_43 -= m._43;
		_44 -= m._44;
	};

	Matrix4 operator+(const Matrix4 &m) const
	{
		return Matrix4(_11 + m._11, _12 + m._12, _13 + m._13, _14 + m._14, 
						_21 + m._21, _22 + m._22, _23 + m._23, _24 + m._24,
						_31 + m._31, _32 + m._32, _33 + m._33, _34 + m._34,
						_41 + m._41, _42 + m._42, _43 + m._43, _44 + m._44);
	};

	Matrix4 operator-(const Matrix4 &m) const
	{
		return Matrix4(_11 - m._11, _12 - m._12, _13 - m._13, _14 - m._14, 
						_21 - m._21, _22 - m._22, _23 - m._23, _24 - m._24,
						_31 - m._31, _32 - m._32, _33 - m._33, _34 - m._34,
						_41 - m._41, _42 - m._42, _43 - m._43, _34 - m._44);
	};

	void operator*=(const Matrix4 &m) 
	{
		Matrix4 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43;
		temp._14 = _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43;
		temp._24 = _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43;
		temp._34 = _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44;

		temp._41 = _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41;
		temp._42 = _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42;
		temp._43 = _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43;
		temp._44 = _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44;

		_11 = temp._11;
		_12 = temp._12;
		_13 = temp._13;
		_14 = temp._14;

		_21 = temp._21;
		_22 = temp._22;
		_23 = temp._23;
		_24 = temp._24;

		_31 = temp._31;
		_32 = temp._32;
		_33 = temp._33;
		_34 = temp._34;

		_41 = temp._41;
		_42 = temp._42;
		_43 = temp._43;
		_44 = temp._44;
	};

	Matrix4 operator*(const Matrix4 &m) const
	{
		Matrix4 temp;

		temp._11 = _11 * m._11 + _12 * m._21 + _13 * m._31 + _14 * m._41;
		temp._12 = _11 * m._12 + _12 * m._22 + _13 * m._32 + _14 * m._42;
		temp._13 = _11 * m._13 + _12 * m._23 + _13 * m._33 + _14 * m._43;
		temp._14 = _11 * m._14 + _12 * m._24 + _13 * m._34 + _14 * m._44;

		temp._21 = _21 * m._11 + _22 * m._21 + _23 * m._31 + _24 * m._41;
		temp._22 = _21 * m._12 + _22 * m._22 + _23 * m._32 + _24 * m._42;
		temp._23 = _21 * m._13 + _22 * m._23 + _23 * m._33 + _24 * m._43;
		temp._24 = _21 * m._14 + _22 * m._24 + _23 * m._34 + _24 * m._44;

		temp._31 = _31 * m._11 + _32 * m._21 + _33 * m._31 + _34 * m._41;
		temp._32 = _31 * m._12 + _32 * m._22 + _33 * m._32 + _34 * m._42;
		temp._33 = _31 * m._13 + _32 * m._23 + _33 * m._33 + _34 * m._43;
		temp._34 = _31 * m._14 + _32 * m._24 + _33 * m._34 + _34 * m._44;

		temp._41 = _41 * m._11 + _42 * m._21 + _43 * m._31 + _44 * m._41;
		temp._42 = _41 * m._12 + _42 * m._22 + _43 * m._32 + _44 * m._42;
		temp._43 = _41 * m._13 + _42 * m._23 + _43 * m._33 + _44 * m._43;
		temp._44 = _41 * m._14 + _42 * m._24 + _43 * m._34 + _44 * m._44;

		return temp;
	};

	void operator*=(const float scalar)
	{
		Matrix4 temp;

		_11 *= scalar;
		_12 *= scalar;
		_13 *= scalar;
		_14 *= scalar;

		_21 *= scalar;
		_22 *= scalar;
		_23 *= scalar;
		_24 *= scalar;

		_31 *= scalar;
		_32 *= scalar;
		_33 *= scalar;
		_34 *= scalar;

		_41 *= scalar;
		_42 *= scalar;
		_43 *= scalar;
		_44 *= scalar;
	};

	Matrix4 operator*(const float scalar) const
	{
		Matrix4 temp;

		temp._11 = _11 * scalar;
		temp._12 = _12 * scalar;
		temp._13 = _13 * scalar;
		temp._14 = _14 * scalar;
		
		temp._21 = _21 * scalar;
		temp._22 = _22 * scalar;
		temp._23 = _23 * scalar;
		temp._24 = _24 * scalar;
		
		temp._31 = _31 * scalar;
		temp._32 = _32 * scalar;
		temp._33 = _33 * scalar;
		temp._34 = _34 * scalar;

		temp._41 = _41 * scalar;
		temp._42 = _42 * scalar;
		temp._43 = _43 * scalar;
		temp._44 = _44 * scalar;

		return temp;
	};

	friend Matrix4 operator*(const float scalar, const Matrix4 &m);

	void Identity()
	{
		_11 = 1.0f;
		_12 = 0.0f;
		_13 = 0.0f;
		_14 = 0.0f;

		_21 = 0.0f;
		_22 = 1.0f;
		_23 = 0.0f;
		_24 = 0.0f;

		_31 = 0.0f;
		_32 = 0.0f;
		_33 = 1.0f;
		_34 = 0.0f;

		_41 = 0.0f;
		_42 = 0.0f;
		_43 = 0.0f;
		_44 = 1.0f;
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

		temp = _41; 
		_41 = _14;
		_14 = temp;

		temp = _34; 
		_34 = _43;
		_43 = temp;

		temp = _24; 
		_24 = _42;
		_42 = temp;
	};

	void Inverse()
	{
	};


};

#endif 
