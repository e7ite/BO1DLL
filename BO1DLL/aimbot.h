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
centity_s* GetAimbotTarget();
void SetAngles(const vec3_t& angles);
