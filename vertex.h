/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef VERTEX_H
#define VERTEX_H

#include "vector.h"

struct Vertex{
	Vector3 pos;   //float x, y, z
	Vector4 color;
	Vector3 normal;
	Vector2 texcoord;

	Vertex(){};
	Vertex(Vector3 p, Vector4 c, Vector3 n, Vector2 t)
	{
		pos = p;
		color = c;
		normal = n;
		texcoord = t;
	};
	~Vertex(){};
};

struct ViewVertex{
	Vector4 pos;	//float x, y, z
	Vector3 normal;
	int		clipflag;
	bool	IsLit;

	ViewVertex(){};
	ViewVertex(Vector3 p, Vector3 n)
	{
		pos = p;
		normal = n;
	};
	~ViewVertex(){};
};

struct TLVertex{
	Vector4 pos;	//float x, y, z, w;
	Vector4 diffuse;
	Vector4 specular;
	Vector3 texcoord;

	TLVertex(){};
	TLVertex(Vector4 p, Vector4 d, Vector4 s, Vector2 t)
	{
		pos = p;
		diffuse = d;
		specular = s;
		texcoord = t;
	};
	~TLVertex(){};
};

#endif 
