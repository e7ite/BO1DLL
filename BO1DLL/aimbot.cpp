#include "aimbot.h"

vec3_t Aimbot::targetAngles;

bool Aimbot::Execute()
{
	if (InGame())
	{
		int target = GetTarget();
		if (target != -1)
		{
			AimTarget_GetTagPos(CG_GetEntity(0, target), 
				"j_head", targetAngles);

			vectoangles(targetAngles - cgameGlob->refdef.vieworg, targetAngles);
			targetAngles -= cgameGlob->predictedPlayerState.delta_angles;

			return true;
		}
	}

	return false;
}

bool Aimbot::ValidateTarget(centity_s *cent)
{
	return cent->nextState.number != cgameGlob->clientNum
		&& cent->nextState.eType == 1
		&& cent->alive & 2;
}

int Aimbot::GetTarget()
{
	int target = -1;
	float closestDistance = 9999999999.0f;

	for (__int32 i = 0; i < cgs->maxclients; ++i)
	{
		float vec[3];
		centity_s *cent = CG_GetEntity(0, i);
		
		if (ValidateTarget(cent)
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

void Aimbot::FixMovement(usercmd_s *cmd, float currentAngle, float oldAngle,
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
	
	float deltaRad = DegreesToRadians(deltaView);
	float rightDeltaRad = DegreesToRadians(deltaView + 90.f);
	cmd->forwardmove = ClampChar(int(
		cosf(deltaRad) * oldForwardmove + cosf(rightDeltaRad) * oldRightmove
	));
	cmd->rightmove = ClampChar(int(
		sinf(deltaRad) * oldForwardmove + sinf(rightDeltaRad) * oldRightmove
	));
}

void Aimbot::RemoveSpread(playerState_s *ps, usercmd_s *cmd)
{
	float minSpread, maxSpread, finalSpread, range;
	vec3_t viewOrg, viewAxis[3], spreadEnd, spreadDir, spreadFix;
	float aimSpreadScale = ps->aimSpreadScale / 255.0f;
	WeaponDef *weap = BG_GetWeaponDef(ps->weapon);

	if (!CG_GetPlayerViewOrigin(0, ps, viewOrg))
		return;

	if (cgameGlob->renderingThirdPerson == 1)
		AngleVectors(cgameGlob->clients[cgameGlob->clientNum].playerAngles,
			viewAxis[0], viewAxis[1], viewAxis[2]);
	else
	{
		float gunAngles[3] = { cgameGlob->gunPitch, cgameGlob->gunYaw, 0.0f };
		AngleVectors(gunAngles, viewAxis[0], viewAxis[1], viewAxis[2]);
	}

	BG_GetSpreadForWeapon(ps, weap, &minSpread, &maxSpread);
	minSpread = ps->fWeaponPosFrac == 1.0f
		? weap->fAdsSpread : minSpread;
	finalSpread = (maxSpread - minSpread) * aimSpreadScale + minSpread;

	range = weap->weapClass == 3 
		? weap->fMinDamageRange : ((dvar_s*)0x3732838)->current.integer;

	CG_BulletEndpos(ps->commandTime, finalSpread, viewOrg, spreadEnd, spreadDir,
		viewAxis[0], viewAxis[1], viewAxis[2], range);

	vectoangles(spreadDir, spreadFix);

	cmd->angles[0] += AngleToShort(cgameGlob->gunPitch - spreadFix[0]);
	cmd->angles[1] += AngleToShort(cgameGlob->gunYaw - spreadFix[1]);
}

bool InGame()
{
	return Dvar_FindVar("cl_ingame")->current.enabled && !cgameGlob->inKillCam;
}

float Distance3D(vec3_t c1, vec3_t c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return sqrtf(dx * dx + dy * dy + dz * dz);
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

void Vec3Normalize(float *v)
{
	float length = sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	v[0] /= length;
	v[1] /= length;
	v[2] /= length;
}