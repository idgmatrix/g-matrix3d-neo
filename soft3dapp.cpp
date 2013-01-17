/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include "soft3dapp.h"
#include "soft3dres.h"
#include "graphics.h"

static Soft3DApp* g_pSoft3DApp = NULL;

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	return g_pSoft3DApp->AppProc(hWnd, uMsg, wParam, lParam);
}

Soft3DApp::Soft3DApp()
{
	g_pSoft3DApp = this;

	m_bIsActive = false;
	
	// Default value
	m_ClientWidth = 640;
	m_ClientHeight = 480;
}

int Soft3DApp::Create(HINSTANCE hInst, int width, int height)
{
	WNDCLASS wc;

	m_ClientWidth = width;
	m_ClientHeight = height;

    wc.style			= CS_HREDRAW|CS_VREDRAW; 
    wc.lpfnWndProc		= WindowProc; 
    wc.cbClsExtra		= 0; 
    wc.cbWndExtra		= 0; 
    wc.hInstance		= hInst; 
    wc.hIcon			= LoadIcon(hInst, (LPCTSTR)IDI_ICON1); 
    wc.hCursor			= LoadCursor(NULL, IDC_ARROW); 
    wc.hbrBackground	= (HBRUSH)GetStockObject(WHITE_BRUSH); 
    wc.lpszMenuName		= MAKEINTRESOURCE(IDR_MENU1); 
    wc.lpszClassName	= "soft3d";
	
	m_hInst = hInst;

	RegisterClass(&wc);

	RECT rect;
	rect.left	= 0; 
	rect.top	= 0;
	rect.right	= m_ClientWidth - 1;
	rect.bottom = m_ClientHeight - 1;
	AdjustWindowRect(&rect, WS_OVERLAPPEDWINDOW/*|WS_SYSMENU|WS_MINIMIZEBOX*/, true);
	m_WindowWidth = rect.right - rect.left + 1;
	m_WindowHeight = rect.bottom - rect.top + 1;
	
	m_hWnd = CreateWindow("soft3d", 
			"soft3d <g-Matrix3d Neo>", 
			WS_OVERLAPPED|WS_SYSMENU|WS_MINIMIZEBOX,
			(GetSystemMetrics(SM_CXFULLSCREEN) - m_WindowWidth) / 2,
			(GetSystemMetrics(SM_CYFULLSCREEN) - m_WindowHeight)/ 2,
			m_WindowWidth,
			m_WindowHeight,
			NULL,
			NULL,
			hInst,
			NULL);

	ShowWindow(m_hWnd, SW_SHOW);
	UpdateWindow(m_hWnd);
	
	return 0;
}

int Soft3DApp::Run()
{
	MSG msg;
	
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			else return msg.wParam;
		}
		else if(IsActive())
		{
			UpdateFrame();
		}
		else 
		{
			WaitMessage();
		}
	}

	return msg.wParam;
}

LRESULT Soft3DApp::AppProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//RECT rect;
	// Mouse
	short int xPos, yPos;
	static bool bMouseOn = false;

	switch (uMsg)
	{
		case WM_CREATE:
			InitGraphics(hWnd, m_ClientWidth, m_ClientHeight);
			break;

		case WM_CLOSE:
			DeinitGraphics(hWnd);
			PostQuitMessage(0);
			break;

		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDC_ABOUT:
					MessageBox(hWnd, 
					"g-Matrix3d Neo v0.40\n\n(c) 2003 Kim Seong Wan(kaswan)  \n\n\tkaswan@hitel.net\n\twww.g-matrix.pe.kr",
					"About soft3d", MB_OK);
					return 0;

				case IDC_OPEN:
					OnOpen(hWnd);
					return 0;

				case IDC_EXIT:
					DeinitGraphics(hWnd);
					PostQuitMessage(0);
					return 0;
			}
			break;
		
		case WM_LBUTTONDOWN:
			bMouseOn = true;
			SetCapture(hWnd);
			xPos = LOWORD(lParam);  // horizontal position of cursor 
			yPos = HIWORD(lParam);  // vertical position of cursor
			//OnPicking(xPos, yPos);
			OnMouseLButtonDown(xPos, yPos);
			break;
		case WM_LBUTTONUP:
			bMouseOn = false;
			ReleaseCapture();
			break;
		case WM_MOUSEMOVE:
			if (bMouseOn)
			{
				xPos = LOWORD(lParam);  // horizontal position of cursor 
				yPos = HIWORD(lParam);  // vertical position of cursor
				//PutPixel(xPos, yPos);
				//UpdateFrame();
				OnMouseMove(xPos, yPos);
			}
			break;
		
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	
	return 0;
}
