/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "graphics.h"

BYTE *LoadBMP(char *filename, int *pwidth, int *pheight)
{
	FILE *fp;

	fp = fopen(filename, "rb");
	if (!fp)
	{
		MessageBox(NULL, "ERROR", "File open Error", MB_OK);
		return NULL;
	}
	
	BITMAPFILEHEADER BMPFH;
	size_t size = fread(&BMPFH, 1, sizeof(BITMAPFILEHEADER), fp);
	if (size != sizeof(BITMAPFILEHEADER))
	{
		MessageBox(NULL, "ERROR", "File header Read Error", MB_OK);
		return NULL;
	}

	BITMAPINFOHEADER BMPIH;
	size = fread(&BMPIH, 1, sizeof(BITMAPINFOHEADER), fp);
	if (size != sizeof(BITMAPINFOHEADER))
	{
		MessageBox(NULL, "ERROR", "Info header Read Error", MB_OK);
		return NULL;
	}

	size_t ImageSize = BMPIH.biWidth * BMPIH.biHeight * 3;
	BYTE *pBMP = new BYTE[ImageSize];
	if (!pBMP)
	{
		MessageBox(NULL, "ERROR", "Image Memory Error", MB_OK);
		return NULL;
	}
	
	*pwidth = BMPIH.biWidth;
	*pheight = BMPIH.biHeight;
	size = fread(pBMP, 1, ImageSize, fp);
	if (size != ImageSize)
	{
		MessageBox(NULL, "ERROR", "Image Read Error", MB_OK);
		return NULL;
	}

	fclose(fp);
	
	/////////////////////////////////////////////////////////////////
	// Convert 24bit RGB -> 32bit ARGB
	int BytesPerScanline = ((BMPIH.biWidth * 3) + 3) &~3;
	int BytesPerScanline32 = BMPIH.biWidth * 4;
	BYTE *pBMP32 = new BYTE[BMPIH.biWidth * BMPIH.biHeight * 4];
	if (!pBMP32)
	{
		MessageBox(NULL, "ERROR", "Image Memory Error", MB_OK);
		return NULL;
	}
	for (int j = 0; j < BMPIH.biHeight; j++)
	{
		for (int i = 0; i < BMPIH.biWidth; i++)
		{
			int jj = BMPIH.biHeight - j - 1; 
			pBMP32[i * 4 + j * BytesPerScanline32 + 0] = pBMP[i * 3 + jj * BytesPerScanline + 0];
			pBMP32[i * 4 + j * BytesPerScanline32 + 1] = pBMP[i * 3 + jj * BytesPerScanline + 1];
			pBMP32[i * 4 + j * BytesPerScanline32 + 2] = pBMP[i * 3 + jj * BytesPerScanline + 2];
			pBMP32[i * 4 + j * BytesPerScanline32 + 3] = 255;
		}
	}
	
	delete[] pBMP;

	return pBMP32;
}

void DrawBMP(int x, int y, BYTE *pBMP)
{
	BYTE *Soff;
	BYTE *Doff;

	int BMPWidth = *((WORD*)pBMP);
	int BMPHeight = *((WORD*)pBMP + 1);
	int BMPscanline = (BMPWidth * 2 + 3) & ~3;
	Soff = pBMP + 4;
	Doff = FrameBuffer.pBits + y * FrameBuffer.BytesPerScanline + x * 2;

	WORD color565;
	for (int j = 0; j < BMPHeight; j++)
	{
		for (int i = 0, k = 0; i < BMPscanline; i+=2, k+=2)
		{
			WORD R = (WORD)*(Soff + i);
			WORD G = (WORD)*(Soff + i + 1);
			WORD B = (WORD)*(Soff + i + 2);
			
			color565  = R >> 16 >> 3 << 11;
			color565 |= G >> 16 >> 2 << 5;
			color565 |= B >> 16 >> 3;
			*(WORD*)(Doff + k) = color565;
			//*(WORD*)(Doff + k) = *(WORD*)(Soff + i);
		
		}
		Soff += BMPscanline;
		Doff += FrameBuffer.BytesPerScanline;
	}
}

