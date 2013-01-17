/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "stdafx.h"

#include "dibsection.h"
#include "vector.h"
#include "Transform.h"

extern HDC	hScreenDC, hMemoryDC;
extern UINT BytesPerScanline;
extern BYTE *pBits;
extern BYTE *pDepthBuffer;
extern CDIBSection FrameBuffer;
extern CTransform TF;

void	InitGraphics(HWND hWnd, int width, int height);
void	DeinitGraphics(HWND hWnd);
bool	IsActive(void);
void	UpdateFrame(void);
void	DisplayFPS(void);
void	PutNumber(int x, int y, int number);
void	PutNumber(int x, int y, UINT number);
void	PutNumber(int x, int y, float number);
void	PutNumberHex(int x, int y, int number);
void	PutNumber(int x, int y, char* text, int number);
void	PutNumber(int x, int y, char* text, UINT number);
void	PutNumber(int x, int y, char* text, float number);
void	PutNumberHex(int x, int y, char* text, int number);
void	ResetFrameCounter(void);

void OnPicking(int mx, int my);
void OnMouseLButtonDown(int mx, int my);
void OnMouseMove(int mx, int my);
void OnOpen(HWND hWnd);

#endif

