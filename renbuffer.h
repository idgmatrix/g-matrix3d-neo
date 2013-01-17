/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef RENBUFFER_H
#define RENBUFFER_H

#include "vertex.h"
#include "graphics.h"

//#define MAXVERTEX 5000

struct RenderInfo{
	int NumVertex;
	int NumIndex;
	int NumActiveVertex;
	int NumClippedVertex;
	int NumRenderIndex;
	int NumDrawed;
	int NumMesh;
	void Clear(void)
	{
		NumVertex = 0;
		NumIndex = 0;
		NumActiveVertex = 0;
		NumClippedVertex = 0;
		NumRenderIndex = 0;
		NumDrawed = 0;
		NumMesh = 0;
	}

	void Display(int x, int y)
	{
		PutNumber(x, y,       "Vert", NumVertex);
		PutNumber(x, y + 20,  "Face", NumIndex / 3);

		PutNumber(x, y + 40,  "RenderVert", NumActiveVertex);
		PutNumber(x, y + 60,  "RenderFace", NumRenderIndex / 3);
		
		PutNumber(x, y + 80,  "ClippedVert", NumClippedVertex);
		PutNumber(x, y + 100, "DrawedFace", NumDrawed);
		
		PutNumber(x, y + 120, "Mesh", NumMesh);
	}
};

class CRenBuffer{
public:	
	int NumVertex;
	Vertex *VertexList;
	ViewVertex *ViewVertexList;
	TLVertex *TLVertexList;
	
	int NumActiveVertex;
	int *ActiveVertexIndexList;

	int NumTri;
	int NumIndex;
	int NumRenderIndex;
	int *IndexList;
	int *RenderIndexList;
	
	int *MTLIDList;
	int MTLID;

	int DrawedCounter;

	CRenBuffer(int MAXVERTEX = 50000)
	{
		NumVertex = 0;
		VertexList = NULL;
		ViewVertexList = new ViewVertex[MAXVERTEX];
		TLVertexList = new TLVertex[MAXVERTEX];
	
		NumActiveVertex = 0;
		ActiveVertexIndexList = new int[MAXVERTEX];

		NumTri = 0;
		NumIndex = 0;
		RenderIndexList = new int[MAXVERTEX * 6];
	
		MTLIDList = new int[MAXVERTEX * 6];
		
		DrawedCounter = 0;
	}
	~CRenBuffer()
	{
		NumVertex = 0;
		VertexList = NULL;
		delete [] ViewVertexList;
		delete [] TLVertexList;
	
		NumActiveVertex = 0;
		delete [] ActiveVertexIndexList;

		NumTri = 0;
		NumIndex = 0;
		delete [] RenderIndexList;
		delete [] MTLIDList;
	}

	void VertexLighting(int vindex);
	void VertexLightingBack(int vindex);

	void RenderMesh(void);

};

#endif

