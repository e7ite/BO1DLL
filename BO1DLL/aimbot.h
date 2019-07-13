#pragma once

#include "structures.h"

namespace Aimbot
{
	extern vec3_t targetAngles;

	bool Execute();
	bool ValidateTarget(centity_s *cent);
	int GetTarget();
	void RemoveSpread(playerState_s *ps, usercmd_s *cmd);
	void FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle,
		float oldForwardmove, float oldRightmove);
}

bool InGame();
float Distance3D(vec3_t c1, vec3_t c2);

float DegreesToRadians(float deg);
float pi();
int AngleToShort(float x);
float ShortToAngle(int x);
