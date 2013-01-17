/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "graphics.h"
#include "vertex.h"

#include "util.h"

#ifdef _MSC_VER
#pragma warning (disable: 4244)
#endif

#define DRAW_MODE 0
#define DEPTH_SHIFT 5    //Minimum Z value 128

int TEXWIDTH; 
int TEXHEIGHT; 
BYTE *pTex;
int TEXWIDTHBOUND;
int TEXHEIGHTBOUND;

struct {
	int lx, rx, ty, by;
//}Bound = {100, 539, 100, 379};
}Bound = {0, 639, 0, 479};

struct {
BYTE r, g, b, a;
}color;

void PutPixel(int x, int y);
void DrawHLine(int x1, int x2, int y);
void DrawVLine(int x, int y1, int y2);
void DrawBox(int x1, int y1, int x2, int y2);
void FillBox(int x1, int y1, int x2, int y2);

void DrawLine(int x1, int y1, int x2, int y2);

void DrawTriangle(TLVertex * tri, int*);
void DrawTrianglePers(TLVertex * tri, int*);
void DrawTrianglePers16(TLVertex * tri, int*);
void DrawTriangleWire(TLVertex * tri, int*);

void SetColor(BYTE r, BYTE g, BYTE b)
{
	color.r = r;
	color.g = g;	
	color.b = b;
}

void PutPixel(int x, int y)
{
	UINT offset;
	WORD color565;

	offset = BytesPerScanline * y + x * 2;
	if ( offset > BytesPerScanline * 479 + 639 * 2 ) return;
	if ( x > Bound.rx || x < Bound.lx ) return;
	
	color565 = color.r >> 3 << 11;
	color565 |= color.g >> 2 << 5;
	color565 |= color.b >> 3;
	*(WORD*)(pBits + offset) = color565;
}

void DrawHLine(int x1, int x2, int y) 
{
	UINT offset;
	WORD color565;

	offset = BytesPerScanline * y + x1 * 2;

	for(int i = x1; i <= x2; i++)
	{
		color565 = color.r >> 3 << 11;
		color565 |= color.g >> 2 << 5;
		color565 |= color.b >> 3;
		*(WORD*)(pBits + offset) = color565;
		offset += 2;
	}
}

void DrawVLine(int x, int y1, int y2)
{
	UINT offset, offsetend;
	WORD color565;

	offset = BytesPerScanline * y1 + x * 2;
	offsetend = BytesPerScanline * y2 + x * 2;

	while(offset <= offsetend) 
	{
		color565  = color.r >> 3 << 11;
		color565 |= color.g >> 2 << 5;
		color565 |= color.b >> 3;
		*(WORD*)(pBits + offset) = color565;
		offset += BytesPerScanline;
	}
}

void DrawBox(int x1, int y1, int x2, int y2)
{
	DrawVLine(x1, y1, y2);
	DrawVLine(x2, y1, y2);
	DrawHLine(x1, x2, y1);
	DrawHLine(x1, x2, y2);
}

void FillBox(int x1, int y1, int x2, int y2)
{
	UINT offset;
	WORD color565;
	
	offset = BytesPerScanline * y1 +  x1 * 2;
	UINT nextline = BytesPerScanline - (x2 - x1) * 2 - 2;

	int i, j;
 	for(i = y1; i <= y2; i++)
	{
		for(j = x1; j <= x2; j++)
		{
			color565  = color.r >> 3 << 11;
			color565 |= color.g >> 2 << 5;
			color565 |= color.b >> 3;
			*(WORD*)(pBits + offset) = color565;
			offset += 2;
		}
		offset += nextline;
	}
}

void DrawLine(int x1, int y1, int x2, int y2)
{
	int y, yd;
	int temp, i;
	int delta_x, delta_y;

	if ( (x1 == x2) && (y1 == y2) ) return;

	if ( x1 > x2 )
	{
		temp = x1;
		x1 = x2;
		x2 = temp;

		temp = y1;
		y1 = y2;
		y2 = temp;
	}

	delta_x = x2 - x1;
	delta_y = y2 - y1;

	if ( delta_x < abs(delta_y) )
	{
		temp = x1;
		x1 = y1;
		y1 = temp;

		temp = x2;
		x2 = y2;
		y2 = temp;

		if ( x1 > x2 )
		{
			temp = x1;
			x1 = x2;
			x2 = temp;

			temp = y1;
			y1 = y2;
			y2 = temp;
		}

		delta_x = x2 - x1;
		delta_y = y2 - y1;

		y = y1 << 16;

		delta_y <<= 16;
		yd = delta_y / delta_x;

		for ( i = x1; i <= x2; i++ )
		{
			PutPixel(y>>16, i);
			y += yd;
		}
	}
	else
	{
		y = y1 << 16;

		delta_y <<= 16;
		yd = delta_y / delta_x;

		for ( i = x1; i <= x2; i++ )
		{
			PutPixel(i, y>>16);
			y += yd;
		}
	}
}


// rasterizer for test 
void DrawScanLine(float xf1, float xf2, int y) 
{
	UINT offset;

	int x1, x2;
	int i;

 	x1 = ftoi(xf1);
	x2 = ftoi(xf2);
	
	if ( x1 == x2 ) return;
	if (x1 < Bound.lx) x1 = Bound.lx;
	if (x2 > Bound.rx) x2 = Bound.rx + 1;

	offset = BytesPerScanline * y + x1 * 2;
	//BYTE *pOffset = pBits + BytesPerScanline * y + x1 * 2;

	WORD color565 = 0x07E0;
	for(i = x1; i < x2; i++)
	{
		
		color565 = *((WORD*)(pBits + offset + 0));
		
		if (color565 != 0)
		{
		*((WORD*)(pBits + offset + 0)) = 0xF800;  // red
		}
		else
		{
		*((WORD*)(pBits + offset + 0)) = 0x07E0; // green
		//*((WORD*)pOffset) = color565; // green
		}
		offset += 2;
	}
}

//////////////////////////////////////
// color interpolation
// Gouraud shading
void DrawScanLine(float xf1, float xf2, int y, Vector3 &cL, Vector3 &cR) 
{
	struct Color3i{
		int r, g, b;
	};
	//UINT offset;
	Color3i cdi, cLLi;
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);

	if ( x1 == x2 ) return;
	
	Vector3 cd = (cR - cL) / (x2 - x1);
	cdi.r = ftoi(cd.r * 255 * 65536);
	cdi.g = ftoi(cd.g * 255 * 65536);
	cdi.b = ftoi(cd.b * 255 * 65536);
	Vector3 cLL = cL;
	if (x1 < Bound.lx) 
	{
		cLL += cd * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1;

	cLLi.r = ftoi(cLL.r * 255 * 65536);
	cLLi.g = ftoi(cLL.g * 255 * 65536);
	cLLi.b = ftoi(cLL.b * 255 * 65536);

	BYTE *pOffset = pBits + BytesPerScanline * y + x1 * 2;

	WORD color565;
	for(int i = x1; i < x2; i++)
	{
		color565  = cLLi.r >> 16 >> 3 << 11;
		color565 |= cLLi.g >> 16 >> 2 << 5;
		color565 |= cLLi.b >> 16 >> 3;
		
		*((WORD*)pOffset) = color565;
		pOffset += 2;
		cLLi.r += cdi.r;
		cLLi.g += cdi.g;
		cLLi.b += cdi.b;
	}
}

///////////////////////////////////////////////
// linear texture mapping
void DrawScanLine(float xf1, float xf2, int y, Vector2 &tL, Vector2 &tR) 
{
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);
	
	if ( x1 == x2 ) return;
	
	// calculate gradient
	Vector2 td = (tR - tL) / (xf2 - xf1);
	
	// sub texel accuracy
	Vector2 tLL = tL + td * (x1 - xf1);
	
	// clipping
	if (x1 < Bound.lx)   // left clip
	{
		tLL += td * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1; // right clip

	UINT offset = BytesPerScanline * y + x1 * 2;
	WORD color565;
	for(int i = x1; i < x2; i++)
	{
		//UINT toffset = (ftoi(tLL.v * 255 - 0.5f) << 8) * 3 + ftoi(tLL.u * 255 - 0.5f) * 3;
		//UINT toffset = (ftoi(tLL.v - 0.5f) << 8) * 3 + ftoi(tLL.u - 0.5f) * 3;
		UINT toffset = ( ((ftoi(tLL.v - 0.5f)&TEXHEIGHTBOUND) * TEXWIDTH) + (ftoi(tLL.u - 0.5f)&TEXWIDTHBOUND) ) * 4;
		
		color565  = (WORD)(*(pTex + toffset + 0)) >> 3;
		color565 |= (WORD)(*(pTex + toffset + 1)) >> 2 << 5;
		color565 |= (WORD)(*(pTex + toffset + 2)) >> 3 << 11;

		*((WORD*)(pBits + offset)) = color565;

		offset += 2;
		tLL += td;
	}

}

// Linear texture & Gouraud shading
void DrawScanLine(float xf1, float xf2, int y, Vector3 &cL, Vector3 &cR, Vector2 &tL, Vector2 &tR) 
{
	struct Color3i{
		int r, g, b;
	} cdi, cLLi;
	
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);

	if ( x1 >= x2 ) return;
	
	// color gradient
	Vector3 cd = (cR - cL) / (x2 - x1);
	cdi.r = ftoi(cd.r * 255 * 65536);
	cdi.g = ftoi(cd.g * 255 * 65536);
	cdi.b = ftoi(cd.b * 255 * 65536);
	
	Vector3 cLL = cL;
	if (x1 < Bound.lx) 
	{
		cLL += cd * (Bound.lx - x1);
		x1 = Bound.lx;
	}

	cLLi.r = ftoi(cLL.r * 255 * 65536);
	cLLi.g = ftoi(cLL.g * 255 * 65536);
	cLLi.b = ftoi(cLL.b * 255 * 65536);

	// calculate gradient
	Vector2 td = (tR - tL) / (xf2 - xf1);
	
	// sub-texel accuracy
	Vector2 tLL = tL + td * (x1 - xf1);
	
	// clipping
	if (x1 < Bound.lx)   // left clip
	{
		tLL += td * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1; // right clip

	int itLLu = ftoi(tLL.u * 65536.0f);
	int itLLv = ftoi(tLL.v * 65536.0f);
	int itdu = ftoi(td.u * 65536.0f);
	int itdv = ftoi(td.v * 65536.0f);
	int half = 32767;
	UINT offset = BytesPerScanline * y + x1 * 2;
	WORD color565;
	for(int i = x1; i < x2; i++)
	{
		//UINT toffset = ((ftoi(tLL.v - 0.5f) << 8) + ftoi(tLL.u - 0.5f)) * 3;
		UINT toffset = ( ((((itLLv + half)>>16)&TEXHEIGHTBOUND) * TEXWIDTH) + (((itLLu + half)>>16)&TEXWIDTHBOUND) ) * 3;
		//UINT toffset = ( (((itLLv + half) >> 16) << 8 ) + ((itLLu + half) >> 16) ) * 3;
		
		WORD tR = (WORD)(*(pTex + toffset + 2));
		WORD tG = (WORD)(*(pTex + toffset + 1));
		WORD tB = (WORD)(*(pTex + toffset + 0));

		color565  = ((cLLi.r>>16) * tR) >> 8 >> 3 << 11;
		color565 |= ((cLLi.g>>16) * tG) >> 8 >> 2 << 5;
		color565 |= ((cLLi.b>>16) * tB) >> 8 >> 3;

		*((WORD*)(pBits + offset)) = color565;
		offset += 2;
		cLLi.r += cdi.r;
		cLLi.g += cdi.g;
		cLLi.b += cdi.b;
		//tLL += td;
		itLLu += itdu;
		itLLv += itdv;
	}
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Perspective texture & Gouraud shading with Z-buffering
void DrawScanLinePers(float xf1, float xf2, int y, Vector3 &cL, Vector3 &cR, Vector3 &tL, Vector3 &tR) 
{
	struct Color3i{
		int r, g, b;
	} cdi, cLLi;
	
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);

	if ( x1 >= x2 ) return;
	
	// color gradient
	Vector3 cd = (cR - cL) / (x2 - x1);
	cdi.r = ftoi(cd.r * 255 * 65536);
	cdi.g = ftoi(cd.g * 255 * 65536);
	cdi.b = ftoi(cd.b * 255 * 65536);
	
	Vector3 cLL = cL;
	if (x1 < Bound.lx) 
	{
		cLL += cd * (Bound.lx - x1);
		x1 = Bound.lx;
	}

	cLLi.r = ftoi(cLL.r * 255 * 65536);
	cLLi.g = ftoi(cLL.g * 255 * 65536);
	cLLi.b = ftoi(cLL.b * 255 * 65536);

	// calculate gradient
	Vector3 td = (tR - tL) / (xf2 - xf1);
	
	// sub-texel accuracy
	Vector3 tLL = tL + td * (x1 - xf1);
	
	// clipping
	if (x1 < Bound.lx)   // left clip
	{
		tLL += td * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1; // right clip

	INT tu = ftoi(tLL.u *   4194304); // << 22   10.22
	INT tv = ftoi(tLL.v *   4194304);
	INT tq = ftoi(tLL.q * 268435456); // << 28    4.28
	INT tdu = ftoi(td.u *   4194304);
	INT tdv = ftoi(td.v *   4194304);
	INT tdq = ftoi(td.q * 268435456);
	INT half = 33554431; // << 25
	UINT offset = BytesPerScanline * y + x1 * 2;
	WORD color565;
	for(int i = x1; i < x2; i++)
	{
		WORD qq = (tq >> DEPTH_SHIFT);
		WORD *pDB = (WORD*)(pDepthBuffer + offset);
		if (*pDB < qq)
		{
			//UINT toffset = ( (((itLLv + half) >> 16) * 256) + ((itLLu + half) >> 16) ) * 3;

			//float w = 1 / tLL.q;
			//UINT toffset = ((ftoi(tLL.v * w - 0.5f) << 8) + ftoi(tLL.u * w - 0.5f)) * 4;
		
			//UINT toffset = ( ( ( (tv << 6) / tq ) * 256/*TEXWIDTH*/ ) + ((tu << 6) / tq) ) * 4;

			INT invq;			// = 0xffffffff / tq;
			INT uoff;			// = (tu * w) >> 28;
			INT voff;			// = (tv * w) >> 28;
#ifdef _MSC_VER
 			_asm mov	ebx, tq
 			_asm mov	edx, 1
 			_asm xor	eax, eax
 			_asm idiv	ebx
		
		 	_asm mov	invq, eax
		 	_asm mov	ebx, tu
		 	_asm imul	ebx
		 	_asm shld	edx, eax, 6
		 	_asm mov	uoff, edx
			
			_asm mov	eax, invq
			_asm mov	ebx, tv
			_asm imul	ebx
			_asm shld	edx, eax, 6
			_asm mov	voff, edx
#else
 			asm ("movl	%0, %%ebx" : : "m" (tq));
 			asm ("movl	$1, %edx");
 			asm ("xorl	%eax, %eax");
 			asm ("idivl	%ebx");
		
		 	asm ("movl	%%eax, %0" : : "m" (invq));
			asm ("movl	%0, %%ebx" : : "m" (tu));
			asm ("imull	%ebx");
			asm ("shldl	$6, %eax, %edx"); 
			asm ("movl	%%edx, %0" : : "m" (uoff));
			
			asm ("movl	%0, %%eax" : : "m" (invq));
			asm ("movl	%0, %%ebx" : : "m" (tv));
			asm ("imull	%ebx");
			asm ("shldl	$6, %eax, %edx");
			asm ("movl	%%edx, %0" : : "m" (voff));
#endif

			//UINT uoff = (tu * w + half) >> 26;
			//UINT voff = (tv * w + half) >> 26;
			
			UINT toffset = ((voff & TEXHEIGHTBOUND) * TEXWIDTH + (uoff & TEXWIDTHBOUND)) * 4;
		
			DWORD tcolor32 = *((DWORD*)(pTex + toffset));
			WORD tB = tcolor32 & 0x000000FF;
			WORD tG = (tcolor32 & 0x0000FF00) >> 8;
			WORD tR = (tcolor32 & 0x00FF0000) >> 16;
			
			//WORD tR = (WORD)(*(pTex + toffset + 2));
			//WORD tG = (WORD)(*(pTex + toffset + 1));
			//WORD tB = (WORD)(*(pTex + toffset + 0));
			
			color565  = ((cLLi.r>>16) * tR) >> 8 >> 3 << 11;
			color565 |= ((cLLi.g>>16) * tG) >> 8 >> 2 << 5;
			color565 |= ((cLLi.b>>16) * tB) >> 8 >> 3;

			*((WORD*)(pBits + offset)) = color565;
			*pDB = qq;
		}
		offset += 2;
		cLLi.r += cdi.r;
		cLLi.g += cdi.g;
		cLLi.b += cdi.b;
		tu += tdu;
		tv += tdv;
		tq += tdq;
	}
}

////////////////////////////////////////////////////////////////////////////
// Perspective texture 16 & Gouraud shading with Z-buffering
void DrawScanLinePers16(float xf1, float xf2, int y, Vector3 &cL, Vector3 &cR, Vector3 &tL, Vector3 &tR) 
{
	struct Color3i{
		int r, g, b;
	} cdi, cLLi;
	
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);

	if ( x1 >= x2 ) return;
	
	// color gradient
	Vector3 cd = (cR - cL) / (x2 - x1);
	cdi.r = ftoi(cd.r * 255 * 65536);
	cdi.g = ftoi(cd.g * 255 * 65536);
	cdi.b = ftoi(cd.b * 255 * 65536);
	
	Vector3 cLL = cL;
	if (x1 < Bound.lx) 
	{
		cLL += cd * (Bound.lx - x1);
		x1 = Bound.lx;
	}

	cLLi.r = ftoi(cLL.r * 255 * 65536);
	cLLi.g = ftoi(cLL.g * 255 * 65536);
	cLLi.b = ftoi(cLL.b * 255 * 65536);

	// calculate gradient
	Vector3 td = (tR - tL) / (xf2 - xf1);
	
	// sub-texel accuracy
	Vector3 tLL = tL + td * (x1 - xf1);
	
	// clipping
	if (x1 < Bound.lx)   // left clip
	{
		tLL += td * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1; // right clip

	INT tu = ftoi(tLL.u *   4194304); // << 22   10.22
	INT tv = ftoi(tLL.v *   4194304);
	INT tq = ftoi(tLL.q * 268435456); // << 28    4.28
	INT tdu = ftoi(td.u *   4194304); // << 22   10.22
	INT tdv = ftoi(td.v *   4194304);
	INT tdq = ftoi(td.q * 268435456);
	INT half = 2097151;					// << 22
	INT offset = BytesPerScanline * y + x1 * 2;
	WORD color565;
	int i;
    for(i = x1; i < (x2-16); i += 16)
	{
		INT invq;			// = 0x100000000 / tq;
		INT uoff;			// = (tu * w) >> 28;
		INT voff;			// = (tv * w) >> 28;

#ifdef _MSC_VER
 		_asm mov	ebx, tq
		_asm mov	edx, 1
		_asm xor	eax, eax
		_asm idiv	ebx
		
		_asm mov	invq, eax
		_asm mov	ebx, tu
		_asm imul	ebx
		_asm shld	edx, eax, 6 + 22
		_asm mov	uoff, edx
			
		_asm mov	eax, invq
		_asm mov	ebx, tv
		_asm imul	ebx
		_asm shld	edx, eax, 6 + 22
		_asm mov	voff, edx
#else
 		asm ("movl	%0, %%ebx" : : "m" (tq));
		asm ("movl	$1, %edx");
		asm ("xorl	%eax, %eax");
		asm ("idivl	%ebx");
		
		asm ("movl	%%eax, %0" : : "m" (invq));
		asm ("movl	%0, %%ebx" : : "m" (tu));
		asm ("imull	%ebx");
		asm ("shldl	$28, %eax, %edx"); 
		asm ("movl	%%edx, %0" : : "m" (uoff));
			
		asm ("movl	%0, %%eax" : : "m" (invq));
		asm ("movl	%0, %%ebx" : : "m" (tv));
		asm ("imull	%ebx");
		asm ("shldl	$28, %eax, %edx");
		asm ("movl	%%edx, %0" : : "m" (voff));
#endif
		INT invq1;			
		INT uoff1;			
		INT voff1;			

#ifdef _MSC_VER
		_asm mov	ebx, tq
		_asm mov	eax, tdq
		_asm shl	eax, 4
		_asm add	ebx, eax
		_asm mov	edx, 1
		_asm xor	eax, eax
		_asm idiv	ebx
		
		_asm mov	invq1, eax
		_asm mov	ebx, tu
		_asm mov	edx, tdu
		_asm shl	edx, 4
		_asm add	ebx, edx
		_asm imul	ebx
		_asm shld	edx, eax, 6 + 22
		_asm mov	uoff1, edx
			
		_asm mov	eax, invq1
		_asm mov	ebx, tv
		_asm mov	edx, tdv
		_asm shl	edx, 4
		_asm add	ebx, edx
		_asm imul	ebx
		_asm shld	edx, eax, 6 + 22
		_asm mov	voff1, edx
#else
 		asm ("movl	%0, %%ebx" : : "m" (tq));
		asm ("movl	%0, %%eax" : : "m" (tdq));
		asm ("shll	$4, %eax");
		asm ("addl	%eax, %ebx");
		asm ("movl	$1, %edx");
		asm ("xorl	%eax, %eax");
		asm ("idivl	%ebx");
		
		asm ("movl	%%eax, %0" : : "m" (invq1));
		asm ("movl	%0, %%ebx" : : "m" (tu));
		asm ("movl	%0, %%edx" : : "m" (tdu));
		asm ("shll	$4, %edx");
		asm ("addl	%edx, %ebx");
		asm ("imull	%ebx");
		asm ("shldl	$28, %eax, %edx"); 
		asm ("movl	%%edx, %0" : : "m" (uoff1));
			
		asm ("movl	%0, %%eax" : : "m" (invq1));
		asm ("movl	%0, %%ebx" : : "m" (tv));
		asm ("movl	%0, %%edx" : : "m" (tdv));
		asm ("shll	$4, %edx");
		asm ("addl	%edx, %ebx");
		asm ("imull	%ebx");
		asm ("shldl	$28, %eax, %edx");
		asm ("movl	%%edx, %0" : : "m" (voff1));
#endif

		int itdu = (uoff1 - uoff) / 16;
		int itdv = (voff1 - voff) / 16;

		for (int j = 0; j < 16; j++)
		{
			WORD qq = tq >> DEPTH_SHIFT;
			WORD *pDB = (WORD*)(pDepthBuffer + offset);
			if (*pDB < qq)
			{
				UINT toffset = ( (((voff + half)>>22)&TEXHEIGHTBOUND) * TEXWIDTH + ( ((uoff + half)>>22)&TEXWIDTHBOUND) ) * 4;

				DWORD tcolor32 = *((DWORD*)(pTex + toffset));
				WORD tB = tcolor32 & 0x000000FF;
				WORD tG = (tcolor32 & 0x0000FF00) >> 8;
				WORD tR = (tcolor32 & 0x00FF0000) >> 16;
			
				//WORD tR = (WORD)(*(pTex + toffset + 2));
				//WORD tG = (WORD)(*(pTex + toffset + 1));
				//WORD tB = (WORD)(*(pTex + toffset + 0));
			
				color565  = ((cLLi.r>>16) * tR) >> 8 >> 3 << 11;
				color565 |= ((cLLi.g>>16) * tG) >> 8 >> 2 << 5;
				color565 |= ((cLLi.b>>16) * tB) >> 8 >> 3;

				*((WORD*)(pBits + offset)) = color565;
				*pDB = qq;
			}
			offset += 2;
			cLLi.r += cdi.r;
			cLLi.g += cdi.g;
			cLLi.b += cdi.b;
			uoff += itdu;
			voff += itdv;
			//tu += tdu;
			//tv += tdv;
			tq += tdq;
		}
		tu += tdu * 16;
		tv += tdv * 16;
	}

	for (int j = i; j < x2; j++)
	{
		WORD qq = tq >> DEPTH_SHIFT;
		WORD *pDB = (WORD*)(pDepthBuffer + offset);
		if (*pDB < qq)
		{
			INT invq;			// = 0x100000000 / tq;
			INT uoff;			// = (tu * w) >> 28;
			INT voff;			// = (tv * w) >> 28;
#ifdef _MSC_VER
			_asm mov	ebx, tq
			_asm mov	edx, 1
			_asm xor	eax, eax
			_asm idiv	ebx
		
			_asm mov	invq, eax
			_asm mov	ebx, tu
			_asm imul	ebx
			_asm shld	edx, eax, 6 + 22
			_asm mov	uoff, edx
			
			_asm mov	eax, invq
			_asm mov	ebx, tv
			_asm imul	ebx
			_asm shld	edx, eax, 6 + 22
			_asm mov	voff, edx
#else
 			asm ("movl	%0, %%ebx" : : "m" (tq));
			asm ("movl	$1, %edx");
			asm ("xorl	%eax, %eax");
			asm ("idivl	%ebx");
		
			asm ("movl	%%eax, %0" : : "m" (invq));
			asm ("movl	%0, %%ebx" : : "m" (tu));
			asm ("imull	%ebx");
			asm ("shldl	$28, %eax, %edx"); 
			asm ("movl	%%edx, %0" : : "m" (uoff));
			
			asm ("movl	%0, %%eax" : : "m" (invq));
			asm ("movl	%0, %%ebx" : : "m" (tv));
			asm ("imull	%ebx");
			asm ("shldl	$28, %eax, %edx");
			asm ("movl	%%edx, %0" : : "m" (voff));
#endif
			UINT toffset = ( (((voff + half)>>22)&TEXHEIGHTBOUND ) * TEXWIDTH + (((uoff + half)>>22)&TEXWIDTHBOUND) ) * 4;

			DWORD tcolor32 = *((DWORD*)(pTex + toffset));
			WORD tB = tcolor32 & 0x000000FF;
			WORD tG = (tcolor32 & 0x0000FF00) >> 8;
			WORD tR = (tcolor32 & 0x00FF0000) >> 16;
			
			//WORD tR = (WORD)(*(pTex + toffset + 2));
			//WORD tG = (WORD)(*(pTex + toffset + 1));
			//WORD tB = (WORD)(*(pTex + toffset + 0));
			
			color565  = ((cLLi.r>>16) * tR) >> 8 >> 3 << 11;
			color565 |= ((cLLi.g>>16) * tG) >> 8 >> 2 << 5;
			color565 |= ((cLLi.b>>16) * tB) >> 8 >> 3;

			*((WORD*)(pBits + offset)) = color565;
			*pDB = qq;
		}
		offset += 2;
		cLLi.r += cdi.r;
		cLLi.g += cdi.g;
		cLLi.b += cdi.b;
		tu += tdu;
		tv += tdv;
		tq += tdq;
	}

}

////////////////////////////////////////////////////////////////////////////////
// (WireFrame) Perspective texture 16 & Gouraud shading  with Z-buffering 
void DrawScanLineWire(float xf1, float xf2, int y, Vector3 &cL, Vector3 &cR, Vector3 &tL, Vector3 &tR) 
{
	struct Color3i{
		int r, g, b;
	} cdi, cLLi;
	
	int x1, x2;

	x1 = ftoi(xf1);
	x2 = ftoi(xf2);

	if ( x1 >= x2 ) return;
	
	// color gradient
	Vector3 cd = (cR - cL) / (x2 - x1);
	cdi.r = ftoi(cd.r * 255 * 65536);
	cdi.g = ftoi(cd.g * 255 * 65536);
	cdi.b = ftoi(cd.b * 255 * 65536);
	
	Vector3 cLL = cL;
	if (x1 < Bound.lx) 
	{
		cLL += cd * (Bound.lx - x1);
		x1 = Bound.lx;
	}

	cLLi.r = ftoi(cLL.r * 255 * 65536);
	cLLi.g = ftoi(cLL.g * 255 * 65536);
	cLLi.b = ftoi(cLL.b * 255 * 65536);

	// calculate gradient
	Vector3 td = (tR - tL) / (xf2 - xf1);
	
	// sub-texel accuracy
	Vector3 tLL = tL + td * (x1 - xf1);
	
	// clipping
	if (x1 < Bound.lx)   // left clip
	{
		tLL += td * (Bound.lx - x1);
		x1 = Bound.lx;
	}
	if (x2 > Bound.rx) x2 = Bound.rx + 1; // right clip

	INT tu = ftoi(tLL.u *   4194304); // << 22   10.22
	INT tv = ftoi(tLL.v *   4194304);
	INT tq = ftoi(tLL.q * 268435456); // << 28    4.28
	INT tdu = ftoi(td.u *   4194304); // << 22   10.22
	INT tdv = ftoi(td.v *   4194304);
	INT tdq = ftoi(td.q * 268435456);
	INT half = 2097151;					// << 22
	INT offset = BytesPerScanline * y + x1 * 2;
	WORD color565;

	INT invq;			// = 0x100000000 / tq;
	INT uoff;			// = (tu * w) >> 28;
	INT voff;			// = (tv * w) >> 28;
	
#ifdef _MSC_VER
 	_asm mov	ebx, tq
	_asm mov	edx, 1
	_asm xor	eax, eax
	_asm idiv	ebx
		
	_asm mov	invq, eax
	_asm mov	ebx, tu
	_asm imul	ebx
	_asm shld	edx, eax, 6 + 22
	_asm mov	uoff, edx
			
	_asm mov	eax, invq
	_asm mov	ebx, tv
	_asm imul	ebx
	_asm shld	edx, eax, 6 + 22
	_asm mov	voff, edx
#else
 	asm ("movl	%0, %%ebx" : : "m" (tq));
	asm ("movl	$1, %edx");
	asm ("xorl	%eax, %eax");
	asm ("idivl	%ebx");
		
	asm ("movl	%%eax, %0" : : "m" (invq));
	asm ("movl	%0, %%ebx" : : "m" (tu));
	asm ("imul	%ebx");
	asm ("shldl	$28, %eax, %edx"); 
	asm ("movl	%%edx, %0" : : "m" (uoff));
			
	asm ("movl	%0, %%eax" : : "m" (invq));
	asm ("movl	%0, %%ebx" : : "m" (tv));
	asm ("imul	%ebx");
	asm ("shldl	$28, %eax, %edx");
	asm ("mov	%%edx, %0" : : "m" (voff));
#endif
	
 	INT invq1;			
	INT uoff1;			
	INT voff1;			

#ifdef _MSC_VER
	_asm mov	ebx, tq
	_asm mov	eax, tdq
	_asm shl	eax, 4
	_asm add	ebx, eax
	_asm mov	edx, 1
	_asm xor	eax, eax
	_asm idiv	ebx
		
	_asm mov	invq1, eax
	_asm mov	ebx, tu
	_asm mov	edx, tdu
	_asm shl	edx, 4
	_asm add	ebx, edx
	_asm imul	ebx
	_asm shld	edx, eax, 6 + 22
	_asm mov	uoff1, edx
			
	_asm mov	eax, invq1
	_asm mov	ebx, tv
	_asm mov	edx, tdv
	_asm shl	edx, 4
	_asm add	ebx, edx
	_asm imul	ebx
	_asm shld	edx, eax, 6 + 22
	_asm mov	voff1, edx
#else
 	asm ("movl	%0, %%ebx" : : "m" (tq));
	asm ("movl	%0, %%eax" : : "m" (tdq));
	asm ("shll	$4, %eax");
	asm ("addl	%eax, %ebx");
	asm ("movl	$1, %edx");
	asm ("xorl	%eax, %eax");
	asm ("idivl	%ebx");
		
	asm ("movl	%%eax, %0" : : "m" (invq1));
	asm ("movl	%0, %%ebx" : : "m" (tu));
	asm ("movl	%0, %%edx" : : "m" (tdu));
	asm ("shll	$4, %edx");
	asm ("addl	%edx, %ebx");
	asm ("imul	%ebx");
	asm ("shldl	$28, %eax, %edx"); 
	asm ("movl	%%edx, %0" : : "m" (uoff1));
			
	asm ("movl	%0, %%eax" : : "m" (invq1));
	asm ("movl	%0, %%ebx" : : "m" (tv));
	asm ("movl	%0, %%edx" : : "m" (tdv));
	asm ("shll	$4, %edx");
	asm ("addl	%edx, %ebx");
	asm ("imul	%ebx");
	asm ("shldl	$28, %eax, %edx");
	asm ("movl	%%edx, %0" : : "m" (voff1));
#endif
	
 	int itdu = (uoff1 - uoff) / 16;
	int itdv = (voff1 - voff) / 16;

	if (x1 > 639) return;
	
	WORD qq = tq >> DEPTH_SHIFT;
	WORD *pDB = (WORD*)(pDepthBuffer + offset);
	if (*pDB < qq)
	{
		UINT toffset = ( (((voff + half)>>22)&TEXHEIGHTBOUND) * TEXWIDTH + ( ((uoff + half)>>22)&TEXWIDTHBOUND) ) * 4;

		DWORD tcolor32 = *((DWORD*)(pTex + toffset));
		WORD tB = tcolor32 & 0x000000FF;
		WORD tG = (tcolor32 & 0x0000FF00) >> 8;
		WORD tR = (tcolor32 & 0x00FF0000) >> 16;
			
		color565  = ((cLLi.r>>16) * tR) >> 8 >> 3 << 11;
		color565 |= ((cLLi.g>>16) * tG) >> 8 >> 2 << 5;
		color565 |= ((cLLi.b>>16) * tB) >> 8 >> 3;

		*((WORD*)(pBits + offset)) = color565;
		*pDB = qq;
	}
}

//--------------------------------------------------
//
//   Triangle Routines
//
//--------------------------------------------------
void DrawTriangle(TLVertex * pTriVertex, int * IList)
{
	int i;
	Vector3 Top, Middle, Bottom;
	bool bIsMiddleLeft = false;
	bool bIsOnlyBottom = false;

	int TopIndex = IList[0];
	int MiddleIndex = IList[1];
	int BottomIndex = IList[2];

 	// Top to Bottom Vertex sorting
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[MiddleIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = MiddleIndex;
		MiddleIndex = temp;
	}
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = BottomIndex;
		BottomIndex = temp;
	}
	if ( pTriVertex[MiddleIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = MiddleIndex;
		MiddleIndex = BottomIndex;
		BottomIndex = temp;
	}
	
	Top = Vector3(pTriVertex[TopIndex].pos.x, pTriVertex[TopIndex].pos.y, pTriVertex[TopIndex].pos.z);
	Middle = Vector3(pTriVertex[MiddleIndex].pos.x, pTriVertex[MiddleIndex].pos.y, pTriVertex[MiddleIndex].pos.z);
	Bottom = Vector3(pTriVertex[BottomIndex].pos.x, pTriVertex[BottomIndex].pos.y, pTriVertex[BottomIndex].pos.z);

	// degenerated triangle
	if (Top.y == Bottom.y) return;
	
	Vector3 cTop(pTriVertex[TopIndex].diffuse.r, pTriVertex[TopIndex].diffuse.g, pTriVertex[TopIndex].diffuse.b);
	Vector3 cMiddle(pTriVertex[MiddleIndex].diffuse.r, pTriVertex[MiddleIndex].diffuse.g, pTriVertex[MiddleIndex].diffuse.b);
	Vector3 cBottom(pTriVertex[BottomIndex].diffuse.r, pTriVertex[BottomIndex].diffuse.g, pTriVertex[BottomIndex].diffuse.b);

	Vector2 tTop(pTriVertex[TopIndex].texcoord.u * 255, pTriVertex[TopIndex].texcoord.v * 255);
	Vector2 tMiddle(pTriVertex[MiddleIndex].texcoord.u * 255, pTriVertex[MiddleIndex].texcoord.v * 255);
	Vector2 tBottom(pTriVertex[BottomIndex].texcoord.u * 255, pTriVertex[BottomIndex].texcoord.v * 255);

	//Vector3 cTop2 = pTriVertex[TopIndex].specular;
	//Vector3 cMiddle2 = pTriVertex[MiddleIndex].specular;
	//Vector3 cBottom2 = pTriVertex[BottomIndex].specular;

	float dTBx, dTMx, dMBx;
	Vector3 DeltaTB, DeltaTM, DeltaMB;
	Vector3 cdTB, cdTM, cdMB;
	Vector2 tdTB, tdTM, tdMB;

	float LTx, RTx, LBx, RBx;		// X
	float dLTx, dRTx, dLBx, dRBx;	// X Gradient
	
	Vector3 cLT, cRT, cLB, cRB;		// Color    
	Vector3 cdLT, cdRT, cdLB, cdRB; //
	
	Vector2 tLT, tRT, tLB, tRB;		// Texture Coord
	Vector2 tdLT, tdRT, tdLB, tdRB;
	
	int TY, MY, BY;

	if (Top.y == Middle.y) 
	{
		bIsOnlyBottom = true;
		
		DeltaTB = Bottom - Top;
		DeltaMB = Bottom - Middle;
		dTBx = DeltaTB.x / DeltaTB.y;
		dMBx = DeltaMB.x / DeltaMB.y;
		cdTB = (cBottom - cTop) / DeltaTB.y;
		cdMB = (cBottom - cMiddle) / DeltaMB.y;
		tdTB = (tBottom - tTop) / DeltaTB.y;
		tdMB = (tBottom - tMiddle) / DeltaMB.y;

		// check middle vertex position
		if (Middle.x < Top.x)
		{
			LBx = Middle.x;
			RBx = Top.x;
			cLB = cMiddle;
			cRB = cTop;
			tLB = tMiddle;
			tRB = tTop;
			bIsMiddleLeft = true;
		}
		else
		{
			LBx = Top.x;
			RBx = Middle.x;
			cLB = cTop;
			cRB = cMiddle;
			tLB = tTop;
			tRB = tMiddle;
			bIsMiddleLeft = false;
		}
		goto MIDDLE_TO_BOTTOM;
	}

	// Edge Interpolation
	//  Top to Bottom 
	DeltaTB = Bottom - Top;
	dTBx = DeltaTB.x / DeltaTB.y;
	cdTB = (cBottom - cTop) / DeltaTB.y;
	tdTB = (tBottom - tTop) / DeltaTB.y;

	//  Top to Middle
	DeltaTM = Middle - Top;
	dTMx = DeltaTM.x / DeltaTM.y;
	cdTM = (cMiddle - cTop) / DeltaTM.y;
	tdTM = (tMiddle - tTop) / DeltaTM.y;

	// check middle vertex position
	if ( Middle.x < (Top.x + dTBx * DeltaTM.y))
	{
		dLTx = dTMx;
		dRTx = dTBx;
		cdLT = cdTM;
		cdRT = cdTB;
		tdLT = tdTM;
		tdRT = tdTB;

		bIsMiddleLeft = true;
	}
	else
	{
		dLTx = dTBx;
		dRTx = dTMx;
		cdLT = cdTB;
		cdRT = cdTM;
		tdLT = tdTB;
		tdRT = tdTM;

		bIsMiddleLeft = false;
	}

	LTx = Top.x;
	RTx = Top.x;
	cLT = cTop;
	cRT = cTop;
	tLT = tTop;
	tRT = tTop;

	TY = ftoi(Top.y);
	MY = ftoi(Middle.y);

	// For sub-pixel accuracy
	LTx += dLTx * (TY - Top.y);
	RTx += dRTx * (TY - Top.y);
	cLT += cdLT * (TY - Top.y);
	cRT += cdRT * (TY - Top.y);
	tLT += tdLT * (TY - Top.y);
	tRT += tdRT * (TY - Top.y);
	if (Top.y > Bound.by) return;
	if (Top.y < Bound.ty && Middle.y >= Bound.ty)
	{
		LTx += dLTx * (Bound.ty - TY);
		RTx += dRTx * (Bound.ty - TY);
		cLT += cdLT * (Bound.ty - TY);
		cRT += cdRT * (Bound.ty - TY);
		tLT += tdLT * (Bound.ty - TY);
		tRT += tdRT * (Bound.ty - TY);

		TY = Bound.ty;
	}
	if (Top.y < Bound.ty && Middle.y < Bound.ty)
	{
		LTx += dLTx * (Middle.y - TY);
		RTx += dRTx * (Middle.y - TY);
		cLT += cdLT * (Middle.y - TY);
		cRT += cdRT * (Middle.y - TY);
		tLT += tdLT * (Middle.y - TY);
		tRT += tdRT * (Middle.y - TY);
		
		if (bIsMiddleLeft)
		{
			LBx = Middle.x;
			RBx = RTx;
			cLB = cMiddle;
			cRB = cRT;
			tLB = tMiddle;
			tRB = tRT;
		}
		else
		{
			LBx = LTx;
			RBx = Middle.x;
			cLB = cLT;
			cRB = cMiddle;
			tLB = tLT;
			tRB = tMiddle;
		}
		goto MIDDLE_TO_BOTTOM;
	}
	if (Middle.y > Bound.by)
	{
		MY = Bound.by + 1;
	}
	
	for (i = TY; i < MY; i++)
	{
#if DRAW_MODE == 0
		DrawScanLine(LTx, RTx, i);
#elif DRAW_MODE == 1
		DrawScanLine(LTx, RTx, i, cLT, cRT);
#elif DRAW_MODE == 2
		DrawScanLine(LTx, RTx, i, tLT, tRT);
#elif DRAW_MODE == 3
		DrawScanLine(LTx, RTx, i, cLT, cRT, tLT, tRT);
#endif
		LTx += dLTx;
		RTx += dRTx;
		cLT += cdLT;
		cRT += cdRT;
		tLT += tdLT;
		tRT += tdRT;
	}

	if (bIsMiddleLeft)
	{
		LBx = Middle.x;
		RBx = RTx;
		cLB = cMiddle;
		cRB = cRT;
		tLB = tMiddle;
		tRB = tRT;
	}
	else
	{
		LBx = LTx;
		RBx = Middle.x;
		cLB = cLT;
		cRB = cMiddle;
		tLB = tLT;
		tRB = tMiddle;
	}

MIDDLE_TO_BOTTOM://///////////////////////////////////////////////////
	
	if (Middle.y == Bottom.y) return;

	DeltaMB = Bottom - Middle;
	dMBx = DeltaMB.x / DeltaMB.y;
	cdMB = (cBottom - cMiddle) / DeltaMB.y;
	tdMB = (tBottom - tMiddle) / DeltaMB.y;

	if (bIsMiddleLeft)
	{
		dLBx = dMBx;
		dRBx = dTBx;
		cdLB = cdMB;
		cdRB = cdTB;
		tdLB = tdMB;
		tdRB = tdTB;
	}
	else
	{
		dLBx = dTBx;
		dRBx = dMBx;
		cdLB = cdTB;
		cdRB = cdMB;
		tdLB = tdTB;
		tdRB = tdMB;
	}
	
	MY = ftoi(Middle.y);
	BY = ftoi(Bottom.y);
	
	/////////////////////////
	// For sub-pixel accuracy
	if ( bIsOnlyBottom )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	else if ( bIsMiddleLeft )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
	}
	else
	{
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	if (Bottom.y < Bound.ty) return;
	if (Middle.y < Bound.ty)
	{
		if ( bIsOnlyBottom )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		else if ( bIsMiddleLeft )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			
			RBx += dRBx * (Bound.ty - Middle.y);
			cRB += cdRB * (Bound.ty - Middle.y);
			tRB += tdRB * (Bound.ty - Middle.y);
		}
		else
		{
			LBx += dLBx * (Bound.ty - Middle.y);
			cLB += cdLB * (Bound.ty - Middle.y);
			tLB += tdLB * (Bound.ty - Middle.y);
			
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		MY = Bound.ty;
	}
	if (Bottom.y > Bound.by)
	{
		BY = Bound.by + 1;
	}
	for (i = MY; i < BY; i++)
	{
#if	DRAW_MODE == 0
		DrawScanLine(LBx, RBx, i);
#elif DRAW_MODE == 1
		DrawScanLine(LBx, RBx, i, cLB, cRB);
#elif DRAW_MODE == 2
		DrawScanLine(LBx, RBx, i, tLB, tRB);
#elif DRAW_MODE == 3
		DrawScanLine(LBx, RBx, i, cLB, cRB, tLB, tRB);
#endif
		LBx += dLBx;
		RBx += dRBx;
		cLB += cdLB;
		cRB += cdRB;
		tLB += tdLB;
		tRB += tdRB;
	}
}

void DrawTrianglePers(TLVertex * pTriVertex, int * IList)
{
	int i;
	Vector4 Top, Middle, Bottom;
	bool bIsMiddleLeft = false;
	bool bIsOnlyBottom = false;
	
	int TopIndex = IList[0];
	int MiddleIndex = IList[1];
	int BottomIndex = IList[2];

	// Top to Bottom Vertex sorting
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[MiddleIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = MiddleIndex;
		MiddleIndex = temp;
	}
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = BottomIndex;
		BottomIndex = temp;
	}
	if ( pTriVertex[MiddleIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = MiddleIndex;
		MiddleIndex = BottomIndex;
		BottomIndex = temp;
	}
	
	Top = pTriVertex[TopIndex].pos;
	Middle = pTriVertex[MiddleIndex].pos;
	Bottom = pTriVertex[BottomIndex].pos;

	// degenerated triangle
	if (Top.y == Bottom.y) return;
	
	Vector3 cTop(pTriVertex[TopIndex].diffuse.r, pTriVertex[TopIndex].diffuse.g, pTriVertex[TopIndex].diffuse.b);
	Vector3 cMiddle(pTriVertex[MiddleIndex].diffuse.r, pTriVertex[MiddleIndex].diffuse.g, pTriVertex[MiddleIndex].diffuse.b);
	Vector3 cBottom(pTriVertex[BottomIndex].diffuse.r, pTriVertex[BottomIndex].diffuse.g, pTriVertex[BottomIndex].diffuse.b);

	//Vector3 cTop2 = pTriVertex[TopIndex].specular;
	//Vector3 cMiddle2 = pTriVertex[MiddleIndex].specular;
	//Vector3 cBottom2 = pTriVertex[BottomIndex].specular;

	Vector3 tTop(pTriVertex[TopIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[TopIndex].texcoord.q,
				pTriVertex[TopIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[TopIndex].texcoord.q,
				1 / pTriVertex[TopIndex].texcoord.q);
	Vector3 tMiddle(pTriVertex[MiddleIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[MiddleIndex].texcoord.q,
				pTriVertex[MiddleIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[MiddleIndex].texcoord.q,
				1 / pTriVertex[MiddleIndex].texcoord.q);
	Vector3 tBottom(pTriVertex[BottomIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[BottomIndex].texcoord.q,
				pTriVertex[BottomIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[BottomIndex].texcoord.q,
				1 / pTriVertex[BottomIndex].texcoord.q);
	
	float dTBx, dTMx, dMBx;
	Vector4 DeltaTB, DeltaTM, DeltaMB;
	Vector3 cdTB, cdTM, cdMB;
	Vector3 tdTB, tdTM, tdMB;

	float LTx, RTx, LBx, RBx;		// X
	float dLTx, dRTx, dLBx, dRBx;	// X Gradient
	
	Vector3 cLT, cRT, cLB, cRB;		// Color    
	Vector3 cdLT, cdRT, cdLB, cdRB; // Color Gradient
	
	Vector3 tLT, tRT, tLB, tRB;		// Texture Coord
	Vector3 tdLT, tdRT, tdLB, tdRB; // TexCoord Gradient
	
	int TY, MY, BY;

	// Bottom Only Triangle case
	if (Top.y == Middle.y) 
	{
		bIsOnlyBottom = true;
		
		DeltaTB = Bottom - Top;
		DeltaMB = Bottom - Middle;
		dTBx = DeltaTB.x / DeltaTB.y;
		dMBx = DeltaMB.x / DeltaMB.y;
		cdTB = (cBottom - cTop) / DeltaTB.y;
		cdMB = (cBottom - cMiddle) / DeltaMB.y;
		tdTB = (tBottom - tTop) / DeltaTB.y;
		tdMB = (tBottom - tMiddle) / DeltaMB.y;

		// check middle vertex position
		if (Middle.x < Top.x)
		{
			LBx = Middle.x;
			RBx = Top.x;
			cLB = cMiddle;
			cRB = cTop;
			tLB = tMiddle;
			tRB = tTop;
			bIsMiddleLeft = true;
		}
		else
		{
			LBx = Top.x;
			RBx = Middle.x;
			cLB = cTop;
			cRB = cMiddle;
			tLB = tTop;
			tRB = tMiddle;
			bIsMiddleLeft = false;
		}
		goto MIDDLE_TO_BOTTOM;
	}

	///////////////////////////////////////////////
	// Edge Interpolation
	//  Top to Bottom 
	DeltaTB = Bottom - Top;
	dTBx = DeltaTB.x / DeltaTB.y;
	cdTB = (cBottom - cTop) / DeltaTB.y;
	tdTB = (tBottom - tTop) / DeltaTB.y;

	//  Top to Middle
	DeltaTM = Middle - Top;
	dTMx = DeltaTM.x / DeltaTM.y;
	cdTM = (cMiddle - cTop) / DeltaTM.y;
	tdTM = (tMiddle - tTop) / DeltaTM.y;

	// check middle vertex position
	if ( Middle.x < (Top.x + dTBx * DeltaTM.y))
	{
		dLTx = dTMx;
		dRTx = dTBx;
		cdLT = cdTM;
		cdRT = cdTB;
		tdLT = tdTM;
		tdRT = tdTB;

		bIsMiddleLeft = true;
	}
	else
	{
		dLTx = dTBx;
		dRTx = dTMx;
		cdLT = cdTB;
		cdRT = cdTM;
		tdLT = tdTB;
		tdRT = tdTM;

		bIsMiddleLeft = false;
	}

	LTx = Top.x;
	RTx = Top.x;
	cLT = cTop;
	cRT = cTop;
	tLT = tTop;
	tRT = tTop;

	TY = ftoi(Top.y);
	MY = ftoi(Middle.y);

	//////////////////////////////////////////////
	// For sub-pixel accuracy
	LTx += dLTx * (TY - Top.y);
	RTx += dRTx * (TY - Top.y);
	cLT += cdLT * (TY - Top.y);
	cRT += cdRT * (TY - Top.y);
	tLT += tdLT * (TY - Top.y);
	tRT += tdRT * (TY - Top.y);
	if (Top.y > Bound.by) return;  // no draw case
	if (Top.y < Bound.ty && Middle.y >= Bound.ty)
	{
		LTx += dLTx * (Bound.ty - TY);
		RTx += dRTx * (Bound.ty - TY);
		cLT += cdLT * (Bound.ty - TY);
		cRT += cdRT * (Bound.ty - TY);
		tLT += tdLT * (Bound.ty - TY);
		tRT += tdRT * (Bound.ty - TY);

		TY = Bound.ty;
	}
	if (Top.y < Bound.ty && Middle.y < Bound.ty)
	{
		LTx += dLTx * (Middle.y - TY);
		RTx += dRTx * (Middle.y - TY);
		cLT += cdLT * (Middle.y - TY);
		cRT += cdRT * (Middle.y - TY);
		tLT += tdLT * (Middle.y - TY);
		tRT += tdRT * (Middle.y - TY);
		
		if (bIsMiddleLeft)
		{
			LBx = Middle.x;
			RBx = RTx;
			cLB = cMiddle;
			cRB = cRT;
			tLB = tMiddle;
			tRB = tRT;
		}
		else
		{
			LBx = LTx;
			RBx = Middle.x;
			cLB = cLT;
			cRB = cMiddle;
			tLB = tLT;
			tRB = tMiddle;
		}
		goto MIDDLE_TO_BOTTOM;
	}
	if (Middle.y > Bound.by)
	{
		MY = Bound.by + 1;
	}
	
	for (i = TY; i < MY; i++)
	{
		DrawScanLinePers(LTx, RTx, i, cLT, cRT, tLT, tRT);

		LTx += dLTx;
		RTx += dRTx;
		cLT += cdLT;
		cRT += cdRT;
		tLT += tdLT;
		tRT += tdRT;
	}

	if (bIsMiddleLeft)
	{
		LBx = Middle.x;
		RBx = RTx;
		cLB = cMiddle;
		cRB = cRT;
		tLB = tMiddle;
		tRB = tRT;
	}
	else
	{
		LBx = LTx;
		RBx = Middle.x;
		cLB = cLT;
		cRB = cMiddle;
		tLB = tLT;
		tRB = tMiddle;
	}

MIDDLE_TO_BOTTOM://///////////////////////////////////////////////////
	
	// degenerated triangle
	if (Middle.y == Bottom.y) return;

	DeltaMB = Bottom - Middle;
	dMBx = DeltaMB.x / DeltaMB.y;
	cdMB = (cBottom - cMiddle) / DeltaMB.y;
	tdMB = (tBottom - tMiddle) / DeltaMB.y;

	if (bIsMiddleLeft)
	{
		dLBx = dMBx;
		dRBx = dTBx;
		cdLB = cdMB;
		cdRB = cdTB;
		tdLB = tdMB;
		tdRB = tdTB;
	}
	else
	{
		dLBx = dTBx;
		dRBx = dMBx;
		cdLB = cdTB;
		cdRB = cdMB;
		tdLB = tdTB;
		tdRB = tdMB;
	}
	
	MY = ftoi(Middle.y);
	BY = ftoi(Bottom.y);
	
	/////////////////////////
	// For sub-pixel accuracy
	if ( bIsOnlyBottom )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	else if ( bIsMiddleLeft )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
	}
	else
	{
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	
	// No draw case
	if (Bottom.y < Bound.ty) return;  
	
	//////////////////////////////////////
	// Upper Bound Clipping
	if (Middle.y < Bound.ty)
	{
		if ( bIsOnlyBottom )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		else if ( bIsMiddleLeft )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			
			RBx += dRBx * (Bound.ty - Middle.y);
			cRB += cdRB * (Bound.ty - Middle.y);
			tRB += tdRB * (Bound.ty - Middle.y);
		}
		else
		{
			LBx += dLBx * (Bound.ty - Middle.y);
			cLB += cdLB * (Bound.ty - Middle.y);
			tLB += tdLB * (Bound.ty - Middle.y);
			
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		MY = Bound.ty;
	}

	if (Bottom.y > Bound.by)
	{
		BY = Bound.by + 1;
	}
	
	for (i = MY; i < BY; i++)
	{
		DrawScanLinePers(LBx, RBx, i, cLB, cRB, tLB, tRB);

		LBx += dLBx;
		RBx += dRBx;
		cLB += cdLB;
		cRB += cdRB;
		tLB += tdLB;
		tRB += tdRB;
	}
}

void DrawTrianglePers16(TLVertex * pTriVertex, int * IList)
{
	int i;
	Vector4 Top, Middle, Bottom;
	bool bIsMiddleLeft = false;
	bool bIsOnlyBottom = false;
	
	int TopIndex = IList[0];
	int MiddleIndex = IList[1];
	int BottomIndex = IList[2];

	// Top to Bottom Vertex sorting
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[MiddleIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = MiddleIndex;
		MiddleIndex = temp;
	}
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = BottomIndex;
		BottomIndex = temp;
	}
	if ( pTriVertex[MiddleIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = MiddleIndex;
		MiddleIndex = BottomIndex;
		BottomIndex = temp;
	}
	
	Top = pTriVertex[TopIndex].pos;
	Middle = pTriVertex[MiddleIndex].pos;
	Bottom = pTriVertex[BottomIndex].pos;

	// degenerated triangle
	if (Top.y == Bottom.y) return;
	
	Vector3 cTop(pTriVertex[TopIndex].diffuse.r, pTriVertex[TopIndex].diffuse.g, pTriVertex[TopIndex].diffuse.b);
	Vector3 cMiddle(pTriVertex[MiddleIndex].diffuse.r, pTriVertex[MiddleIndex].diffuse.g, pTriVertex[MiddleIndex].diffuse.b);
	Vector3 cBottom(pTriVertex[BottomIndex].diffuse.r, pTriVertex[BottomIndex].diffuse.g, pTriVertex[BottomIndex].diffuse.b);

	//Vector3 cTop2 = pTriVertex[TopIndex].specular;
	//Vector3 cMiddle2 = pTriVertex[MiddleIndex].specular;
	//Vector3 cBottom2 = pTriVertex[BottomIndex].specular;

	Vector3 tTop(pTriVertex[TopIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[TopIndex].texcoord.q,
				pTriVertex[TopIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[TopIndex].texcoord.q,
				1 / pTriVertex[TopIndex].texcoord.q);
	Vector3 tMiddle(pTriVertex[MiddleIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[MiddleIndex].texcoord.q,
				pTriVertex[MiddleIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[MiddleIndex].texcoord.q,
				1 / pTriVertex[MiddleIndex].texcoord.q);
	Vector3 tBottom(pTriVertex[BottomIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[BottomIndex].texcoord.q,
				pTriVertex[BottomIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[BottomIndex].texcoord.q,
				1 / pTriVertex[BottomIndex].texcoord.q);
	
	float dTBx, dTMx, dMBx;
	Vector4 DeltaTB, DeltaTM, DeltaMB;
	Vector3 cdTB, cdTM, cdMB;
	Vector3 tdTB, tdTM, tdMB;

	float LTx, RTx, LBx, RBx;		// X
	float dLTx, dRTx, dLBx, dRBx;	// X Gradient
	
	Vector3 cLT, cRT, cLB, cRB;		// Color    
	Vector3 cdLT, cdRT, cdLB, cdRB; // Color Gradient
	
	Vector3 tLT, tRT, tLB, tRB;		// Texture Coord
	Vector3 tdLT, tdRT, tdLB, tdRB; // TexCoord Gradient
	
	int TY, MY, BY;

	// Bottom Only Triangle case
	if (Top.y == Middle.y) 
	{
		bIsOnlyBottom = true;
		
		DeltaTB = Bottom - Top;
		DeltaMB = Bottom - Middle;
		dTBx = DeltaTB.x / DeltaTB.y;
		dMBx = DeltaMB.x / DeltaMB.y;
		cdTB = (cBottom - cTop) / DeltaTB.y;
		cdMB = (cBottom - cMiddle) / DeltaMB.y;
		tdTB = (tBottom - tTop) / DeltaTB.y;
		tdMB = (tBottom - tMiddle) / DeltaMB.y;

		// check middle vertex position
		if (Middle.x < Top.x)
		{
			LBx = Middle.x;
			RBx = Top.x;
			cLB = cMiddle;
			cRB = cTop;
			tLB = tMiddle;
			tRB = tTop;
			bIsMiddleLeft = true;
		}
		else
		{
			LBx = Top.x;
			RBx = Middle.x;
			cLB = cTop;
			cRB = cMiddle;
			tLB = tTop;
			tRB = tMiddle;
			bIsMiddleLeft = false;
		}
		goto MIDDLE_TO_BOTTOM;
	}

	///////////////////////////////////////////////
	// Edge Interpolation
	//  Top to Bottom 
	DeltaTB = Bottom - Top;
	dTBx = DeltaTB.x / DeltaTB.y;
	cdTB = (cBottom - cTop) / DeltaTB.y;
	tdTB = (tBottom - tTop) / DeltaTB.y;

	//  Top to Middle
	DeltaTM = Middle - Top;
	dTMx = DeltaTM.x / DeltaTM.y;
	cdTM = (cMiddle - cTop) / DeltaTM.y;
	tdTM = (tMiddle - tTop) / DeltaTM.y;

	// check middle vertex position
	if ( Middle.x < (Top.x + dTBx * DeltaTM.y))
	{
		dLTx = dTMx;
		dRTx = dTBx;
		cdLT = cdTM;
		cdRT = cdTB;
		tdLT = tdTM;
		tdRT = tdTB;

		bIsMiddleLeft = true;
	}
	else
	{
		dLTx = dTBx;
		dRTx = dTMx;
		cdLT = cdTB;
		cdRT = cdTM;
		tdLT = tdTB;
		tdRT = tdTM;

		bIsMiddleLeft = false;
	}

	LTx = Top.x;
	RTx = Top.x;
	cLT = cTop;
	cRT = cTop;
	tLT = tTop;
	tRT = tTop;

	TY = ftoi(Top.y);
	MY = ftoi(Middle.y);

	//////////////////////////////////////////////
	// For sub-pixel accuracy
	LTx += dLTx * (TY - Top.y);
	RTx += dRTx * (TY - Top.y);
	cLT += cdLT * (TY - Top.y);
	cRT += cdRT * (TY - Top.y);
	tLT += tdLT * (TY - Top.y);
	tRT += tdRT * (TY - Top.y);
	if (Top.y > Bound.by) return;  // no draw case
	if (Top.y < Bound.ty && Middle.y >= Bound.ty)
	{
		LTx += dLTx * (Bound.ty - TY);
		RTx += dRTx * (Bound.ty - TY);
		cLT += cdLT * (Bound.ty - TY);
		cRT += cdRT * (Bound.ty - TY);
		tLT += tdLT * (Bound.ty - TY);
		tRT += tdRT * (Bound.ty - TY);

		TY = Bound.ty;
	}
	if (Top.y < Bound.ty && Middle.y < Bound.ty)
	{
		LTx += dLTx * (Middle.y - TY);
		RTx += dRTx * (Middle.y - TY);
		cLT += cdLT * (Middle.y - TY);
		cRT += cdRT * (Middle.y - TY);
		tLT += tdLT * (Middle.y - TY);
		tRT += tdRT * (Middle.y - TY);
		
		if (bIsMiddleLeft)
		{
			LBx = Middle.x;
			RBx = RTx;
			cLB = cMiddle;
			cRB = cRT;
			tLB = tMiddle;
			tRB = tRT;
		}
		else
		{
			LBx = LTx;
			RBx = Middle.x;
			cLB = cLT;
			cRB = cMiddle;
			tLB = tLT;
			tRB = tMiddle;
		}
		goto MIDDLE_TO_BOTTOM;
	}
	if (Middle.y > Bound.by)
	{
		MY = Bound.by + 1;
	}
	
	for (i = TY; i < MY; i++)
	{
		DrawScanLinePers16(LTx, RTx, i, cLT, cRT, tLT, tRT);

		LTx += dLTx;
		RTx += dRTx;
		cLT += cdLT;
		cRT += cdRT;
		tLT += tdLT;
		tRT += tdRT;
	}

	if (bIsMiddleLeft)
	{
		LBx = Middle.x;
		RBx = RTx;
		cLB = cMiddle;
		cRB = cRT;
		tLB = tMiddle;
		tRB = tRT;
	}
	else
	{
		LBx = LTx;
		RBx = Middle.x;
		cLB = cLT;
		cRB = cMiddle;
		tLB = tLT;
		tRB = tMiddle;
	}

MIDDLE_TO_BOTTOM://///////////////////////////////////////////////////
	
	// degenerated triangle
	if (Middle.y == Bottom.y) return;

	DeltaMB = Bottom - Middle;
	dMBx = DeltaMB.x / DeltaMB.y;
	cdMB = (cBottom - cMiddle) / DeltaMB.y;
	tdMB = (tBottom - tMiddle) / DeltaMB.y;

	if (bIsMiddleLeft)
	{
		dLBx = dMBx;
		dRBx = dTBx;
		cdLB = cdMB;
		cdRB = cdTB;
		tdLB = tdMB;
		tdRB = tdTB;
	}
	else
	{
		dLBx = dTBx;
		dRBx = dMBx;
		cdLB = cdTB;
		cdRB = cdMB;
		tdLB = tdTB;
		tdRB = tdMB;
	}
	
	MY = ftoi(Middle.y);
	BY = ftoi(Bottom.y);
	
	/////////////////////////
	// For sub-pixel accuracy
	if ( bIsOnlyBottom )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	else if ( bIsMiddleLeft )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
	}
	else
	{
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	
	// No draw case
	if (Bottom.y < Bound.ty) return;  
	
	//////////////////////////////////////
	// Upper Bound Clipping
	if (Middle.y < Bound.ty)
	{
		if ( bIsOnlyBottom )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		else if ( bIsMiddleLeft )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			
			RBx += dRBx * (Bound.ty - Middle.y);
			cRB += cdRB * (Bound.ty - Middle.y);
			tRB += tdRB * (Bound.ty - Middle.y);
		}
		else
		{
			LBx += dLBx * (Bound.ty - Middle.y);
			cLB += cdLB * (Bound.ty - Middle.y);
			tLB += tdLB * (Bound.ty - Middle.y);
			
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		MY = Bound.ty;
	}

	if (Bottom.y > Bound.by)
	{
		BY = Bound.by + 1;
	}
	
	for (i = MY; i < BY; i++)
	{
		DrawScanLinePers16(LBx, RBx, i, cLB, cRB, tLB, tRB);

		LBx += dLBx;
		RBx += dRBx;
		cLB += cdLB;
		cRB += cdRB;
		tLB += tdLB;
		tRB += tdRB;
	}
}

void DrawTriangleWire(TLVertex * pTriVertex, int * IList)
{
	int i;
	Vector4 Top, Middle, Bottom;
	bool bIsMiddleLeft = false;
	bool bIsOnlyBottom = false;
	
	int TopIndex = IList[0];
	int MiddleIndex = IList[1];
	int BottomIndex = IList[2];

	// Top to Bottom Vertex sorting
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[MiddleIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = MiddleIndex;
		MiddleIndex = temp;
	}
	if ( pTriVertex[TopIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = TopIndex;
		TopIndex = BottomIndex;
		BottomIndex = temp;
	}
	if ( pTriVertex[MiddleIndex].pos.y > pTriVertex[BottomIndex].pos.y )
	{
		int temp = MiddleIndex;
		MiddleIndex = BottomIndex;
		BottomIndex = temp;
	}
	
	Top = pTriVertex[TopIndex].pos;
	Middle = pTriVertex[MiddleIndex].pos;
	Bottom = pTriVertex[BottomIndex].pos;

	// degenerated triangle
	if (Top.y == Bottom.y) return;
	
	Vector3 cTop(pTriVertex[TopIndex].diffuse.r, pTriVertex[TopIndex].diffuse.g, pTriVertex[TopIndex].diffuse.b);
	Vector3 cMiddle(pTriVertex[MiddleIndex].diffuse.r, pTriVertex[MiddleIndex].diffuse.g, pTriVertex[MiddleIndex].diffuse.b);
	Vector3 cBottom(pTriVertex[BottomIndex].diffuse.r, pTriVertex[BottomIndex].diffuse.g, pTriVertex[BottomIndex].diffuse.b);

	//Vector3 cTop2 = pTriVertex[TopIndex].specular;
	//Vector3 cMiddle2 = pTriVertex[MiddleIndex].specular;
	//Vector3 cBottom2 = pTriVertex[BottomIndex].specular;

	Vector3 tTop(pTriVertex[TopIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[TopIndex].texcoord.q,
				pTriVertex[TopIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[TopIndex].texcoord.q,
				1 / pTriVertex[TopIndex].texcoord.q);
	Vector3 tMiddle(pTriVertex[MiddleIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[MiddleIndex].texcoord.q,
				pTriVertex[MiddleIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[MiddleIndex].texcoord.q,
				1 / pTriVertex[MiddleIndex].texcoord.q);
	Vector3 tBottom(pTriVertex[BottomIndex].texcoord.u * (TEXWIDTH - 1) / pTriVertex[BottomIndex].texcoord.q,
				pTriVertex[BottomIndex].texcoord.v * (TEXHEIGHT - 1) / pTriVertex[BottomIndex].texcoord.q,
				1 / pTriVertex[BottomIndex].texcoord.q);
	
	float dTBx, dTMx, dMBx;
	Vector4 DeltaTB, DeltaTM, DeltaMB;
	Vector3 cdTB, cdTM, cdMB;
	Vector3 tdTB, tdTM, tdMB;

	float LTx, RTx, LBx, RBx;		// X
	float dLTx, dRTx, dLBx, dRBx;	// X Gradient
	
	Vector3 cLT, cRT, cLB, cRB;		// Color    
	Vector3 cdLT, cdRT, cdLB, cdRB; // Color Gradient
	
	Vector3 tLT, tRT, tLB, tRB;		// Texture Coord
	Vector3 tdLT, tdRT, tdLB, tdRB; // TexCoord Gradient
	
	int TY, MY, BY;

	// Bottom Only Triangle case
	if (Top.y == Middle.y) 
	{
		bIsOnlyBottom = true;
		
		DeltaTB = Bottom - Top;
		DeltaMB = Bottom - Middle;
		dTBx = DeltaTB.x / DeltaTB.y;
		dMBx = DeltaMB.x / DeltaMB.y;
		cdTB = (cBottom - cTop) / DeltaTB.y;
		cdMB = (cBottom - cMiddle) / DeltaMB.y;
		tdTB = (tBottom - tTop) / DeltaTB.y;
		tdMB = (tBottom - tMiddle) / DeltaMB.y;

		// check middle vertex position
		if (Middle.x < Top.x)
		{
			LBx = Middle.x;
			RBx = Top.x;
			cLB = cMiddle;
			cRB = cTop;
			tLB = tMiddle;
			tRB = tTop;
			bIsMiddleLeft = true;
		}
		else
		{
			LBx = Top.x;
			RBx = Middle.x;
			cLB = cTop;
			cRB = cMiddle;
			tLB = tTop;
			tRB = tMiddle;
			bIsMiddleLeft = false;
		}
		goto MIDDLE_TO_BOTTOM;
	}

	///////////////////////////////////////////////
	// Edge Interpolation
	//  Top to Bottom 
	DeltaTB = Bottom - Top;
	dTBx = DeltaTB.x / DeltaTB.y;
	cdTB = (cBottom - cTop) / DeltaTB.y;
	tdTB = (tBottom - tTop) / DeltaTB.y;

	//  Top to Middle
	DeltaTM = Middle - Top;
	dTMx = DeltaTM.x / DeltaTM.y;
	cdTM = (cMiddle - cTop) / DeltaTM.y;
	tdTM = (tMiddle - tTop) / DeltaTM.y;

	// check middle vertex position
	if ( Middle.x < (Top.x + dTBx * DeltaTM.y))
	{
		dLTx = dTMx;
		dRTx = dTBx;
		cdLT = cdTM;
		cdRT = cdTB;
		tdLT = tdTM;
		tdRT = tdTB;

		bIsMiddleLeft = true;
	}
	else
	{
		dLTx = dTBx;
		dRTx = dTMx;
		cdLT = cdTB;
		cdRT = cdTM;
		tdLT = tdTB;
		tdRT = tdTM;

		bIsMiddleLeft = false;
	}

	LTx = Top.x;
	RTx = Top.x;
	cLT = cTop;
	cRT = cTop;
	tLT = tTop;
	tRT = tTop;

	TY = ftoi(Top.y);
	MY = ftoi(Middle.y);

	//////////////////////////////////////////////
	// For sub-pixel accuracy
	LTx += dLTx * (TY - Top.y);
	RTx += dRTx * (TY - Top.y);
	cLT += cdLT * (TY - Top.y);
	cRT += cdRT * (TY - Top.y);
	tLT += tdLT * (TY - Top.y);
	tRT += tdRT * (TY - Top.y);
	if (Top.y > Bound.by) return;  // no draw case
	if (Top.y < Bound.ty && Middle.y >= Bound.ty)
	{
		LTx += dLTx * (Bound.ty - TY);
		RTx += dRTx * (Bound.ty - TY);
		cLT += cdLT * (Bound.ty - TY);
		cRT += cdRT * (Bound.ty - TY);
		tLT += tdLT * (Bound.ty - TY);
		tRT += tdRT * (Bound.ty - TY);

		TY = Bound.ty;
	}
	if (Top.y < Bound.ty && Middle.y < Bound.ty)
	{
		LTx += dLTx * (Middle.y - TY);
		RTx += dRTx * (Middle.y - TY);
		cLT += cdLT * (Middle.y - TY);
		cRT += cdRT * (Middle.y - TY);
		tLT += tdLT * (Middle.y - TY);
		tRT += tdRT * (Middle.y - TY);
		
		if (bIsMiddleLeft)
		{
			LBx = Middle.x;
			RBx = RTx;
			cLB = cMiddle;
			cRB = cRT;
			tLB = tMiddle;
			tRB = tRT;
		}
		else
		{
			LBx = LTx;
			RBx = Middle.x;
			cLB = cLT;
			cRB = cMiddle;
			tLB = tLT;
			tRB = tMiddle;
		}
		goto MIDDLE_TO_BOTTOM;
	}
	if (Middle.y > Bound.by)
	{
		MY = Bound.by + 1;
	}
	
	for (i = TY; i < MY; i++)
	{
		DrawScanLineWire(LTx, RTx, i, cLT, cRT, tLT, tRT);

		LTx += dLTx;
		RTx += dRTx;
		cLT += cdLT;
		cRT += cdRT;
		tLT += tdLT;
		tRT += tdRT;
	}

	if (bIsMiddleLeft)
	{
		LBx = Middle.x;
		RBx = RTx;
		cLB = cMiddle;
		cRB = cRT;
		tLB = tMiddle;
		tRB = tRT;
	}
	else
	{
		LBx = LTx;
		RBx = Middle.x;
		cLB = cLT;
		cRB = cMiddle;
		tLB = tLT;
		tRB = tMiddle;
	}

MIDDLE_TO_BOTTOM://///////////////////////////////////////////////////
	
	// degenerated triangle
	if (Middle.y == Bottom.y) return;

	DeltaMB = Bottom - Middle;
	dMBx = DeltaMB.x / DeltaMB.y;
	cdMB = (cBottom - cMiddle) / DeltaMB.y;
	tdMB = (tBottom - tMiddle) / DeltaMB.y;

	if (bIsMiddleLeft)
	{
		dLBx = dMBx;
		dRBx = dTBx;
		cdLB = cdMB;
		cdRB = cdTB;
		tdLB = tdMB;
		tdRB = tdTB;
	}
	else
	{
		dLBx = dTBx;
		dRBx = dMBx;
		cdLB = cdTB;
		cdRB = cdMB;
		tdLB = tdTB;
		tdRB = tdMB;
	}
	
	MY = ftoi(Middle.y);
	BY = ftoi(Bottom.y);
	
	/////////////////////////
	// For sub-pixel accuracy
	if ( bIsOnlyBottom )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	else if ( bIsMiddleLeft )
	{
		LBx += dLBx * (MY - Middle.y);
		cLB += cdLB * (MY - Middle.y);
		tLB += tdLB * (MY - Middle.y);
	}
	else
	{
		RBx += dRBx * (MY - Middle.y);
		cRB += cdRB * (MY - Middle.y);
		tRB += tdRB * (MY - Middle.y);
	}
	
	// No draw case
	if (Bottom.y < Bound.ty) return;  
	
	//////////////////////////////////////
	// Upper Bound Clipping
	if (Middle.y < Bound.ty)
	{
		if ( bIsOnlyBottom )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		else if ( bIsMiddleLeft )
		{
			LBx += dLBx * (Bound.ty - MY);
			cLB += cdLB * (Bound.ty - MY);
			tLB += tdLB * (Bound.ty - MY);
			
			RBx += dRBx * (Bound.ty - Middle.y);
			cRB += cdRB * (Bound.ty - Middle.y);
			tRB += tdRB * (Bound.ty - Middle.y);
		}
		else
		{
			LBx += dLBx * (Bound.ty - Middle.y);
			cLB += cdLB * (Bound.ty - Middle.y);
			tLB += tdLB * (Bound.ty - Middle.y);
			
			RBx += dRBx * (Bound.ty - MY);
			cRB += cdRB * (Bound.ty - MY);
			tRB += tdRB * (Bound.ty - MY);
		}
		MY = Bound.ty;
	}

	if (Bottom.y > Bound.by)
	{
		BY = Bound.by + 1;
	}
	
	for (i = MY; i < BY; i++)
	{
		DrawScanLineWire(LBx, RBx, i, cLB, cRB, tLB, tRB);

		LBx += dLBx;
		RBx += dRBx;
		cLB += cdLB;
		cRB += cdRB;
		tLB += tdLB;
		tRB += tdRB;
	}
}
