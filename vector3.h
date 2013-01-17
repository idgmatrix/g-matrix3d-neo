/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef VECTOR3_H
#define VECTOR3_H

#include "matrix.h"
#include "vector2.h"

struct Vector3 {
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
	float q;
	float b;
	};

	Vector3(){ x = y = z = 0;} // for vertex normal initialization
	Vector3(float a, float b, float c)
	{
		x = a;
		y = b;
		z = c;
	}
	Vector3(Vector2 & vec)
	{
		x = vec.x;
		y = vec.y;
		z = 1;
	}
	~Vector3(){}
	
	Vector3 operator-(void) const
	{
		return Vector3(-x, -y, -z);
	}

	Vector3 operator+(const Vector3 &rhs) const
	{
		return Vector3(x + rhs.x, y + rhs.y, z + rhs.z);
	}
	
	Vector3 operator-(const Vector3 &rhs) const
	{
		return Vector3(x - rhs.x, y - rhs.y, z - rhs.z);
	}
	
	Vector3 operator*(const float scalar) const
	{
		return Vector3(x * scalar, y * scalar, z * scalar);
	}

	Vector3 operator/(const float scalar) const
	{
		return Vector3(x / scalar, y / scalar, z / scalar);
	}

	friend Vector3 operator*(const float scalar, const Vector3 &rhs);
	
	float operator*(const Vector3 &rhs) const
	{
		return (x * rhs.x + y * rhs.y + z * rhs.z);
	}

	Vector3 operator*(const Matrix3 &m) const
	{
		Vector3 temp;
		
		temp.x = x * m._11 + y * m._21 + z * m._31; 
		temp.y = x * m._12 + y * m._22 + z * m._32; 
		temp.z = x * m._13 + y * m._23 + z * m._33; 
		
		return temp; 
	}
	Vector3 operator*(const Matrix4 &m) const
	{
		Vector3 temp;
		
		temp.x = x * m._11 + y * m._21 + z * m._31;// + m._41; 
		temp.y = x * m._12 + y * m._22 + z * m._32;// + m._42; 
		temp.z = x * m._13 + y * m._23 + z * m._33;// + m._43; 
		
		return temp; 
	}
	
	// Cross product
	Vector3 operator^(const Vector3 &rhs) const
	{
		Vector3 rvalue;

		rvalue.x = y * rhs.z - z * rhs.y;
		rvalue.y = z * rhs.x - x * rhs.z;
		rvalue.z = x * rhs.y - y * rhs.x;
 
		return rvalue;
	}

	Vector3 & operator+=(const Vector3 &rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;

		return *this;
	}
	
	Vector3 & operator-=(const Vector3 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;

		return *this;
	}
	
	Vector3 & operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;
		z *= scalar;

		return *this;
	}

	Vector3 & operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;
		z /= scalar;

		return *this;
	}
	
	void Normalize(void);
};

#endif
