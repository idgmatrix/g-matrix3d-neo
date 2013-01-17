#ifndef MATERIAL_H
#define MATERIAL_H

class Material {
public:
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	Vector3 emmision;
	int shiness;
	int TEXID;

	Material()
	{
		ambient = Vector3(0,0,0);
		diffuse = Vector3(0.7f,0.7f,0.7f);
		specular = Vector3(0,0,0);
		emmision = Vector3(0,0,0);
		shiness = 0;
		TEXID = 0;
	}
	~Material(){}

	void SetMaterial(Vector3 a, Vector3 d, Vector3 s, int sh = 0, Vector3 e = Vector3(0,0,0), int t = 0)
	{
		ambient = a;
		diffuse = d;
		specular = s;
		emmision = e;
		shiness = sh;
		TEXID = t;
	}
};

#endif

