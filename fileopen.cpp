/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"

char *GetOpenFileNameDlg(HWND hWnd);

char *GetOpenFileNameDlg(HWND hWnd)
{ 
	OPENFILENAME ofn;
	char str[] = "3DS MAX ASE Files(*.ASE)\0*.ase\0";
	char lpFile[1024] = "*.ase";
	static char lpTitle[1024] = "Open ASE";

	ofn.lStructSize			= sizeof(OPENFILENAME);
	ofn.hwndOwner			= hWnd;
	ofn.hInstance			= NULL;
	ofn.lpstrFilter			= str;
	ofn.lpstrCustomFilter	= NULL;
	ofn.nMaxCustFilter		= 40;
	ofn.nFilterIndex		= 0;
	ofn.lpstrFile			= lpFile;
	ofn.nMaxFile			= 256;
	ofn.lpstrFileTitle		= lpTitle; // recieve file name
	ofn.nMaxFileTitle		= 1024;
	ofn.lpstrInitialDir		= NULL;
	ofn.lpstrTitle			= lpTitle;
	ofn.Flags				= 0;
	ofn.nFileOffset			= 0;
	ofn.nFileExtension		= 0;
	ofn.lpstrDefExt			= "ase";
	ofn.lCustData			= 0;
	ofn.lpfnHook			= NULL;
	ofn.lpTemplateName		= NULL;

	if(!GetOpenFileName(&ofn))
	{
		return NULL;
	}

	return ofn.lpstrFileTitle;
}
