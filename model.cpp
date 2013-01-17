/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#include "model.h"
#include "Mesh.h"

int Model::Render(void)
{
	for (int i = 0; i < NumMesh; i++)
	{
		MeshList[i].Render();
	}

	return 0;
}

