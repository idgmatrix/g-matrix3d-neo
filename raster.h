/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef RASTER_H
#define RASTER_H

void SetColor(BYTE r, BYTE g, BYTE b);
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

#endif

