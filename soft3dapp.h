/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef SOFT3DAPP_H
#define SOFT3DAPP_H

#include "stdafx.h"

class Soft3DApp{
public:
	Soft3DApp();
	int Create(HINSTANCE hInst, int width, int height);
	int Run();
	HRESULT AppProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND m_hWnd;
	HINSTANCE m_hInst;
	bool m_bIsActive;
	int m_ClientWidth;
	int m_ClientHeight;
	int m_WindowWidth;
	int m_WindowHeight;
};

#endif

