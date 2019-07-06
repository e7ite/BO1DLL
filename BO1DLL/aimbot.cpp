#include "aimbot.h"

vec3_t Aimbot::targetAngles;
bool Aimbot::gotTarget;

float Distance3D(vec3_t c1, vec3_t c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrtf(dx * dx + dy * dy + dz * dz);
}

bool ExecuteAimbot()
{
	if (InGame())
	{
		centity_s *target = GetAimbotTarget();
		if (target)
		{
			AimTarget_GetTagPos(target, "j_head", Aimbot::targetAngles);

			vectoangles(Aimbot::targetAngles - cgameGlob->refdef.vieworg,
				Aimbot::targetAngles);

			Aimbot::targetAngles -= cgameGlob->predictedPlayerState.delta_angles;

			SetAngles(Aimbot::targetAngles);

			return true;
		}
	}

	return false;
}

bool ValidTarget(centity_s *cent)
{
	return cent->nextState.number != cgameGlob->clientNum
		&& cent->nextState.eType == 1
		&& !(cent->lerp.eFlags & 0x20) && !(cent->lerp.eFlags & 0x4000);
}

centity_s* GetAimbotTarget()
{
	centity_s *cent = nullptr;
	float vec[3];
	float closestDistance = static_cast<float>(INT_MAX);

	for (__int32 i = 0; i < 1024; ++i)
	{
		centity_s *tmpcent = CG_GetEntity(0, i);
		if (ValidTarget(tmpcent)
			&& AimTarget_GetTagPos(tmpcent, "j_head", vec))
		{
			float distance = Distance3D(
				cgameGlob->predictedPlayerState.origin, vec);
			if (distance < closestDistance)
			{
				cent = tmpcent;
				closestDistance = distance;
			}
		}
	}

	return cent;
}

void SetAngles(const vec3_t& angles)
{
	clientActive->viewangles[0] = angles.pitch;
	clientActive->viewangles[1] = angles.yaw;
	clientActive->viewangles[2] = angles.roll;
}

bool InGame()
{
	return Dvar_FindVar("cl_ingame")->current.enabled && !cgameGlob->inKillCam;
}