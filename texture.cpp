/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "texture.h"

#include "bmp.h"

int CTexture::Load(char * name)
{
	this->pTex = LoadBMP(name, &TextureWidth, &TextureHeight);

	return 0;
}

int CTexture::Create(int width, int height)
{
	this->pTex = new BYTE[width * height * 4];

	TextureWidth = width;
	TextureHeight = height;
	int WidthBytes = width * 4;
	BYTE *pOffset = pTex;
	BYTE color;
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++)
		{
			//0x001F blue
			//0x03E0 green 
			//0x7C00 red

			if ((i % 64) < 32) 
			{
				if ((j % 64) < 32) color = 255;
				else color = 100;
			}
			else 
			{
				if ((j % 64) < 32) color = 100;
				else color = 255;
			}
			*(pOffset + j*4 + 0) = color; 
			*(pOffset + j*4 + 1) = color; 
			*(pOffset + j*4 + 2) = color; 
			*(pOffset + j*4 + 3) = 255; 
		}
		pOffset += WidthBytes;
	}

	return 0;
}
