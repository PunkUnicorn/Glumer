// Glumer.cpp
// Copyright (c) 2014
// Author: Matthew Cocks
// License: Attribution 4.0 International (CC BY 4.0)
#define BUILDING_DLL
#include "cHUD_Colour.h"
#include "cGlumShapeFactory.h"
#include <vector>
#include <map>
#include <SDL.h>
#include "cGlumShapeFactory.h"
#include "STDCALL.h"

#include "DLL_PUBLIC.h"
#include "Glumer.h"

#define M_PI   3.14159265358979323846264338327950288

using namespace Glumer;

extern "C" DLL_PUBLIC void STDCALL glhFrustumf2(float matrix[16], float left, float right, float bottom, float top,
	float znear, float zfar)
{
	float temp, temp2, temp3, temp4;
	temp = 2.0f * znear;
	temp2 = right - left;
	temp3 = top - bottom;
	temp4 = zfar - znear;
	matrix[0] = temp / temp2;
	matrix[1] = 0.0f;
	matrix[2] = 0.0f;
	matrix[3] = 0.0f;
	matrix[4] = 0.0f;
	matrix[5] = temp / temp3;
	matrix[6] = 0.0f;
	matrix[7] = 0.0f;
	matrix[8] = (right + left) / temp2;
	matrix[9] = (top + bottom) / temp3;
	matrix[10] = (-zfar - znear) / temp4;
	matrix[11] = -1.0f;
	matrix[12] = 0.0f;
	matrix[13] = 0.0f;
	matrix[14] = (-temp * zfar) / temp4;
	matrix[15] = 0.0f;
}

// Matrix will receive the calculated perspective matrix.
// You would have to upload to your shader
// or use glLoadMatrixf if you aren't using shaders.
extern "C" DLL_PUBLIC void STDCALL glhPerspectivef2(float matrix[16], float fovyInDegrees, float aspectRatio, float znear, float zfar)
{
	float ymax, xmax;
	//float temp, temp2, temp3, temp4;
	ymax = znear * tanf(fovyInDegrees * (float)M_PI / 360.0f);
	// ymin = -ymax;
	// xmin = -ymax * aspectRatio;
	xmax = ymax * aspectRatio;
	glhFrustumf2(matrix, -xmax, xmax, -ymax, ymax, znear, zfar);
}

extern "C" DLL_PUBLIC void STDCALL glPerspectiveProxy(float fovyInDegrees, float aspectRatio, float znear, float zfar)
{
	gluPerspective(fovyInDegrees, aspectRatio, znear, zfar);
}

extern "C" DLL_PUBLIC void STDCALL gluLookAtProxy(float eyex,
	float eyey,
	float eyez,
	float centerx,
	float centery,
	float centerz,
	float upx,
	float upy,
	float upz)
{
	gluLookAt(eyex,
		eyey,
		eyez,
		centerx,
		centery,
		centerz,
		upx,
		upy,
		upz);
	//gluPerspective(fovyInDegrees, aspectRatio, znear, zfar);
}

