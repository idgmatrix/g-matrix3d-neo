/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "texmanager.h"


void CTexManager::Init(int num = 100)
{
	TexList = new CTexture[num];
	MaxTexIndex = num;
}

void CTexManager::DeInit(void)
{
	if (TexList) delete[] TexList;
	TexList = NULL;
}

int CTexManager::Load(char * name)
{
	if (NumTexIndex == MaxTexIndex) return -1;
	int i = NumTexIndex;
	if (TexList[i].pixeltype == EMPTY)
	{
		TexList[i].pixeltype = RGBA;
		TexList[i].Load(name);
		NumTexIndex = i + 1;
		CurrentTexIndex = i;

		::pTex = TexList[i].pTex;
		::TEXWIDTH = TexList[i].TextureWidth;
		::TEXHEIGHT = TexList[i].TextureHeight;
		::TEXWIDTHBOUND = TEXWIDTH - 1;
		::TEXHEIGHTBOUND = TEXHEIGHT - 1;

		return i;
	}
	return -1;
}

int CTexManager::Create(int w, int h)
{
	if (NumTexIndex == MaxTexIndex) return -1;
	int i = NumTexIndex;
	if (TexList[i].pixeltype == EMPTY)
	{
		TexList[i].pixeltype = RGBA;
		TexList[i].Create(w, h);
		NumTexIndex = i + 1;
		CurrentTexIndex = i;

		::pTex = TexList[i].pTex;
		::TEXWIDTH = TexList[i].TextureWidth;
		::TEXHEIGHT = TexList[i].TextureHeight;
		::TEXWIDTHBOUND = TEXWIDTH - 1;
		::TEXHEIGHTBOUND = TEXHEIGHT - 1;

		return i;
	}
	return -1;
}

