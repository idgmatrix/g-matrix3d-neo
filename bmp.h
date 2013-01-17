/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef BMP_H
#define BMP_H

#include "stdafx.h"

BYTE *LoadBMP(char *filename, int *pwidth, int *pheight);
void DrawBMP(int x, int y, BYTE * pBMP);

#endif

