#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"

typedef enum _LIGHTTYPE {
	LT_NO = 0,
	LT_POINT = 1,
	LT_DIRECTION,
	LT_SPOT,
	FORCE_DWORD = 0x7fffffff
} LIGHTTYPE;

class Light {
public:
	LIGHTTYPE type;
	
	Vector3 pos;
	Vector3 dir;
	
	Vector3 ambient;
	Vector3 diffuse;
	Vector3 specular;
	
	float spot_inner;
	float spot_outer;

	Light(){}
	~Light(){}
};

#endif