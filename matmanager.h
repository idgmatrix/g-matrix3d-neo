#ifndef MATMANAGER_H
#define MATMANAGER_H

#include "material.h"

class CMatManager{
public:
	int NumMaterial;
	int CurrentMaterial;
	Material * MaterialList;
	
	CMatManager()
	{
		NumMaterial = 0;
		CurrentMaterial = 0;
		MaterialList = NULL;
	}
	~CMatManager()
	{
		NumMaterial = 0;
		CurrentMaterial = 0;
		if (MaterialList) delete [] MaterialList;
	}

	void Init(int num)
	{
		NumMaterial = num;
		MaterialList = new Material[num];
	}
	void Destroy(void)
	{
		NumMaterial = 0;
		CurrentMaterial = 0;
		if (MaterialList) delete [] MaterialList;
		MaterialList = NULL;
	}
	void SetMaterial(int num)
	{
		if (0 <= num && num < NumMaterial) 
			CurrentMaterial = num;
	}

};

#endif

