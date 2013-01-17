/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "stdafx.h"
#include <assert.h>

#include "renbuffer.h"
#include "transform.h"

#include "graphics.h"
#include "raster.h"
#include "texmanager.h"
#include "matmanager.h"

#ifndef _MSC_VER
#define _CW_DEFAULT ( _RC_NEAR + _PC_53 + _EM_INVALID + _EM_ZERODIVIDE + _EM_OVERFLOW + _EM_UNDERFLOW + _EM_INEXACT + _EM_DENORMAL)
#endif

extern CTexManager TEXMAN;
extern CMatManager MATMAN;
extern RenderInfo RInfo;

Vector4 LightPosition(-200, 150, 0, 1);

void CRenBuffer::RenderMesh(void)
{
	int i;

	TF.SetTransform();

	MATMAN.SetMaterial(CRenBuffer::MTLID);//////////////////////////////

	RInfo.NumMesh++;
	RInfo.NumVertex += NumVertex;
	RInfo.NumIndex += NumIndex; 
	//////////////////////////////////////////////////////////////////////
	// Transform to Clip Space
	for(i = 0; i < NumVertex; i++)
	{
		TLVertexList[i].pos = Vector4(VertexList[i].pos) * TF.WorldViewProjectTM;
		TLVertexList[i].texcoord = Vector3(VertexList[i].texcoord);
		TLVertexList[i].diffuse = VertexList[i].color;
	}
	
	////////////////////////////////////////////////////////////////////
	// Set clip flags
	for(i = 0; i < NumVertex; i++)
	{
		ViewVertexList[i].clipflag = 0;
		ViewVertexList[i].IsLit = true;

		if ( TLVertexList[i].pos.x < -TLVertexList[i].pos.w)
		{
			ViewVertexList[i].clipflag |= 0x01; // 01b
		}
		if ( TLVertexList[i].pos.x > TLVertexList[i].pos.w)
		{
			ViewVertexList[i].clipflag |= 0x02; // 10b
		}
		
		if ( TLVertexList[i].pos.y < -TLVertexList[i].pos.w)
		{
			ViewVertexList[i].clipflag |= 0x4; //0100b;
		}
		if ( TLVertexList[i].pos.y > TLVertexList[i].pos.w)
		{
			ViewVertexList[i].clipflag |= 0x8; //1000b;
		}
		
		if ( TLVertexList[i].pos.z < 0.0f)
		{
			ViewVertexList[i].clipflag |= 0x10; //10000b;
		}
		if ( TLVertexList[i].pos.z > TLVertexList[i].pos.w)
		{
			ViewVertexList[i].clipflag |= 0x20; //0100000b;
		}

		//PutNumber(0, 40, TLVertexList[i].pos.z);
		//PutNumber(0, 60, TLVertexList[i].pos.w);
		//PutNumberHex(0, 80, ViewVertexList[i].clipflag);
	}

	//////////////////////////////////////////////////////////////////////
	// View Frustum Culling at Clip Space
	int Index = 0;
	int RIndex = 0;
	for (i = 0; i < NumIndex; i += 3)
	{
		if (0 == (ViewVertexList[IndexList[i]].clipflag
			& ViewVertexList[IndexList[i+1]].clipflag
			& ViewVertexList[IndexList[i+2]].clipflag))
		{
			if (ViewVertexList[IndexList[i]].IsLit == true)
			{
				ViewVertexList[IndexList[i]].IsLit = false;
				ActiveVertexIndexList[Index++] = IndexList[i];
			}
			RenderIndexList[RIndex++] = IndexList[i];
			
			if (ViewVertexList[IndexList[i+1]].IsLit == true)
			{
				ViewVertexList[IndexList[i+1]].IsLit = false;
				ActiveVertexIndexList[Index++] = IndexList[i+1];
			}
			RenderIndexList[RIndex++] = IndexList[i+1];
			
			if (ViewVertexList[IndexList[i+2]].IsLit == true)
			{
				ViewVertexList[IndexList[i+2]].IsLit = false;
				ActiveVertexIndexList[Index++] = IndexList[i+2];
			}
			RenderIndexList[RIndex++] = IndexList[i+2];
			
		}
	}
	
	NumActiveVertex = Index;
	NumRenderIndex = RIndex;
	
	RInfo.NumActiveVertex += Index;
	RInfo.NumRenderIndex += RIndex;
	
	///////////////////////////////////////////////////////////////////////////
	// View Transforamtion (transform only Active Vertex)
	for(i = 0; i < NumActiveVertex; i++)
	{
		int ii = ActiveVertexIndexList[i];
		ViewVertexList[ii].pos = Vector4(VertexList[ii].pos) * TF.WorldViewTM;
		ViewVertexList[ii].normal = VertexList[ii].normal * TF.NormalTM;

	}
	
	////////////////////////////////////////////////////////////////////////////
	// Lighting with Near Z clipping
	int icount = NumRenderIndex;
	int iNumVertex = NumVertex;
	for(i = 0; i < icount; i += 3)
	{
		if ((ViewVertexList[RenderIndexList[i]].clipflag 
			| ViewVertexList[RenderIndexList[i+1]].clipflag 
			| ViewVertexList[RenderIndexList[i+2]].clipflag) & 0x10)
		{
			
			VertexLighting(RenderIndexList[i]);
			VertexLighting(RenderIndexList[i+1]);
			VertexLighting(RenderIndexList[i+2]);
			
			// Check types for Near Z Clipping 
			int I[3];
			I[0] = RenderIndexList[i];
			I[1] = RenderIndexList[i+1];
			I[2] = RenderIndexList[i+2];
			if (ViewVertexList[I[0]].clipflag & 0x10)
			{
				int temp;
				temp = I[0];
				I[0] = I[1];
				I[1] = I[2];
				I[2] = temp;
			
				if (ViewVertexList[I[0]].clipflag & 0x10)
				{
					int temp;
					temp = I[0];
					I[0] = I[1];
					I[1] = I[2];
					I[2] = temp;
				}
			}
			else if (!(ViewVertexList[I[2]].clipflag & 0x10))
			{
					int temp;
					temp = I[2];
					I[2] = I[1];
					I[1] = I[0];
					I[0] = temp;
			}

			if (ViewVertexList[I[1]].clipflag & 0x10)  // Near Z Clipping Simple Case
			{
				float dz1 = (0.0f - TLVertexList[I[0]].pos.z) / (TLVertexList[I[1]].pos.z - TLVertexList[I[0]].pos.z); 
				TLVertexList[iNumVertex].pos = TLVertexList[I[0]].pos + (TLVertexList[I[1]].pos - TLVertexList[I[0]].pos) * dz1; 
				TLVertexList[iNumVertex].diffuse = TLVertexList[I[0]].diffuse + (TLVertexList[I[1]].diffuse - TLVertexList[I[0]].diffuse) * dz1; 
				TLVertexList[iNumVertex].texcoord = TLVertexList[I[0]].texcoord + (TLVertexList[I[1]].texcoord - TLVertexList[I[0]].texcoord) * dz1; 

				float dz2 = (0.0f - TLVertexList[I[0]].pos.z) / (TLVertexList[I[2]].pos.z - TLVertexList[I[0]].pos.z); 
				TLVertexList[iNumVertex + 1].pos = TLVertexList[I[0]].pos + (TLVertexList[I[2]].pos - TLVertexList[I[0]].pos) * dz2; 
				TLVertexList[iNumVertex + 1].diffuse = TLVertexList[I[0]].diffuse + (TLVertexList[I[2]].diffuse - TLVertexList[I[0]].diffuse) * dz2; 
				TLVertexList[iNumVertex + 1].texcoord = TLVertexList[I[0]].texcoord + (TLVertexList[I[2]].texcoord - TLVertexList[I[0]].texcoord) * dz2;

				RenderIndexList[i] = I[0];
				RenderIndexList[i+1] = iNumVertex;
				RenderIndexList[i+2] = iNumVertex + 1;

				ActiveVertexIndexList[NumActiveVertex++] = iNumVertex;
				ActiveVertexIndexList[NumActiveVertex++] = iNumVertex + 1;
				
				ViewVertexList[iNumVertex].IsLit = true;
				ViewVertexList[iNumVertex + 1].IsLit = true;
				
				iNumVertex += 2;

			}
			else // none simple case
			{
				float dz0 = (0.0f - TLVertexList[I[0]].pos.z) / (TLVertexList[I[2]].pos.z - TLVertexList[I[0]].pos.z); 
				TLVertexList[iNumVertex].pos = TLVertexList[I[0]].pos + (TLVertexList[I[2]].pos - TLVertexList[I[0]].pos) * dz0; 
				TLVertexList[iNumVertex].diffuse = TLVertexList[I[0]].diffuse + (TLVertexList[I[2]].diffuse - TLVertexList[I[0]].diffuse) * dz0; 
				TLVertexList[iNumVertex].texcoord = TLVertexList[I[0]].texcoord + (TLVertexList[I[2]].texcoord - TLVertexList[I[0]].texcoord) * dz0; 

				float dz1 = (0.0f - TLVertexList[I[1]].pos.z) / (TLVertexList[I[2]].pos.z - TLVertexList[I[1]].pos.z); 
				TLVertexList[iNumVertex + 1].pos = TLVertexList[I[1]].pos + (TLVertexList[I[2]].pos - TLVertexList[I[1]].pos) * dz1; 
				TLVertexList[iNumVertex + 1].diffuse = TLVertexList[I[1]].diffuse + (TLVertexList[I[2]].diffuse - TLVertexList[I[1]].diffuse) * dz1; 
				TLVertexList[iNumVertex + 1].texcoord = TLVertexList[I[1]].texcoord + (TLVertexList[I[2]].texcoord - TLVertexList[I[1]].texcoord) * dz1; 

				RenderIndexList[i] = I[0];
				RenderIndexList[i+1] = I[1];
				RenderIndexList[i+2] = iNumVertex;
				
				RenderIndexList[NumRenderIndex++] = I[1];
				RenderIndexList[NumRenderIndex++] = iNumVertex + 1;
				RenderIndexList[NumRenderIndex++] = iNumVertex;

				ActiveVertexIndexList[NumActiveVertex++] = iNumVertex + 1;
				ActiveVertexIndexList[NumActiveVertex++] = iNumVertex;

				ViewVertexList[iNumVertex].IsLit = true;
				ViewVertexList[iNumVertex + 1].IsLit = true;
				
				iNumVertex += 2;
			}

		}
	}
	
	/////////////////////////////////////////////////////////////////
	// Viewport Scaling
	for(i = 0; i < NumActiveVertex; i++)
	{
		int ii = ActiveVertexIndexList[i];
		TLVertexList[ii].pos = TLVertexList[ii].pos * TF.TM[MM_VIEWPORT];
	}

	/////////////////////////////////////////////////////////////////
	//Perspective Division
	for(i = 0; i < NumActiveVertex; i++)
	{
		int ii = ActiveVertexIndexList[i];
		assert(TLVertexList[ii].pos.w != 0); 
		TLVertexList[ii].texcoord.q = TLVertexList[ii].pos.w; 
		TLVertexList[ii].pos /= TLVertexList[ii].pos.w;
	}
	
	RInfo.NumClippedVertex += NumActiveVertex;
	
	/////////////////////////////////////////////////////////
	//
	// Lighting & Back Face culling
	//
	/////////////////////////////////////////////////////////
	//_controlfp(~( _EM_ZERODIVIDE | _EM_INVALID ), _MCW_EM);
	//_controlfp(~( _EM_ZERODIVIDE ), _MCW_EM);
	_controlfp( _RC_UP, _MCW_RC);  // ceiling mode
	for (i = 0; i < NumRenderIndex; i += 3)
	{
		//TEXMAN.SetTexture(MTLIDList[i/3]);//////////////////////////////
		
		int i0 = RenderIndexList[i];
		int i1 = RenderIndexList[i+1];
		int i2 = RenderIndexList[i+2];
		
		// Back Face Culling
		Vector4 A = TLVertexList[i1].pos - TLVertexList[i0].pos; 
		Vector4 B = TLVertexList[i2].pos - TLVertexList[i0].pos;
		
		if ((A.x * B.y - A.y * B.x) > 0) // Back Face Culling
		{
			//Lighting
			VertexLighting(RenderIndexList[i]);
			VertexLighting(RenderIndexList[i+1]);
			VertexLighting(RenderIndexList[i+2]);
			
			// Draw triangle
			//DrawTriangleWire(TLVertexList, &RenderIndexList[i]);
			//DrawTrianglePers16(TLVertexList, &RenderIndexList[i]);
			DrawTrianglePers(TLVertexList, &RenderIndexList[i]);
			//DrawTriangle(TLVertexList, &RenderIndexList[i]);
			
			DrawedCounter++;
		}
/*		else
		{
			//Lighting
			VertexLightingBack(RenderIndexList[i]);
			VertexLightingBack(RenderIndexList[i+1]);
			VertexLightingBack(RenderIndexList[i+2]);
			
			// Draw triangle
			DrawTrianglePers16(TLVertexList, &RenderIndexList[i]);
			//DrawTrianglePers(TLVertexList, &RenderIndexList[i]);
			//DrawTriangle(TLVertexList, &RenderIndexList[i]);
			DrawedCounter++;
		}
*/
	}
	
    
    //_controlfp( _RC_NEAR, _MCW_RC );
	_control87( _CW_DEFAULT, 0xfffff );

	if (DrawedCounter == 0)
	{
	 	RInfo.NumDrawed += DrawedCounter;
	}
 	RInfo.NumDrawed += DrawedCounter;
	DrawedCounter = 0;

}	

void CRenBuffer::VertexLighting(int vindex)
{
	int ii = vindex;
	if (ViewVertexList[ii].IsLit == false)
	{
		ViewVertexList[ii].IsLit = true;
		
		Vector4 Light4 = LightPosition - ViewVertexList[ii].pos;
		Vector3 Light(Light4.x, Light4.y, Light4.z); 
		Light.Normalize();
		
		Vector3 View(-ViewVertexList[ii].pos.x, -ViewVertexList[ii].pos.y, -ViewVertexList[ii].pos.z);
		View.Normalize();

		Vector3 Half = Light + View;
		Half.Normalize();

		float DInner = Light * ViewVertexList[ii].normal;
		if (DInner < 0) DInner = 0;
		
		float SInner = Half * ViewVertexList[ii].normal;
		if (DInner == 0) SInner = 0;
		
		int SpecularPower = MATMAN.MaterialList[MATMAN.CurrentMaterial].shiness;
		
		float Specular = 1.0f;
		for (int i = 0; i < SpecularPower; i++)
		{
			Specular *= SInner;
		}
		
		TLVertexList[ii].diffuse.r = DInner * MATMAN.MaterialList[MATMAN.CurrentMaterial].diffuse.r * 1.0f;
		TLVertexList[ii].diffuse.g = DInner * MATMAN.MaterialList[MATMAN.CurrentMaterial].diffuse.g * 1.0f;
		TLVertexList[ii].diffuse.b = DInner * MATMAN.MaterialList[MATMAN.CurrentMaterial].diffuse.b * 1.0f;
		
		TLVertexList[ii].diffuse.r += Specular * MATMAN.MaterialList[MATMAN.CurrentMaterial].specular.r * 1.0f;
		TLVertexList[ii].diffuse.g += Specular * MATMAN.MaterialList[MATMAN.CurrentMaterial].specular.g * 1.0f;
		TLVertexList[ii].diffuse.b += Specular * MATMAN.MaterialList[MATMAN.CurrentMaterial].specular.b * 1.0f;
		
		if (TLVertexList[ii].diffuse.r > 1.0f) TLVertexList[ii].diffuse.r = 1.0f;
		if (TLVertexList[ii].diffuse.g > 1.0f) TLVertexList[ii].diffuse.g = 1.0f;
		if (TLVertexList[ii].diffuse.b > 1.0f) TLVertexList[ii].diffuse.b = 1.0f;

	}
}

void CRenBuffer::VertexLightingBack(int vindex)
{
	int ii = vindex;
	if (ViewVertexList[ii].IsLit == false)
	{
		ViewVertexList[ii].IsLit = true;
		Vector4 Light = LightPosition - ViewVertexList[ii].pos;
		float length = (float)sqrt(Light.x * Light.x + Light.y * Light.y + Light.z * Light.z);
		Light /= length;
		float Inner = Vector3(Light.x, Light.y, Light.z) * -ViewVertexList[ii].normal;
		if (Inner < 0) Inner = 0;
		TLVertexList[ii].diffuse = Vector3(Inner, Inner, Inner);
	}
}

