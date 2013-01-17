/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"

typedef enum _MATRIXMODETYPE {
	MM_WORLD = 0,
	MM_VIEW = 1,
	MM_PROJECT = 2,
	MM_VIEWPORT = 3,
	MM_FORCE_DWORD = 0x7fffffff
}MATRIXMODETYPE;

//#define PI 3.1415926535f

class CTransform {
	MATRIXMODETYPE CurrentMode;
	Matrix4 WorldTM;
	Matrix4 ViewTM;
	Matrix4 ProjectTM;
	Matrix4 ViewportTM;
	
	Matrix4 TM[4];
	Matrix3 NormalTM;

	Matrix4 WorldViewTM;
	Matrix4 WorldViewProjectTM;
	
	friend class CRenBuffer;

public:
	void LoadMatrix(const Matrix4 & M)
	{
		TM[CurrentMode] = M;
	}

	void SetMatrixMode(const MATRIXMODETYPE i)
	{
		CurrentMode = i;
	}

	void RotateX(float theta);
	void RotateY(float theta);
	void RotateZ(float theta);
	void Scale(float x, float y, float z);
	void Translate(float x, float y, float z);
	void LoadIndentity(void);
	void SimplePerspective(void);
	void Perspective(float width, float height, float nearz, float farz);
	void PerspectiveFOVW(float fovw, float aspectw, float nearz, float farz);
	void PerspectiveFOVH(float fovh, float aspecth, float nearz, float farz);
	void SetTransform(void);

//private:
//	void CalcNormalTM(void){};

};

#endif

