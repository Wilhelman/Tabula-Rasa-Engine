#ifndef __trCAMERA3D_H__
#define __trCAMERA3D_H__

#include "Globals.h" 

#include "MathGeoLib/MathBuildConfig.h"
#include "MathGeoLib/MathGeoLib.h"
#include "MathGeoLib/MathGeoLibFwd.h"

#include "trApp.h"

class trCamera3D : public trModule
{
public:
	trCamera3D();
	~trCamera3D();

	bool Start();
	bool Update(float dt);
	bool CleanUp();

	void Look(const vec &Position, const vec &Reference, bool RotateAroundReference = false);
	void LookAt(const vec &Spot);
	void Move(const vec &Movement);
	float* GetViewMatrix();

private:

	void CalculateViewMatrix();

public:

	vec X, Y, Z, Position, Reference;

private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;

	math::float3x3 transform;
};

#endif