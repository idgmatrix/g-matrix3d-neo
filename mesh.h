/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef MESH_H
#define MESH_H

#include "stdafx.h"

#include "vector.h"
#include "matrix.h"
#include "vertex.h"

class Mesh{
public:	
	int NumVertex;
	Vertex *VertexList;

	int NumIndex;
	int *IndexList;
	
	int *MTLIDList;

	int MTLID;
	
	bool IsBone;
	bool IsCreated;
	 
	Mesh()
	{
		NumVertex = 0;
		VertexList = NULL;

		NumIndex = 0;
		IndexList = NULL;
	
		MTLID = 0;
		MTLIDList = NULL;
		
		IsBone = false;
		IsCreated = false;
	}
	~Mesh()
	{
		NumVertex = 0;
		NumIndex = 0;
	
		if (IsCreated)
		{
			delete [] VertexList;
			delete [] IndexList;
			delete [] MTLIDList;
		}
	}

	void CreateMesh(int numv, int numi)
	{
		NumVertex = numv; 
		VertexList = new Vertex[NumVertex];
		
		NumIndex = numi;
		IndexList = new int[NumIndex];
		
		MTLID = -1;
		MTLIDList = new int[NumIndex / 3];

		IsCreated = true;
	}
	
	// -------------------------
	void LoadMesh(Vertex * Vlist, int * Ilist, int numv, int numi)
	{
		VertexList = Vlist;
		IndexList = Ilist;

		NumVertex = numv; 
		NumIndex = numi;
	}
	
	void Render(void);
};

#endif

