/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "asemodel.h"
#include "model.h"
#include "matmanager.h"

#define SKIPLINE		fgets(line, 256, fp)

extern CMatManager MATMAN;
extern bool g_bIsASE;

static int BRACE_COUNT = 0;
static bool FindOpenBrace(char * line);
static bool FindCloseBrace(char * line);
static bool FindCloseBraceNoCount(char * line);
static bool FindBone(char *line);

Model *ASEMODEL::LoadASE(char *fname)
{
	FILE *fp;
	
	fp = fopen(fname, "rt");
	if ( fp == NULL )
	{
		return NULL;
	}
	
	MATMAN.Destroy();
	
	DecodeASE(fp);
	
	fclose(fp);

	if (NUMMATERIAL == 0) MATMAN.Init(1);
		
	Model *pModel = new Model;
	pModel->CreateModel(NUMGEOMOBJECT);
  
	int i;
    for (i = 0; i < NUMMATERIAL; i++)
	{
		MATMAN.MaterialList[i] = MaterialList[i];
	}

	for (i = 0; i < NUMGEOMOBJECT; i++)
	{
		if (GeomObjectList[i].NormalFlag == false)
			MakeVertexNormal(i);
	}

	for (i = 0; i < NUMGEOMOBJECT; i++)
	{
		if (GeomObjectList[i].TexCoordFlag == true)
			TransferToMesh(&pModel->MeshList[i], i);
		else
			TransferToMeshNoTexture(&pModel->MeshList[i], i);
	}

	g_bIsASE = true;

	char string[256], name[256];
	sprintf(string, "%s", "soft3d <g-Matrix3d Neo>");
	sprintf(name, " - [%s]", fname);
	strcat(string, name);
	SetWindowText(GetActiveWindow(), string);

	return 	pModel;

}


void ASEMODEL::MakeVertexNormal(int gindex)
{
	int i;

	for (i = 0; i < GeomObjectList[gindex].NUMFACE; i++)
	{
		int i0 = GeomObjectList[gindex].FaceList[i].index[0];
		int i1 = GeomObjectList[gindex].FaceList[i].index[1];
		int i2 = GeomObjectList[gindex].FaceList[i].index[2];
		
		Vector3 A = GeomObjectList[gindex].VertexList[i0];
		Vector3 B = GeomObjectList[gindex].VertexList[i1];
		Vector3 C = GeomObjectList[gindex].VertexList[i2];
		
		Vector3 FaceNormal = (B - A) ^ (C - A);
		FaceNormal.Normalize();

		GeomObjectList[gindex].VNormalList[i0] += FaceNormal; 
		GeomObjectList[gindex].VNormalList[i1] += FaceNormal; 
		GeomObjectList[gindex].VNormalList[i2] += FaceNormal; 
	}
	
	for (i = 0; i < GeomObjectList[gindex].NUMVERTEX; i++)
	{
		GeomObjectList[gindex].VNormalList[i].Normalize(); 
	}
}

void ASEMODEL::TransferToMesh(Mesh *pMesh, int gindex)
{
	int i;
	
	pMesh->CreateMesh(GeomObjectList[gindex].NUMTVERTEX, GeomObjectList[gindex].NUMTFACE * 3);

	pMesh->MTLID = GeomObjectList[gindex].MTLID;
	pMesh->IsBone = GeomObjectList[gindex].BoneFlag;

	for (i = 0; i < GeomObjectList[gindex].NUMTVERTEX; i++)
	{
		pMesh->VertexList[i].texcoord.u = GeomObjectList[gindex].TVertexList[i].u;
		pMesh->VertexList[i].texcoord.v = GeomObjectList[gindex].TVertexList[i].v;
		pMesh->VertexList[i].color = Vector4(1,1,1,1);
	}

	for (i = 0; i < GeomObjectList[gindex].NUMTFACE; i++)
	{
		pMesh->IndexList[i*3 + 0] = GeomObjectList[gindex].TFaceList[i].index[0];
		pMesh->IndexList[i*3 + 1] = GeomObjectList[gindex].TFaceList[i].index[1];
		pMesh->IndexList[i*3 + 2] = GeomObjectList[gindex].TFaceList[i].index[2];
		
		pMesh->MTLIDList[i] = GeomObjectList[gindex].MTLIDList[i];
	}

	for (i = 0; i < GeomObjectList[gindex].NUMTFACE; i++)
	{
		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[0] ].pos 
		= GeomObjectList[gindex].VertexList[ GeomObjectList[gindex].FaceList[i].index[0] ];

		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[1] ].pos
		= GeomObjectList[gindex].VertexList[ GeomObjectList[gindex].FaceList[i].index[1] ];

		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[2] ].pos 
		= GeomObjectList[gindex].VertexList[ GeomObjectList[gindex].FaceList[i].index[2] ];

		
		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[0] ].normal 
		= GeomObjectList[gindex].VNormalList[ GeomObjectList[gindex].FaceList[i].index[0] ];
		
		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[1] ].normal 
		= GeomObjectList[gindex].VNormalList[ GeomObjectList[gindex].FaceList[i].index[1] ];
		
		pMesh->VertexList[ GeomObjectList[gindex].TFaceList[i].index[2] ].normal 
		= GeomObjectList[gindex].VNormalList[ GeomObjectList[gindex].FaceList[i].index[2] ];
	}
}

void ASEMODEL::TransferToMeshNoTexture(Mesh *pMesh, int gindex)
{
	int i;

	pMesh->CreateMesh(GeomObjectList[gindex].NUMVERTEX, GeomObjectList[gindex].NUMFACE * 3);

	pMesh->MTLID = GeomObjectList[gindex].MTLID;
	pMesh->IsBone = GeomObjectList[gindex].BoneFlag;

	for (i = 0; i < GeomObjectList[gindex].NUMVERTEX; i++)
	{
		pMesh->VertexList[i].pos = GeomObjectList[gindex].VertexList[i];
		pMesh->VertexList[i].color = Vector4(1,1,1,1);
		pMesh->VertexList[i].texcoord = Vector2(0,0);
		pMesh->VertexList[i].normal = GeomObjectList[gindex].VNormalList[i];
	}

	for (i = 0; i < GeomObjectList[gindex].NUMFACE; i++)
	{
		pMesh->IndexList[i*3 + 0] = GeomObjectList[gindex].FaceList[i].index[0];
		pMesh->IndexList[i*3 + 1] = GeomObjectList[gindex].FaceList[i].index[1];
		pMesh->IndexList[i*3 + 2] = GeomObjectList[gindex].FaceList[i].index[2];
		
		pMesh->MTLIDList[i] = GeomObjectList[gindex].MTLIDList[i];
	}
}

int ASEMODEL::DecodeASE(FILE *fp)
{
	linecount++;
	fgets(line, 256, fp);
	sscanf(line, "%s", string);

	// CHECK ASE FILE
	if (_stricmp(string, "*3DSMAX_ASCIIEXPORT") != 0) return -1;

	while ( !feof(fp) )
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		
		if (_stricmp(string, "*COMMENT") == 0) continue;
		
		///////////////////////
		// Scene Descriptions
		if (_stricmp(string, "*SCENE") == 0)
			DecodeSCENE(fp);
		
		//////////////////////////////
		// material list
		if (_stricmp(string, "*MATERIAL_LIST") == 0)
			DecodeMATERIAL_LIST(fp);
		
		////////////////////
		// geometry object
		if (_stricmp(string, "*GEOMOBJECT") == 0)
			DecodeGEOMOBJECT(fp);
	}

	return linecount;	
}

int ASEMODEL::DecodeSCENE(FILE *fp)
{
	linecount += 8;
	
	fgets(line, 256, fp);//*SCENE_FILENAME	
	fgets(line, 256, fp);//*SCENE_FIRSTFRAME	
	fgets(line, 256, fp);//*SCENE_LASTFRAME	
	fgets(line, 256, fp);//*SCENE_FRAMESPEED	
	fgets(line, 256, fp);//*SCENE_TICKSPERFRAME	
	fgets(line, 256, fp);//*SCENE_BACKGROUND_STATIC	
	fgets(line, 256, fp);//*SCENE_AMBIENT_STATIC	
	fgets(line, 256, fp);//}
	
	return 0;
}
		
int ASEMODEL::DecodeMATERIAL_LIST(FILE *fp)
{

	linecount++;
	fgets(line, 256, fp);//*MATERIAL_COUNT	
	sscanf(line, "%s%d", string, &NUMMATERIAL); 
	
	if (NUMMATERIAL == 0) 
	{
		linecount++;
		fgets(line, 256, fp);//}
		return 0;
	}
	else MATMAN.Init(NUMMATERIAL);

	MaterialList = new Material[NUMMATERIAL];

	for (int mindex = 0; mindex < NUMMATERIAL; mindex++)
	{
		linecount++;
		fgets(line, 256, fp);//*MATERIAL 0 {
		FindOpenBrace(line);
		sscanf(line, "%s%d", string, &mindex); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_NAME "CICETO" 
		sscanf(line, "%s%s", string, string); 
		
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_CLASS "Standard"
		sscanf(line, "%s%s", string, string); 

		linecount++;
		fgets(line, 256, fp);//*MATERIAL_AMBIENT 0.3176	0.5137	0.8078
		sscanf(line, "%s%f%f%f", string, 
			&MaterialList[mindex].ambient.r,
			&MaterialList[mindex].ambient.g,
			&MaterialList[mindex].ambient.b); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_DIFFUSE 0.3176	0.5137	0.8078
		sscanf(line, "%s%f%f%f", string, 
			&MaterialList[mindex].diffuse.r,
			&MaterialList[mindex].diffuse.g,
			&MaterialList[mindex].diffuse.b); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_SPECULAR 0.9000	0.9000	0.9000
		sscanf(line, "%s%f%f%f", string, 
			&MaterialList[mindex].specular.r,
			&MaterialList[mindex].specular.g,
			&MaterialList[mindex].specular.b); 
	
		float shine;
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_SHINE 0.0000
		sscanf(line, "%s%f", string, &shine);
		MaterialList[mindex].shiness = int(shine * 100);
		if (MaterialList[mindex].shiness > 128) MaterialList[mindex].shiness = 128;
	
		float strength;
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_SHINESTRENGTH 0.0000
		sscanf(line, "%s%f", string, &strength);
		if (strength > 1.0f) strength = 1.0f;
		MaterialList[mindex].specular *= strength;
					 
/*	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_TRANSPARENCY 0.0000
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_WIRESIZE 1.0000
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_SHADING Blinn
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_XP_FALLOFF 0.0000
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_SELFILLUM 0.0000
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_FALLOFF In
		sscanf(line, "%s%s", string, string); 
	
		linecount++;
		fgets(line, 256, fp);//*MATERIAL_XP_TYPE Filter
		sscanf(line, "%s%s", string, string); 
*/		
		do {
			linecount++;
			fgets(line, 256, fp);
			FindOpenBrace(line);
			FindCloseBrace(line);
		} while(BRACE_COUNT > 0); 
	
	}

/*
		*MAP_AMBIENT {
			*MAP_NAME "Map #6"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 0
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Brick\Brkrun.JPG"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_DIFFUSE {
			*MAP_NAME "Map #12"
			*MAP_CLASS "Noise"
			*MAP_SUBNO 1
			*MAP_AMOUNT 1.0000
		}
		*MAP_SPECULAR {
			*MAP_NAME "Map #10"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 2
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Brick\Yellobrk.JPG"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_SHINE {
			*MAP_NAME "Map #11"
			*MAP_CLASS "Noise"
			*MAP_SUBNO 3
			*MAP_AMOUNT 1.0000
		}
		*MAP_SHINESTRENGTH {
			*MAP_NAME "Map #2"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 4
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Fabric\Carptgry.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_SELFILLUM {
			*MAP_NAME "Map #60"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 5
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Concrete\Stucco.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_OPACITY {
			*MAP_NAME "Map #61"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 6
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Concrete\Stucco4.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_FILTERCOLOR {
			*MAP_NAME "Brown_Concrete"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 7
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Concrete\Grycon7.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_BUMP {
			*MAP_NAME "Map #64"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 8
			*MAP_AMOUNT 0.3000
			*BITMAP "D:\3dsmax5\maps\Concrete\Tiles.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_INVERT
			*BITMAP_FILTER SAT
		}
		*MAP_REFLECT {
			*MAP_NAME "Map #1"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 9
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Fabric\Carptblu.jpg"
			*MAP_TYPE Spherical
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
		*MAP_REFRACT {
			*MAP_NAME "Map #63"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 10
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Concrete\Tiles.jpg"
			*MAP_TYPE Spherical
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER SAT
		}
		*MAP_REFRACT {
			*MAP_NAME "Map #49"
			*MAP_CLASS "Bitmap"
			*MAP_SUBNO 11
			*MAP_AMOUNT 1.0000
			*BITMAP "D:\3dsmax5\maps\Concrete\Asphalt3.jpg"
			*MAP_TYPE Screen
			*UVW_U_OFFSET 0.0000
			*UVW_V_OFFSET 0.0000
			*UVW_U_TILING 1.0000
			*UVW_V_TILING 1.0000
			*UVW_ANGLE 0.0000
			*UVW_BLUR 1.0000
			*UVW_BLUR_OFFSET 0.0000
			*UVW_NOUSE_AMT 1.0000
			*UVW_NOISE_SIZE 1.0000
			*UVW_NOISE_LEVEL 1
			*UVW_NOISE_PHASE 0.0000
			*BITMAP_FILTER Pyramidal
		}
	
	}		
*/

	return 0;
}
		
int ASEMODEL::DecodeGEOMOBJECT(FILE *fp)
{
	NUMGEOMOBJECT++;
	
	linecount += 17;
	fgets(line, 256, fp);//*NODE_NAME
	if (FindBone(line)) GeomObjectList[GeomIndex].BoneFlag = true;

	fgets(line, 256, fp);//*NODE_PARENT
	sscanf(line, "%s", string);
	if (_stricmp(string, "*NODE_PARENT") == 0)
	{
		linecount++;
		fgets(line, 256, fp);//*NODE_TM {
		fgets(line, 256, fp);//*NODE_NAME
		fgets(line, 256, fp);//*INHERIT_POS
		fgets(line, 256, fp);//*INHERIT_ROT
		fgets(line, 256, fp);//*INHERIT_SCL
	}
	else
	{
		fgets(line, 256, fp);//*NODE_NAME
		fgets(line, 256, fp);//*INHERIT_POS
		fgets(line, 256, fp);//*INHERIT_ROT
		fgets(line, 256, fp);//*INHERIT_SCL
	}

	fgets(line, 256, fp);//*TM_ROW0
	sscanf(line, "%s%f%f%f", string, 
		&GeomObjectList[GeomIndex].NODE_TM._11, 
		&GeomObjectList[GeomIndex].NODE_TM._12, 
		&GeomObjectList[GeomIndex].NODE_TM._13);

	fgets(line, 256, fp);//*TM_ROW1
	sscanf(line, "%s%f%f%f", string, 
		&GeomObjectList[GeomIndex].NODE_TM._21, 
		&GeomObjectList[GeomIndex].NODE_TM._22, 
		&GeomObjectList[GeomIndex].NODE_TM._23);

	fgets(line, 256, fp);//*TM_ROW2
	sscanf(line, "%s%f%f%f", string, 
		&GeomObjectList[GeomIndex].NODE_TM._31, 
		&GeomObjectList[GeomIndex].NODE_TM._32, 
		&GeomObjectList[GeomIndex].NODE_TM._33);

	fgets(line, 256, fp);//*TM_ROW3

	fgets(line, 256, fp);//*TM_POS
	fgets(line, 256, fp);//*TM_ROTAXIS
	fgets(line, 256, fp);//*TM_ROTANGLE 
	fgets(line, 256, fp);//*TM_SCALE
	fgets(line, 256, fp);//*TM_SCALEAXIS
	fgets(line, 256, fp);//*TM_SCALEAXISANG
	fgets(line, 256, fp);//}

	linecount++;
	fgets(line, 256, fp);
	sscanf(line, "%s", string);
	if (_stricmp(string, "*MESH") == 0)
		DecodeMESH(fp);

	while(1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if (_stricmp(string, "*TM_ANIMATION") == 0)
		{
			BRACE_COUNT = 1;
			do {
				linecount++;
				fgets(line, 512, fp);
				FindOpenBrace(line);
				FindCloseBrace(line);
			} while(BRACE_COUNT > 0); 
		}
		if (_stricmp(string, "*WIREFRAME_COLOR") == 0)
		{	
			GeomObjectList[GeomIndex].MTLID = 0;	///////////////////////
			
			//GeomObjectList[GeomIndex].BoneFlag = true; //////////////////////
			
			linecount++;
			fgets(line, 256, fp); // "}"
			break;
		}
		if (_stricmp(string, "*MATERIAL_REF") == 0)
		{
			sscanf(line, "%s%d", string, &GeomObjectList[GeomIndex].MTLID);
			
			linecount++;
			fgets(line, 256, fp); // "}"
			break;
		}
		
		if (FindCloseBraceNoCount(line))
		{
			GeomObjectList[GeomIndex].MTLID = 0;	///////////////////////
			break;
		}
	
	}
	
	//GeomObjectList[GeomIndex].MTLID = 0;	///////////////////////

//		if (GeomObjectList[GeomIndex].MTLID == -1)
//		{
//			linecount++;
//		}
	
	GeomIndex++;
	
	return 0;
}

int ASEMODEL::DecodeMESH(FILE *fp)
{
	linecount++;
	fgets(line, 256, fp);//*TIMEVALUE
		
	linecount++;
	fgets(line, 256, fp);//*MESH_NUMVERTEX
	sscanf(line, "%s%d", string, &GeomObjectList[GeomIndex].NUMVERTEX);

	GeomObjectList[GeomIndex].VertexList = new Vector3[GeomObjectList[GeomIndex].NUMVERTEX];
	GeomObjectList[GeomIndex].VNormalList = new Vector3[GeomObjectList[GeomIndex].NUMVERTEX];
	
	linecount++;
	fgets(line, 256, fp);//*MESH_NUMFACES
	sscanf(line, "%s%d", string, &GeomObjectList[GeomIndex].NUMFACE);
	
	GeomObjectList[GeomIndex].FaceList = new ASE_FACE[GeomObjectList[GeomIndex].NUMFACE];
	GeomObjectList[GeomIndex].MTLIDList = new int[GeomObjectList[GeomIndex].NUMFACE];
			
	////////////////////////////////////////
	
	linecount++;
	fgets(line, 256, fp);//*MESH_VERTEX_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_VERTEX_LIST") == 0 )
		DecodeMESH_VERTEX_LIST(fp);

	linecount++;
	fgets(line, 256, fp);//*MESH_FACE_LIST
	sscanf(line, "%s", string);
	if ( _stricmp(string, "*MESH_FACE_LIST") == 0 )
		DecodeMESH_FACE_LIST(fp);

	//--------------------------------------------------
	//--------------------------------------------------
	while (1)
	{
		linecount++;
		fgets(line, 256, fp);
		sscanf(line, "%s", string);
		if ( _stricmp(string, "*MESH_NUMTVERTEX") == 0 )//*MESH_NUMTVERTEX
		{
			int num = 0;
			sscanf(line, "%s%f", string, &num);
			if (num == 0)
			{
				continue;
			}
			
			GeomObjectList[GeomIndex].TexCoordFlag = true; //////////////////////////////
			
			DecodeMESH_TVERTLIST(fp);
			
			linecount++;
			fgets(line, 256, fp);//*MESH_NUMTVFACES
			DecodeMESH_TFACELIST(fp);
			
			continue;
		}	
		
		if ( _stricmp(string, "*MESH_NUMCVERTEX") == 0 )//*MESH_NUMCVERTEX
		{
			DecodeMESH_CVERTEX(fp);
			continue;
		}
		
		if ( _stricmp(string, "*MESH_NORMALS") == 0 )//*MESH_NORMALS
		{
			GeomObjectList[GeomIndex].NormalFlag = true; ///////////////////////////////

			DecodeMESH_NORMALS(fp);
			continue;
		}

		if ( _stricmp(string, "}") == 0 ) return 0;
	}
	
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int ASEMODEL::DecodeMESH_VERTEX_LIST(FILE *fp)
{
	for (int i = 0; i < GeomObjectList[GeomIndex].NUMVERTEX; i++)
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEX
		sscanf(line, "%s%d%f%f%f", string, string, 
			&GeomObjectList[GeomIndex].VertexList[i].x, 
			&GeomObjectList[GeomIndex].VertexList[i].z, 
			&GeomObjectList[GeomIndex].VertexList[i].y);
	}

	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int ASEMODEL::DecodeMESH_FACE_LIST(FILE *fp)
{
	for (int i = 0; i < GeomObjectList[GeomIndex].NUMFACE; i++)
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_FACE
		sscanf(line, "%s%s%s%d%s%d%s%d%s%d%s%d%s%d%s%d%s%d", 
			string, string, 
			string, &GeomObjectList[GeomIndex].FaceList[i].index[2], 
			string, &GeomObjectList[GeomIndex].FaceList[i].index[1], 
			string, &GeomObjectList[GeomIndex].FaceList[i].index[0],
			string, string,
			string, string,
			string, string,
			string, string,
			string, &GeomObjectList[GeomIndex].MTLIDList[i]);
	}

	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int ASEMODEL::DecodeMESH_TVERTLIST(FILE *fp)
{
	sscanf(line, "%s%d", string, &GeomObjectList[GeomIndex].NUMTVERTEX);
	
	if (GeomObjectList[GeomIndex].NUMTVERTEX == 0) 
	{
		GeomObjectList[GeomIndex].TexCoordFlag = false;
		return 0;
	}

	GeomObjectList[GeomIndex].TVertexList = new Vector3[GeomObjectList[GeomIndex].NUMTVERTEX];

	linecount++;
	fgets(line, 256, fp);//*MESH_TVERTLIST
	
	for (int i = 0; i < GeomObjectList[GeomIndex].NUMTVERTEX; i++)
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_TVERT
		sscanf(line, "%s%d%f%f", string, string, 
			&GeomObjectList[GeomIndex].TVertexList[i].u, 
			&GeomObjectList[GeomIndex].TVertexList[i].v);
		
		GeomObjectList[GeomIndex].TVertexList[i].v
			= 1.0f - GeomObjectList[GeomIndex].TVertexList[i].v;
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int ASEMODEL::DecodeMESH_TFACELIST(FILE *fp)
{
	sscanf(line, "%s%d", string, &GeomObjectList[GeomIndex].NUMTFACE);

	GeomObjectList[GeomIndex].TFaceList = new ASE_FACE[GeomObjectList[GeomIndex].NUMTFACE];

	linecount++;
	fgets(line, 256, fp);//*MESH_TFACELIST

	for (int i = 0; i < GeomObjectList[GeomIndex].NUMTFACE; i++)
	{
		linecount++;
		fgets(line, 256, fp);//*MESH_TFACE
		sscanf(line, "%s%d%d%d%d", string, string, 
			&GeomObjectList[GeomIndex].TFaceList[i].index[2], 
			&GeomObjectList[GeomIndex].TFaceList[i].index[1], 
			&GeomObjectList[GeomIndex].TFaceList[i].index[0]);
	}
	linecount++;
	fgets(line, 256, fp);//}

	return 0;
}

int	ASEMODEL::DecodeMESH_CVERTEX(FILE *fp)
{
	return 0;
}

int ASEMODEL::DecodeMESH_NORMALS(FILE *fp)
{
	float nx, ny, nz;
	
	int i;
    for (i = 0; i < GeomObjectList[GeomIndex].NUMFACE; i++)
	{
		int i0 = GeomObjectList[GeomIndex].FaceList[i].index[0];
		int i1 = GeomObjectList[GeomIndex].FaceList[i].index[1];
		int i2 = GeomObjectList[GeomIndex].FaceList[i].index[2];
		
		linecount++;
		fgets(line, 256, fp);//*MESH_FACENORMAL

		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string, &nx, &ny, &nz);
		//GeomObjectList[GeomIndex].VNormalList[i0] += Vector3(nx, ny, nz) * GeomObjectList[GeomIndex].NODE_TM;
		GeomObjectList[GeomIndex].VNormalList[i0] += Vector3(nx, ny, nz);

		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string, &nx, &ny, &nz);
		//GeomObjectList[GeomIndex].VNormalList[i1] += Vector3(nx, ny, nz) * GeomObjectList[GeomIndex].NODE_TM;
		GeomObjectList[GeomIndex].VNormalList[i1] += Vector3(nx, ny, nz);
		
		linecount++;
		fgets(line, 256, fp);//*MESH_VERTEXNORMAL
		sscanf(line, "%s%d%f%f%f", string, string, &nx, &ny, &nz);
		//GeomObjectList[GeomIndex].VNormalList[i2] += Vector3(nx, ny, nz) * GeomObjectList[GeomIndex].NODE_TM;
		GeomObjectList[GeomIndex].VNormalList[i2] += Vector3(nx, ny, nz);
	}

	for (i = 0; i < GeomObjectList[GeomIndex].NUMVERTEX; i++)
	{
		Vector3 temp;
		
		GeomObjectList[GeomIndex].VNormalList[i] = 
			GeomObjectList[GeomIndex].VNormalList[i] * GeomObjectList[GeomIndex].NODE_TM;

		GeomObjectList[GeomIndex].VNormalList[i].Normalize();
		
		temp.x = GeomObjectList[GeomIndex].VNormalList[i].x;
		temp.y = GeomObjectList[GeomIndex].VNormalList[i].z;
		temp.z = GeomObjectList[GeomIndex].VNormalList[i].y;
		
		GeomObjectList[GeomIndex].VNormalList[i] = temp;
	}
	
	linecount++;
	fgets(line, 256, fp);//}
	
	return 0;
}
		
bool FindOpenBrace(char * line)
{
	if ( strchr(line, '{') )
	{
		BRACE_COUNT++;
		return true;
	}
	return false;
}

bool FindCloseBrace(char * line)
{
	if ( strchr(line, '}') )
	{
		BRACE_COUNT--;
		return true;
	}
	return false;
}

bool FindCloseBraceNoCount(char * line)
{
	if ( strchr(line, '}') )
	{
		return true;
	}
	return false;
}

bool FindBone(char *line)
{
	_strlwr(line);

	if (strstr(line, "bip")) return true;
	if (strstr(line, "bone")) return true;

	return false;
}

