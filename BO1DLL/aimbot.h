#pragma once

#include "structures.h"

namespace Aimbot
{
	extern vec3_t targetAngles;
	extern bool gotTarget;
}

bool InGame();
float Distance3D(vec3_t c1, vec3_t c2);
bool ExecuteAimbot();
bool ValidTarget(centity_s *cent);
int GetAimbotTarget();
void RemoveSpread(playerState_s *ps, usercmd_s *cmd);
void SetAngles(const vec3_t& angles);
void FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle,
	float oldForwardmove, float oldRightmove);
float DegreesToRadians(float deg);
float pi();
int AngleToShort(float x);
float ShortToAngle(int x);
