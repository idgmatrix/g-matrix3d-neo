/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef VECTOR4_H
#define VECTOR4_H

#include "matrix.h"
#include "vector3.h"

struct Vector4 {
	union {
	float x;
	float u;
	float r;
	};
	union {
	float y;
	float v;
	float g;
	};
	union {
	float z;
	//float r;
	float b;
	};
	union {
	float w;
	float rhw;
	float q;
	float a;
	};

	Vector4(){}
	Vector4(float a, float b, float c, float d = 1)
	{
		x = a;
		y = b;
		z = c;
		w = d;
	}
	Vector4(const Vector3 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = vec.z;
		w = 1;
	}
	~Vector4(){}
	
	Vector4 operator-(void) const
	{
		return Vector4(-x, -y, -z, -w);
	}
	Vector4 operator+(const Vector4 &rhs) const
	{
		return Vector4(x + rhs.x, y + rhs.y, z + rhs.z, w + rhs.w);
	}
	
	Vector4 operator-(const Vector4 &rhs) const
	{
		return Vector4(x - rhs.x, y - rhs.y, z - rhs.z, w - rhs.w);
	}
	
	Vector4 operator*(const float scalar) const
	{
		return Vector4(x * scalar, y * scalar, z * scalar, w * scalar);
	}

	Vector4 operator/(const float scalar) const
	{
		return Vector4(x / scalar, y / scalar, z / scalar, w / scalar);
	}

	friend Vector4 operator*(const float scalar, const Vector4 &rhs);
	
	float operator*(const Vector4 &rhs) const
	{
		//return (x * rhs.x + y * rhs.y + z * rhs.z + w * rhs.w);
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	Vector4 operator*(const Matrix4 &m) const
	{
		Vector4 temp;
		
		temp.x = x * m._11 + y * m._21 + z * m._31 + w * m._41; 
		temp.y = x * m._12 + y * m._22 + z * m._32 + w * m._42; 
		temp.z = x * m._13 + y * m._23 + z * m._33 + w * m._43; 
		temp.w = x * m._14 + y * m._24 + z * m._34 + w * m._44;
		
		return temp; 
	}
	
	Vector4 & operator*=(const Matrix4 &m)
	{
		Vector4 temp;
		
		temp.x = x * m._11 + y * m._21 + z * m._31 + w * m._41; 
		temp.y = x * m._12 + y * m._22 + z * m._32 + w * m._42; 
		temp.z = x * m._13 + y * m._23 + z * m._33 + w * m._43; 
		temp.w = x * m._14 + y * m._24 + z * m._34 + w * m._44;
		
		*this = temp;
		return *this; 
	}
	
	Vector4 & operator+=(const Vector4 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		w += rhs.w;

		return *this;
	}
	
	Vector4 & operator-=(const Vector4 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		w -= rhs.w;

		return *this;
	}
	
	Vector4 & operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;
		w *= scalar;

		return *this;
	}

	Vector4 & operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;
		w /= scalar;

		return *this;
	}

public:
	void PerspectiveDivide(void);
};

#endif
