/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef DIBSECTION_H
#define DIBSECTION_H

#include "stdafx.h"

class CDIBSection{
public:	
	BYTE *pBits;
	int Width, Height;
	UINT BytesPerScanline;
	HDC hScreenDC, hMemoryDC;
	HBITMAP hDefaultBitmap, hDIBitmap;	

public:	
	CDIBSection();
	~CDIBSection();
	HDC Create(HDC hScreenDC, int width, int height);
	void Destroy(void);
	void Clear(BYTE r, BYTE g, BYTE b);
	void Swap(void);
private:
	HBITMAP MakeDIBSection(int width, int height, BYTE **ppBits);
};

#endif

