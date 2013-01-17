/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "mesh.h"
#include "renbuffer.h"

extern CRenBuffer RBuffer;

void Mesh::Render(void)
{
	RBuffer.NumVertex = this->NumVertex;	
	RBuffer.NumIndex = this->NumIndex;	
	RBuffer.VertexList = this->VertexList;
	RBuffer.IndexList = this->IndexList;
	RBuffer.MTLIDList = this->MTLIDList;
	RBuffer.MTLID = this->MTLID;

	if (!this->IsBone)
		RBuffer.RenderMesh();
}

