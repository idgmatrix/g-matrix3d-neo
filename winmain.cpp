/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "soft3dapp.h"

Soft3DApp g_Soft3DApp;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    if( FAILED( g_Soft3DApp.Create(hInstance, 640, 480) ) )
        return 0;
	
	return g_Soft3DApp.Run();
}

