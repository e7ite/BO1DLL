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
void Vec3Normalize(float *v);
//void Aimbot::PredictPosition(centity_s *cent, vec3_t pos, int scale, bool z)
//{
//	vec3_t enemyVelocityTemp, enemyVelocity;
//
//	VectorSubtract(TargetEntity->NewOrigin, TargetEntity->OldOrigin, EnemyVelocityTemp);
//
//	EnemyVelocity.x = EnemyVelocityTemp[0];
//	EnemyVelocity.y = EnemyVelocityTemp[1];
//	EnemyVelocity.z = EnemyVelocityTemp[2];
//
//	Vector3 CalcVec = EnemyVelocity.Normalize();
//
//	CalcVec.x *= Scale;
//	CalcVec.y *= Scale;
//	CalcVec.z *= Scale;
//
//	float TempOld = BonePos[2];
//
//	BonePos[0] += CalcVec.x;
//	BonePos[1] += CalcVec.y;
//	BonePos[2] += CalcVec.z;
//
//	if (NotAllowZ)
//		BonePos[2] = TempOld;
//}