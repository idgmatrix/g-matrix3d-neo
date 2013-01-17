/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "util.h"
#include "transform.h"

void CTransform::RotateX(float theta)
{
	Matrix4 M;
	
	float Sin = sinf(theta);
	float Cos = cosf(theta);

	M._11 = 1;	
	M._12 = 0;	
	M._13 = 0;
	M._14 = 0;

	M._21 = 0;
	M._22 = Cos;
	M._23 = Sin;
	M._24 = 0;

	M._31 = 0;
	M._32 = -Sin;
	M._33 = Cos;
	M._34 = 0;

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;
	M._44 = 1;

	TM[CurrentMode] *= M;
}	

void CTransform::RotateY(float theta)
{
	Matrix4 M;
	
	float Sin = sinf(theta);
	float Cos = cosf(theta);

	M._11 = Cos;
	M._12 = 0;
	M._13 = -Sin;
	M._14 = 0;

	M._21 = 0;
	M._22 = 1;
	M._23 = 0;
	M._24 = 0;

	M._31 = Sin;
	M._32 = 0;
	M._33 = Cos;
	M._34 = 0;

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;
	M._44 = 1;

	TM[CurrentMode] *= M;
}

void CTransform::RotateZ(float theta)
{
	Matrix4 M;
	
	float Sin = sinf(theta);
	float Cos = cosf(theta);

	M._11 = Cos;
	M._12 = Sin;
	M._13 = 0;
	M._14 = 0;

	M._21 = -Sin;
	M._22 = Cos;
	M._23 = 0;
	M._24 = 0;

	M._31 = 0;
	M._32 = 0;
	M._33 = 1;
	M._34 = 0;

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;
	M._44 = 1;

	TM[CurrentMode] *= M;
}

void CTransform::Scale(float x, float y, float z)
{
	Matrix4 M;
	
	M._11 = x;
	M._12 = 0;
	M._13 = 0;
	M._14 = 0;

	M._21 = 0;
	M._22 = y;
	M._23 = 0;
	M._24 = 0;

	M._31 = 0;
	M._32 = 0;
	M._33 = z;
	M._34 = 0;

	M._41 = 0;
	M._42 = 0;
	M._43 = 0;
	M._44 = 1;

	TM[CurrentMode] *= M;
}

void CTransform::Translate(float x, float y, float z)
{
	Matrix4 M;
	
	M._11 = 1;
	M._12 = 0;
	M._13 = 0;
	M._14 = 0;

	M._21 = 0;
	M._22 = 1;
	M._23 = 0;
	M._24 = 0;

	M._31 = 0;
	M._32 = 0;
	M._33 = 1;
	M._34 = 0;

	M._41 = x;
	M._42 = y;
	M._43 = z;
	M._44 = 1;

	TM[CurrentMode] *= M;
}

void CTransform::LoadIndentity(void)
{
	TM[CurrentMode]._11 = 1;
	TM[CurrentMode]._12 = 0;
	TM[CurrentMode]._13 = 0;
	TM[CurrentMode]._14 = 0;

	TM[CurrentMode]._21 = 0;
	TM[CurrentMode]._22 = 1;
	TM[CurrentMode]._23 = 0;
	TM[CurrentMode]._24 = 0;

	TM[CurrentMode]._31 = 0;
	TM[CurrentMode]._32 = 0;
	TM[CurrentMode]._33 = 1;
	TM[CurrentMode]._34 = 0;

	TM[CurrentMode]._41 = 0;
	TM[CurrentMode]._42 = 0;
	TM[CurrentMode]._43 = 0;
	TM[CurrentMode]._44 = 1;
}

void CTransform::SimplePerspective(void)
{
	TM[CurrentMode]._11 = 1;
	TM[CurrentMode]._12 = 0;
	TM[CurrentMode]._13 = 0;
	TM[CurrentMode]._14 = 0;

	TM[CurrentMode]._21 = 0;
	TM[CurrentMode]._22 = 1;
	TM[CurrentMode]._23 = 0;
	TM[CurrentMode]._24 = 0;

	TM[CurrentMode]._31 = 0;
	TM[CurrentMode]._32 = 0;
	TM[CurrentMode]._33 = 1;
	TM[CurrentMode]._34 = 1;

	TM[CurrentMode]._41 = 0;
	TM[CurrentMode]._42 = 0;
	TM[CurrentMode]._43 = 0;
	TM[CurrentMode]._44 = 0;
}

void CTransform::Perspective(float width, float height, float nearz, float farz)
{
	TM[CurrentMode]._11 = 2 * nearz / width;
	TM[CurrentMode]._12 = 0;
	TM[CurrentMode]._13 = 0;
	TM[CurrentMode]._14 = 0;

	TM[CurrentMode]._21 = 0;
	TM[CurrentMode]._22 = 2 * nearz / height;
	TM[CurrentMode]._23 = 0;
	TM[CurrentMode]._24 = 0;

	TM[CurrentMode]._31 = 0;
	TM[CurrentMode]._32 = 0;
	TM[CurrentMode]._33 = 1 / (farz - nearz);
	TM[CurrentMode]._34 = 1;

	TM[CurrentMode]._41 = 0;
	TM[CurrentMode]._42 = 0;
	TM[CurrentMode]._43 = -nearz / (farz - nearz);
	TM[CurrentMode]._44 = 0;
}

void CTransform::PerspectiveFOVW(float fovw, float aspectw, float nearz, float farz)
{
	float dfov = fovw * PI / 180;
	float width = 2 * (nearz * tanf(dfov * 0.5f));
	float height = width * aspectw;

	TM[CurrentMode]._11 = 2 * nearz / width;
	TM[CurrentMode]._12 = 0;
	TM[CurrentMode]._13 = 0;
	TM[CurrentMode]._14 = 0;

	TM[CurrentMode]._21 = 0;
	TM[CurrentMode]._22 = 2 * nearz / height;
	TM[CurrentMode]._23 = 0;
	TM[CurrentMode]._24 = 0;

	TM[CurrentMode]._31 = 0;
	TM[CurrentMode]._32 = 0;
	TM[CurrentMode]._33 = farz / (farz - nearz);
	TM[CurrentMode]._34 = 1;

	TM[CurrentMode]._41 = 0;
	TM[CurrentMode]._42 = 0;
	TM[CurrentMode]._43 = -(nearz * farz) / (farz - nearz);
	TM[CurrentMode]._44 = 0;
}

void CTransform::PerspectiveFOVH(float fovh, float aspecth, float nearz, float farz)
{
	float dfov = fovh * PI / 180;
	float height = 2 * (nearz * tanf(dfov * 0.5f));
	float width = height * aspecth;

	TM[CurrentMode]._11 = 2 * nearz / width;
	TM[CurrentMode]._12 = 0;
	TM[CurrentMode]._13 = 0;
	TM[CurrentMode]._14 = 0;

	TM[CurrentMode]._21 = 0;
	TM[CurrentMode]._22 = 2 * nearz / height;
	TM[CurrentMode]._23 = 0;
	TM[CurrentMode]._24 = 0;

	TM[CurrentMode]._31 = 0;
	TM[CurrentMode]._32 = 0;
	TM[CurrentMode]._33 = farz / (farz - nearz);
	TM[CurrentMode]._34 = 1;

	TM[CurrentMode]._41 = 0;
	TM[CurrentMode]._42 = 0;
	TM[CurrentMode]._43 = -(nearz * farz) / (farz - nearz);
	TM[CurrentMode]._44 = 0;
}

void CTransform::SetTransform(void)
{
	WorldViewTM = TM[MM_WORLD] * TM[MM_VIEW];
	
	WorldViewProjectTM = TM[MM_WORLD] * TM[MM_VIEW] * TM[MM_PROJECT];
	
	NormalTM._11 = WorldViewTM._11;
	NormalTM._12 = WorldViewTM._12;
	NormalTM._13 = WorldViewTM._13;
	NormalTM._21 = WorldViewTM._21;
	NormalTM._22 = WorldViewTM._22;
	NormalTM._23 = WorldViewTM._23;
	NormalTM._31 = WorldViewTM._31;
	NormalTM._32 = WorldViewTM._32;
	NormalTM._33 = WorldViewTM._33;

	NormalTM.Inverse();
	NormalTM.Transpose();

}
