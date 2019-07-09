#include "aimbot.h"

vec3_t Aimbot::targetAngles;
bool Aimbot::gotTarget;

float Distance3D(vec3_t c1, vec3_t c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrtf((float)((dx * dx) + (dy * dy) + (dz * dz)));
}

bool ExecuteAimbot()
{
	if (InGame())
	{
		int target = GetAimbotTarget();
		if (target != -1)
		{
			AimTarget_GetTagPos(CG_GetEntity(0, target), "j_head", Aimbot::targetAngles);

			vectoangles(Aimbot::targetAngles - cgameGlob->refdef.vieworg,
				Aimbot::targetAngles);
			Aimbot::targetAngles -= cgameGlob->predictedPlayerState.delta_angles;

			//SetAngles(Aimbot::targetAngles);

			return true;
		}
	}

	return false;
}

bool ValidTarget(centity_s *cent)
{
	return cent->nextState.number != cgameGlob->clientNum
		&& cent->nextState.eType == 1
		&& cent->alive & 2;
}

int GetAimbotTarget()
{
	int target = -1;
	float vec[3];
	float closestDistance = static_cast<float>(INT_MAX);

	for (__int32 i = 0; i < 1024; ++i)
	{
		centity_s *cent = CG_GetEntity(0, i);
		if (ValidTarget(cent)
			&& (cgameGlob->clients[i].team 
				!= cgameGlob->clients[cgameGlob->clientNum].team
				|| !strcmp(cgs->gametype, "dm"))
			&& AimTarget_GetTagPos(cent, "j_head", vec)
			&& AimTarget_IsTargetVisible(cent, "j_head"))
		{
			float distance = Distance3D(
				cgameGlob->predictedPlayerState.origin, vec);
			if (distance < closestDistance)
			{
				target = i;
				closestDistance = distance;
			}
		}
	}

	return target;
}

void SetAngles(const vec3_t& angles)
{
	clientActive->viewangles[0] = angles.pitch;
	clientActive->viewangles[1] = angles.yaw;
	clientActive->viewangles[2] = angles.roll;
}

void FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle,
	float oldForwardmove, float oldRightmove)
{
	float deltaView = currentAngle - oldAngle, f1, f2;

	if (oldAngle < 0.f)
		f1 = 360.0f + oldAngle;
	else
		f1 = oldAngle;

	if (currentAngle < 0.0f)
		f2 = 360.0f + currentAngle;
	else
		f2 = currentAngle;

	if (f2 < f1)
		deltaView = abs(f2 - f1);
	else
		deltaView = 360.0f - abs(f1 - f2);
	deltaView = 360.0f - deltaView;

	cmd->forwardmove = ClampChar(
		cosf(DegreesToRadians(deltaView)) * oldForwardmove
		+ cosf(DegreesToRadians(deltaView + 90.f)) * oldRightmove
		);
	cmd->rightmove = ClampChar(
		sinf(DegreesToRadians(deltaView)) * oldForwardmove
		+ sinf(DegreesToRadians(deltaView + 90.f)) * oldRightmove
		);
}

void RemoveSpread(playerState_s *ps, usercmd_s *cmd)
{
	/*float minSpread, maxSpread, spreadX, spreadY, finalSpread;
	float cgSpread = cgameGlob->aimSpreadScale / 255.0f;
	WeaponDef *weap = BG_GetWeaponDef(cgameGlob->weaponSelect);

	BG_GetSpreadForWeapon(ps, weap, &minSpread, &maxSpread);
	RandomBulletDir(ps->commandTime, &spreadX, &spreadY);
	minSpread = ps->fWeaponPosFrac == 1.0f
		? weap->fAdsSpread : minSpread;
	finalSpread = cgSpread * (maxSpread - minSpread) + minSpread;

	spreadX *= finalSpread;
	spreadY *= finalSpread;

	cmd->angles[0] += AngleToShort(spreadY);
	cmd->angles[1] += AngleToShort(spreadX);*/

	float minSpread, maxSpread, finalSpread, range;
	vec3_t viewOrg, viewAxis[3], spreadEnd, spreadDir, spreadFix;
	float cgSpread = cgameGlob->aimSpreadScale / 255.0f;
	WeaponDef *weap = BG_GetWeaponDef(ps->weapon);

	if (!CG_GetPlayerViewOrigin(0, ps, viewOrg))
		return;

	if (cgameGlob->renderingThirdPerson == 1)
		AngleVectors(cgameGlob->clients[cgameGlob->clientNum].playerAngles,
			viewAxis[0], viewAxis[1], viewAxis[2]);
	else
	{
		float tmp[3] = { cgameGlob->gunPitch, cgameGlob->gunYaw, 0.0f };
		AngleVectors(tmp, viewAxis[0], viewAxis[1], viewAxis[2]);
	}

	BG_GetSpreadForWeapon(ps, weap, &minSpread, &maxSpread);
	minSpread = ps->fWeaponPosFrac == 1.0f
		? weap->fAdsSpread : minSpread;
	finalSpread = minSpread + (maxSpread - minSpread) * cgSpread;

	range = weap->weapType == 3 ? weap->fMinDamageRange : 8192.0f;

	CG_BulletEndpos(ps->commandTime, finalSpread, viewOrg, spreadEnd, spreadDir,
		viewAxis[0], viewAxis[1], viewAxis[2], range);

	vectoangles(spreadDir, spreadFix);

	cmd->angles[0] += AngleToShort(cgameGlob->gunPitch - spreadFix[0]);
	cmd->angles[1] += AngleToShort(cgameGlob->gunYaw - spreadFix[1]);
}

float DegreesToRadians(float deg)
{
	return deg * (pi() / 180.f);
}

float pi()
{
	float funcRet;

	__asm
	{
		fldpi
		fstp			funcRet
	}

	return funcRet;
}

int AngleToShort(float x)
{
	return ((int)(x * 65536 / 360) & 65535);
}

float ShortToAngle(int x)
{
	return (x * (360.0f / 65536));
}

bool InGame()
{
	return Dvar_FindVar("cl_ingame")->current.enabled && !cgameGlob->inKillCam;
}
