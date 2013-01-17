/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef TEXTURE_H
#define TEXTURE_H

#include "stdafx.h"

typedef enum _PIXELTYPE {
	EMPTY = 0,
	RGB = 1,
	RGBA = 2,
	FORCE_DWORD = 0x7fffffff
}PIXELTYPE;

class CTexture{
public:
	CTexture()
	{
		pTex = NULL;
		pixeltype = EMPTY;
	}
	~CTexture()
	{
		if (pTex) delete[] pTex;
	}

	int Load(char * name);
	int Create(int width, int height);
	
	BYTE *pTex;
	int TextureWidth, TextureHeight;
	PIXELTYPE pixeltype;
};

#endif

