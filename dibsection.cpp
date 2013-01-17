/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "dibsection.h"

typedef struct tagBITMAPINFO16{
	BITMAPINFOHEADER bmiHeader;
	DWORD Bitmask[3];
}BITMAPINFO16;

CDIBSection::CDIBSection()
{
	pBits = NULL;
}

CDIBSection::~CDIBSection()
{
	//Destroy();
}

HDC CDIBSection::Create(HDC hScreenDC, int width, int height)
{
	this->hScreenDC = hScreenDC;
	hMemoryDC = CreateCompatibleDC(hScreenDC);

	hDIBitmap = MakeDIBSection(width, height, &pBits);
	hDefaultBitmap = (HBITMAP)SelectObject(hMemoryDC, hDIBitmap);

	Width = width;
	Height = height;
	
	BytesPerScanline = ((Width * 2) + 3) &~3;

	return hMemoryDC;
}

void CDIBSection::Destroy()
{
	SelectObject(hMemoryDC, hDefaultBitmap);
	DeleteObject(hDIBitmap);
	DeleteDC(hMemoryDC);
}

HBITMAP CDIBSection::MakeDIBSection(int width, int height, BYTE **ppBits)
{
	BITMAPINFO16 BMInfo;

	BMInfo.bmiHeader.biSize				= sizeof(BITMAPINFOHEADER);
	BMInfo.bmiHeader.biWidth			= width;
	BMInfo.bmiHeader.biHeight			= -height;
	BMInfo.bmiHeader.biPlanes			= 1;
	BMInfo.bmiHeader.biBitCount			= 16;
	BMInfo.bmiHeader.biCompression		= BI_BITFIELDS;
	BMInfo.bmiHeader.biSizeImage		= 0;
	BMInfo.bmiHeader.biXPelsPerMeter	= 0;
	BMInfo.bmiHeader.biYPelsPerMeter	= 0;
	BMInfo.bmiHeader.biClrUsed			= 0;
	BMInfo.bmiHeader.biClrImportant		= 0;
	BMInfo.Bitmask[0] = 0xF800; // 5 bit red    5 0x7C00
	BMInfo.Bitmask[1] = 0x07E0; // 6 bit green  5 0x03E0
	BMInfo.Bitmask[2] = 0x001F; // 5 bit blue   5 0x001F

	return( CreateDIBSection(hMemoryDC, (BITMAPINFO *)&BMInfo, DIB_RGB_COLORS, (VOID **)ppBits, NULL, 0) );
}

void CDIBSection::Clear(BYTE r, BYTE g, BYTE b)
{
	UINT offset = 0;
	DWORD color565565;
	WORD color565;

	color565 = ((WORD)r) >> 3 << 11;
	color565 |= ((WORD)g) >> 2 << 5;
	color565 |= ((WORD)b) >> 3;

	if (color565 == 0)
	{
		BYTE * pB = pBits;
		int Pitch = BytesPerScanline;
		int H = Height;

#ifdef _MSC_VER
		_asm {
			xor		eax, eax;
			mov		edi, pB;
			mov		ecx, Pitch
			imul	ecx, H
			shr		ecx, 2;
			cld
			rep		stosd;
		}
#else
		asm ("xorl	%eax, %eax");
		asm ("movl	%0, %%edi" : : "m" (pB) );
		asm ("movl	%0, %%ecx" : : "m" (Pitch) ); 
		asm	("imul	%0, %%ecx" : : "m" (H) );
		asm ("shrl	$2, %ecx");
		asm ("cld");
		asm ("rep stosl");
#endif

		return;
	}
	
 	color565565 = color565;
	color565565 = (color565565 << 16) | color565;
	while ( offset < BytesPerScanline )
	{
			*((DWORD*)(pBits + offset)) = color565565;
			offset += 4;
	}
	offset = BytesPerScanline;

	for (int i = 0; i < Height - 1; i++)
	{
		memcpy(pBits + offset, pBits, BytesPerScanline);
		offset += BytesPerScanline;
	}

}

void CDIBSection::Swap()
{
	BitBlt(hScreenDC, 0, 0, Width, Height, hMemoryDC, 0, 0, SRCCOPY);
}
