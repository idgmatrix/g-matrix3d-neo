/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef VECTOR2_H
#define VECTOR2_H

#include "matrix.h"

struct Vector2 {
	union {
	float x;
	float u;
	};
	union {
	float y;
	float v;
	};

	Vector2(){}
	Vector2(float a, float b)
	{
		x = a;
		y = b;
	}
	~Vector2(){}
	
	Vector2 operator-(void) const
	{
		return Vector2(-x, -y);
	}
	
	Vector2 operator+(const Vector2 &rhs) const
	{
		return Vector2(x + rhs.x, y + rhs.y);
	};
	Vector2 operator-(const Vector2 &rhs) const
	{
		return Vector2(x - rhs.x, y - rhs.y);
	};
	Vector2 operator*(const float scalar) const
	{
		return Vector2(x * scalar, y * scalar);
	};
	Vector2 operator/(const float scalar) const
	{
		return Vector2(x / scalar, y / scalar);
	};
	
	friend Vector2 operator*(const float scalar, const Vector2 &rhs);
	
	float operator*(const Vector2 &rhs) const
	{
		return (x * rhs.x + y * rhs.y);
	};

	Vector2 operator*(const Matrix2 m) const
	{
		Vector2 temp;
		
		temp.x = x * m._11 + y * m._21; 
		temp.y = x * m._12 + y * m._22; 
		
		return temp; 
	};
	
	float operator^(const Vector2 &rhs) const
	{
		return (x * rhs.y - y * rhs.x);
	}

	Vector2 & operator+=(const Vector2 &rhs)
	{
		x += rhs.x;
		y += rhs.y;

		return *this;
	}
	Vector2 & operator-=(const Vector2 &rhs)
	{
		x -= rhs.x;
		y -= rhs.y;

		return *this;
	}
	Vector2 & operator*=(const float scalar)
	{
		x *= scalar;
		y *= scalar;

		return *this;
	}
	Vector2 & operator/=(const float scalar)
	{
		x /= scalar;
		y /= scalar;

		return *this;
	}
	void Normalize(void);
};

#endif
