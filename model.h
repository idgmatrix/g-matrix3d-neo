#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"

struct Model{
	int NumMesh;
	Mesh *MeshList;

	Model()
	{
		NumMesh = 0;
		MeshList = NULL;
	}
	~Model()
	{
		if (MeshList) delete[] MeshList;
	}
	
	void CreateModel(int num)
	{
		NumMesh = num;
		MeshList = new Mesh[num];
	}
	int Render(void);
};

#endif

