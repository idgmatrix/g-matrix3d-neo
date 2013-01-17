/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef ASEMODEL_H
#define ASEMODEL_H

#include "stdafx.h"
#include "vector3.h"
#include "matrix3.h"
#include "mesh.h"
#include "model.h"
#include "material.h"

struct ASE_FACE{
	int index[3];
};

struct ASE_GEOMOBJECT{
	int NUMVERTEX;
	int NUMFACE;

	int NUMTVERTEX; 
	int NUMTFACE;
		
	Matrix3 NODE_TM;
	
	Vector3 *VertexList;
	Vector3 *TVertexList;
	
	ASE_FACE *FaceList;
	ASE_FACE *TFaceList;
	
	int *MTLIDList;
	int MTLID;
		
	Vector3 *VNormalList;
	
	bool BoneFlag;
	bool NormalFlag;
	bool TexCoordFlag;

	ASE_GEOMOBJECT()
	{
		NUMVERTEX = 0;
		NUMFACE = 0;

		NUMTVERTEX = 0; 
		NUMTFACE = 0;

		VertexList = NULL;
		TVertexList = NULL;
		
		FaceList = NULL;
		TFaceList = NULL;
		MTLIDList = NULL;
		VNormalList = NULL;
		
		BoneFlag = false;
		NormalFlag = false;
		TexCoordFlag = false;
	}
	~ASE_GEOMOBJECT()
	{
		if (VertexList) delete[] VertexList;
		if (TVertexList) delete[] TVertexList;
		if (FaceList) delete[] FaceList;
		if (TFaceList) delete[] TFaceList;
		if (MTLIDList) delete[] MTLIDList;
		if (VNormalList) delete[] VNormalList;
	}
};

class ASEMODEL{
	int NUMGEOMOBJECT;
	int NUMMATERIAL;
	
	ASE_GEOMOBJECT *GeomObjectList;
	Material *MaterialList;

	int linecount;
	int GeomIndex;

	char line[256];
	char string[80];
	
public:
	ASEMODEL()
	{
		NUMGEOMOBJECT = 0;
		NUMMATERIAL = 0;
		GeomObjectList = NULL;
		MaterialList = NULL;
		GeomIndex = 0;
		linecount = 0;
	}
	~ASEMODEL()
	{
		delete[] GeomObjectList;
		delete[] MaterialList;
	}
	
	void Init(int num = 200)
	{
		GeomObjectList = new ASE_GEOMOBJECT[num];
	}
	
	Model *LoadASE(char *fname);

private:
	void MakeVertexNormal(int gindex = 0);
	
	void TransferToMesh(Mesh *pMesh, int gindex = 0);
	void TransferToMeshNoTexture(Mesh *pMesh, int gindex = 0);
	
	int DecodeASE(FILE *fp);
	int DecodeSCENE(FILE *fp);
	int DecodeMATERIAL_LIST(FILE *fp);
	int DecodeGEOMOBJECT(FILE *fp);
	int DecodeMESH(FILE *fp);
	int DecodeMESH_VERTEX_LIST(FILE *fp);
	int DecodeMESH_FACE_LIST(FILE *fp);
	int DecodeMESH_TVERTLIST(FILE *fp);
	int DecodeMESH_TFACELIST(FILE *fp);
	int DecodeMESH_CVERTEX(FILE *fp);
	int DecodeMESH_NORMALS(FILE *fp);
};

#endif

