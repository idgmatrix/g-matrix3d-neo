/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include <assert.h>

#include "fileopen.h"
#include "dibsection.h"
#include "bmp.h"

#include "transform.h"
#include "vertex.h"
#include "mesh.h"
#include "renbuffer.h"
#include "raster.h"
#include "util.h"
#include "texmanager.h"
#include "matmanager.h"
#include "asemodel.h"
#include "model.h"

#ifdef _MSC_VER
#define _CW_DEFAULT ( _RC_NEAR + _PC_53 + _EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT + _EM_DENORMAL)
#endif
//typedef Vector3 Color3;
//typedef Vector4 Color4;

static float thetaX0, thetaY0;
static float thetaX, thetaY;

/////////////////////////////
CTransform TF;
CRenBuffer RBuffer;
CDIBSection FrameBuffer;
CTexManager TEXMAN;
CMatManager MATMAN;
RenderInfo RInfo;
/////////////////////////////

Model *pModel;

/*
#define REDI 240.0f

Mesh Rect;

Vertex VertexList[16] = {
 Vertex(Vector3(-REDI,  REDI, -REDI), Vector4(1.0f, 0.5f, 0.5f), Vector3(0,0,-1), Vector2(0,0)),
 Vertex(Vector3( REDI,  REDI, -REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,0,-1), Vector2(1,0)),
 Vertex(Vector3( REDI, -REDI, -REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,0,-1), Vector2(1,1)),
 Vertex(Vector3(-REDI, -REDI, -REDI), Vector4(0.5f, 0.5f, 1.0f), Vector3(0,0,-1), Vector2(0,1)),
 
 Vertex(Vector3(-REDI,  REDI, REDI), Vector4(1.0f, 1.0f, 1.0f), Vector3(0,0,1), Vector2(1,0)),
 Vertex(Vector3( REDI,  REDI, REDI), Vector4(1.0f, 1.0f, 1.0f), Vector3(0,0,1), Vector2(0,0)),
 Vertex(Vector3( REDI, -REDI, REDI), Vector4(1.0f, 1.0f, 1.0f), Vector3(0,0,1), Vector2(0,1)),
 Vertex(Vector3(-REDI, -REDI, REDI), Vector4(1.0f, 1.0f, 1.0f), Vector3(0,0,1), Vector2(1,1)),

 Vertex(Vector3(-REDI,  REDI, -REDI), Vector4(1.0f, 0.5f, 0.5f), Vector3(0,1,0), Vector2(0,0)),
 Vertex(Vector3( REDI,  REDI, -REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,1,0), Vector2(1,0)),
 Vertex(Vector3(-REDI,  REDI,  REDI), Vector4(1.0f, 0.5f, 0.5f), Vector3(0,1,0), Vector2(0,1)),
 Vertex(Vector3( REDI,  REDI,  REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,1,0), Vector2(1,1)),

 Vertex(Vector3( REDI, -REDI, -REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,-1,0), Vector2(1,1)),
 Vertex(Vector3(-REDI, -REDI, -REDI), Vector4(0.5f, 0.5f, 1.0f), Vector3(0,-1,0), Vector2(0,1)),
 Vertex(Vector3( REDI, -REDI,  REDI), Vector4(1.0f, 1.0f, 0.5f), Vector3(0,-1,0), Vector2(1,0)),
 Vertex(Vector3(-REDI, -REDI,  REDI), Vector4(0.5f, 0.5f, 1.0f), Vector3(0,-1,0), Vector2(0,0))};
 

int IndexList[36] = {0,1,2, 0,2,3, // front
					4,6,5, 4,7,6,  // back
					8,10,11, 8,11,9,  // up
					12,15,13, 12,14,15,  // down
					0,7,4, 0,3,7,  // left
					1,5,6, 1,6,2}; // right

*/
HDC	hScreenDC, hMemoryDC;
UINT BytesPerScanline;
BYTE *pBits;
BYTE *pDepthBuffer;

bool	bLoadedBMP;
bool	g_bIsActive;
bool	g_bIsASE;
UINT	frameCounter;
LARGE_INTEGER Frequency;
LARGE_INTEGER StartCounter, EndCounter;
	
static float fps;

void InitGraphics(HWND hWnd, int width, int height);
void DeinitGraphics(HWND hWnd);
void UpdateFrame(void);

void DisplayFPS(void);
void PutNumber(int x, int y, int number);
void PutNumber(int x, int y, UINT number);
void PutNumber(int x, int y, float number);
void PutNumberHex(int x, int y, int number);
void PutNumber(int x, int y, char* text, int number);
void PutNumber(int x, int y, char* text, UINT number);
void PutNumber(int x, int y, char* text, float number);
void PutNumberHex(int x, int y, char* text, int number);
void ResetFrameCounter(void);

void SetColor(BYTE r, BYTE g, BYTE b);

void OnOpen(HWND hWnd);

void OnPicking(int mx, int my);
void OnMouseMove(int mx, int my);

void InitGraphics(HWND hWnd, int width, int height)
{
	// Set Frame Buffer
	hScreenDC = GetDC(hWnd);
	hMemoryDC = FrameBuffer.Create(hScreenDC, width, height);
	BytesPerScanline = FrameBuffer.BytesPerScanline;
	pBits = FrameBuffer.pBits;
	
	// Set Depth Buffer
	pDepthBuffer = new BYTE[640 * 480 * 2];

	MATMAN.Init(20);
	
	// Texture 
	TEXMAN.Init(20);
	TEXMAN.Create(256, 256);
	TEXMAN.Load("Wizard.bmp");
	TEXMAN.SetTexture(0);
	
	//////////////////////////////////////////////
	//Rect.LoadMesh(VertexList, IndexList, 16, 36);
	
	QueryPerformanceFrequency(&Frequency);

	g_bIsActive = TRUE;
}

void DeinitGraphics(HWND hWnd)
{
	g_bIsActive = FALSE;

	delete[] pDepthBuffer;
	TEXMAN.DeInit();
	FrameBuffer.Destroy();
	ReleaseDC(hWnd, hScreenDC);
}

bool IsActive(void)
{
	return g_bIsActive;
}

void ClearDepthBuffer(void)
{
#ifdef _MSC_VER
	_asm {
		cld
		mov edi, pDepthBuffer
		xor	eax, eax
		mov	ecx, 640 * 480 * 2 / 4
		rep stosd
	}
#else
	asm ("cld");
	asm ("movl	%0, %%edi" : : "m" (pDepthBuffer) );
	asm ("xorl	%eax, %eax");
	asm ("movl	$(640 * 480 * 2 / 4), %ecx"); 
	asm ("rep stosl");
#endif
}

void ClearDepthBufferMMX(void)
{
/*	_asm {
	
		mov		edi, pDepthBuffer
		pxor	mm0, mm0
		mov		ecx, 640 * 480 * 2 / 8
L0:		movq	[edi], mm0
		add		edi, 8
		dec		ecx
		jnz		L0
		emms
	}
*/
}

void UpdateFrame(void)
{
	static float frametime;
	static Vector3 camera;
	static float FOV = 90;
	frameCounter++;

	// Clear fame buffer and depth buffer
	FrameBuffer.Clear(0, 0, 200);
	//memset(pDepthBuffer, 0, 640 * 480 * 2);
	ClearDepthBuffer();
		
	//SetColor(255, 50, 50);
	//DrawBox(20, 20, 639, 479);
	//FillBox(20, 20, 639, 479);

	// Keyboard Inputs
	if (GetAsyncKeyState(VK_LEFT)) camera.x -= 10;   
	if (GetAsyncKeyState(VK_RIGHT)) camera.x += 10;     
	if (GetAsyncKeyState(VK_UP))  camera.z += 10;   
	if (GetAsyncKeyState(VK_DOWN)) camera.z -= 10;   
	if (GetAsyncKeyState(VK_PRIOR))  camera.y += 10;   
	if (GetAsyncKeyState(VK_NEXT)) camera.y -= 10;   
	if (GetAsyncKeyState(VK_ADD)) FOV -= 5;   
	if (GetAsyncKeyState(VK_SUBTRACT)) FOV += 5;   
	
	PutNumber(0,  0, "camera.x", camera.x);
	PutNumber(0, 20, "camera.y", camera.y);
	PutNumber(0, 40, "camera.z", camera.z);
	PutNumber(0, 60, "FOV", FOV);
	
	if (GetAsyncKeyState(VK_F1)) TEXMAN.SetTexture(0);   
	if (GetAsyncKeyState(VK_F2)) TEXMAN.SetTexture(1);   
	if (GetAsyncKeyState(VK_F3)) TEXMAN.SetTexture(2);   
	if (GetAsyncKeyState(VK_F4)) TEXMAN.SetTexture(3);   
	
	/////////////////////////////////////////////////////////////
	
	TF.SetMatrixMode(MM_VIEW);
	TF.LoadIndentity();
	//TF.Translate(0, 0, -100);
	TF.Translate(-camera.x, -camera.y, -camera.z);
	
	TF.SetMatrixMode(MM_PROJECT);
	TF.LoadIndentity();
	//TF.SimplePerspective();
	//TF.Perspective(640, 480, 640, 2000);
	TF.PerspectiveFOVW(FOV, 0.75, 128, 10000);
	//TF.PerspectiveFOVH(75, 1.3333f, 320, 2000);
	
	TF.SetMatrixMode(MM_VIEWPORT);
	TF.LoadIndentity();
	TF.Scale(320, -240, 1);
	TF.Translate(320, 240, 0);

	////////////////////////////////////////////////////////////
	
	TF.SetMatrixMode(MM_WORLD);
	TF.LoadIndentity();
	//TF.Scale(5, 5, 5);
	TF.RotateY(thetaY); 
	TF.RotateX(thetaX + 0.5f); 
	TF.Translate(100, 1, 300);

	if (g_bIsASE)
	{
		//pModel->Render();
	}

	TF.SetMatrixMode(MM_WORLD);
	TF.LoadIndentity();
	TF.RotateY(thetaY); 
	TF.RotateX(thetaX); 
	TF.Translate(0, 0, 300);

	if (g_bIsASE)
	{
		pModel->Render();
	}

	DisplayFPS();
	RInfo.Display(0, 340);
	RInfo.Clear();

	FrameBuffer.Swap();

	// Calculate FPS 
	QueryPerformanceCounter(&EndCounter);
	frametime = (float)(EndCounter.QuadPart - StartCounter.QuadPart) / (float)Frequency.QuadPart;
	StartCounter = EndCounter;
	fps = 1 / frametime;

}

//------------------------------------------------------------------------
void ResetFrameCounter(void)
{
	frameCounter = 0;
}

void DisplayFPS(void)
{
	char string[20];

	sprintf(string, "FPS: %f", fps);
	TextOut(hMemoryDC, 250, 0, string, strlen(string));
	//TextOut(hScreenDC, 250, 0, string, strlen(string));
}

void PutNumber(int x, int y, int number)
{
	char string[20];

	sprintf(string, "%d", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
	//TextOut(hScreenDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, UINT number)
{
	char string[20];

	sprintf(string, "%u", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, float number)
{
	char string[20];

	sprintf(string, "%f", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, char* text, int number)
{
	char string[128];

	sprintf(string, "%s: %i", text, number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
	//TextOut(hScreenDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, char* text, UINT number)
{
	char string[128];

	sprintf(string, "%s: %u", text, number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumber(int x, int y, char* text, float number)
{
	char string[128];

	sprintf(string, "%s: %f", text, number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumberHex(int x, int y, int number)
{
	char string[20];

	sprintf(string, "0x%X    ", number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

void PutNumberHex(int x, int y, char* text, int number)
{
	char string[128];

	sprintf(string, "%s: 0x%X    ", text, number);
	TextOut(hMemoryDC, x, y, string, strlen(string));
}

//================================================================//

int Picked; 
void OnPicking(int mx, int my)
{
/*	for (int i = 0; i < 4; i++)
	{
		int dx = abs(ftoi(VertexList[i].pos.x) - mx);
		int dy = abs(ftoi(VertexList[i].pos.y) - my);
		if (dx <= 5 && dy <= 5)
		{
			Picked = i;
			return;
		}
	}
	Picked = -1;
*/
}

static int mx0, my0;
void OnMouseLButtonDown(int mx, int my)
{
	mx0 = mx;
	my0 = my;

	thetaX0 = thetaX;
	thetaY0 = thetaY;
}

void OnMouseMove(int mx, int my)
{
	thetaX = thetaX0 - DEG2RAD(my-my0);
	thetaY = thetaY0 - DEG2RAD(mx-mx0);
}

void OnOpen(HWND hWnd)
{
	char *fname = NULL;

	fname = GetOpenFileNameDlg(hWnd);
	if (NULL == fname)
	{	
		return;
	}
	
	if (pModel) delete pModel;
	
	ASEMODEL ase;
	ase.Init();
	pModel = ase.LoadASE(fname);
	assert(pModel);

	//BYTE *pBMP = LoadBMP(fname, );
	//if (pBMP == NULL) return;
	//DrawBMP(0, 0, pBMP);
}
