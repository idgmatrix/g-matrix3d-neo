/******************************************************
	g-Matrix3D Neo Engine
	Copyright (c)2003 Kim Seong Wan (kaswan, Âð»§±Í½Å)
	
	E-mail: kaswan@hitel.net
	http://www.g-matrix.pe.kr

*******************************************************/
#ifndef UTIL_H
#define UTIL_H

#ifndef PI
#define PI 3.1415926535897932384626f
#endif

#define DEG2RAD(x)  ((x) * PI / 180.0f)
#define RAD2DEG(x)  ((x) * 180.0f / PI)

inline int ftoi(float f)
{
	int i;

#ifdef _MSC_VER
	_asm fld	f
	_asm fistp	i
#else
	asm ("fld %0" : : "m" (f));
	asm ("fistpl %0" : :"m" (i));
#endif

	return i;
}


#endif


