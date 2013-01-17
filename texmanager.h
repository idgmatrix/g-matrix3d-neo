/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef TEXMANAGER_H
#define TEXMANAGER_H

#include "texture.h"

extern int TEXWIDTH; 
extern int TEXHEIGHT; 
extern BYTE *pTex;
extern int TEXWIDTHBOUND; 
extern int TEXHEIGHTBOUND; 

class CTexManager{
	CTexture *TexList;
	int TexIndex;
	int CurrentTexIndex;
	int NumTexIndex;
	int MaxTexIndex;

public:
	CTexManager()
	{
		TexList = NULL;
		TexIndex = 0;
		CurrentTexIndex = 0;
		NumTexIndex = 0;
		MaxTexIndex = 0;
	}
	~CTexManager()
	{ 
		if (TexList) delete [] TexList;
	}

	void Init(int num);
	void DeInit(void);
	int Load(char * name);
	int Create(int, int);
	int SetTexture(int index)
	{
		if (CurrentTexIndex == index) return CurrentTexIndex;
		if (index < NumTexIndex && index >= 0)
		{	
			CurrentTexIndex = index;

			::pTex = TexList[CurrentTexIndex].pTex;
			::TEXWIDTH = TexList[CurrentTexIndex].TextureWidth;
			::TEXHEIGHT = TexList[CurrentTexIndex].TextureHeight;
			::TEXWIDTHBOUND = TEXWIDTH - 1;
			::TEXHEIGHTBOUND = TEXHEIGHT - 1;
		}
		return CurrentTexIndex;
	}
	BYTE *GetCurrentTexPointer(void)
	{
		return TexList[CurrentTexIndex].pTex;
	}
	int GetCurrentTexWidth(void)
	{
		return TexList[CurrentTexIndex].TextureWidth;
	}
	int GetCurrentTexHeight(void)
	{
		return TexList[CurrentTexIndex].TextureHeight;
	}
};

#endif

