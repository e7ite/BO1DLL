#include "esp.h"

void DrawBorderBox(float head[2], float foot[2], const float *color)
{
	float boxHeight = (foot[1] - head[1]), boxWidth = boxHeight / 2.2f;

	CG_DrawRotatedPicPhysical(GameData::scrPlace,
		(foot[0] - (boxWidth / 2)), (foot[1] - boxHeight), boxWidth, 1, 0,
		color, Material_RegisterHandle("white", 0)); //top
	CG_DrawRotatedPicPhysical(GameData::scrPlace,
		(foot[0] - (boxWidth / 2)), foot[1], boxWidth, 1, 0, color,
		Material_RegisterHandle("white", 0)); //bottom
	CG_DrawRotatedPicPhysical(GameData::scrPlace,
		(foot[0] - (boxWidth / 2)), (foot[1] - boxHeight),
		1, boxHeight, 0, color, Material_RegisterHandle("white", 0)); //left
	CG_DrawRotatedPicPhysical(GameData::scrPlace,
		(foot[0] + (boxWidth / 2)), (foot[1] - boxHeight), 1,
		boxHeight, 0, color, Material_RegisterHandle("white", 0)); //right
}

void DrawName(centity_s *cent, float head[2], const float *color)
{
	float namePos[2];
	Font_s *fontPointer = CL_RegisterFont(normalFont.dir, 0);
	const char *name = cgameGlob->clients[cent->nextState.number].name;
	WorldPosToScreenPos(0, cent->pose.origin, namePos);

	RenderGameText(name,
		AlignText(name, normalFont, 0.3f, 
			namePos[0], ALIGN_CENTER, 0, 0, &fontPointer),
		head[1], 0.3f, Colors::white, fontPointer, 0);
}

void ScaleWeapon(WeaponVariantDef *weap, rectDef_s *rect)
{
	if (weap->weapDef->hudIconRatio)
		if (weap->weapDef->hudIconRatio == 1)
		{
			rect->x -= rect->w;
			rect->w *= 2.0f;
		}
		else
		{
			rect->x -= rect->w * 3.0f;
			rect->w *= 4.0f;
		}
}

void DrawEntityESP(centity_s *cent, int compassType, const rectDef_s *parentRect,
	const rectDef_s *mapRect)
{
	WeaponVariantDef *weap = BG_GetWeaponVariantDef(
		cent->nextState.eType == 3
		? cent->nextState.itemIndex % 2048
		: cent->nextState.weapon
	);

	if (!weap)
		return;

	float origin[2];
	WorldPosToScreenPos(0, cent->pose.origin, origin);
	rectDef_s rect = { origin[0], origin[1], 30, 30, 0, 0 };
	
	bool scavenger = Variables::scavengerESP
		&& BG_HasPerk(cgameGlob->clientNum, "specialty_scavenger")
		&& !strcmp(weap->szInternalName, "scavenger_item_mp");

	struct Material *material;
	if (scavenger)
	{
		material = Material_RegisterHandle("perk_scavenger", 0);

		CG_DrawRotatedPicPhysical(scrPlace, origin[0], origin[1],
			rect.w, rect.h, 0, Colors::white,
			material);
	}
	else
	{
		if (!weap->weapDef->hudIcon
			|| (cent->nextState.eType == 3 && !Variables::pickupESP)
			|| (cent->nextState.eType == 4 && !Variables::missileESP))
			return;

		material = weap->weapDef->hudIcon;
		ScaleWeapon(weap, &rect);

		if (weap)
			CG_DrawRotatedPicPhysical(scrPlace, rect.x, rect.y,
				rect.w, rect.h, 0, Colors::white,
				material);
	}

	CalcCompassFriendlySize(0, &rect.w, &rect.h);
	if (!scavenger)
		ScaleWeapon(weap, &rect);

	WorldPosToCompass(cent, compassType, mapRect, &rect);

	UI_DrawHandlePic(scrPlace, rect.x, rect.y, rect.w, rect.h,
		0, 0, Colors::white, material);
}

void DrawPlayerESP(centity_s *cent, int compassType, const rectDef_s *parentRect, 
	const rectDef_s *mapRect)
{
	if (!Variables::friendlyESP && !Variables::enemyESP)
		return;

	vec3_t head, feet;
	float headScreen[2], feetScreen[2];
	const float *color = Colors::red;

	if (strcmp(cgs->gametype, "dm")
		&& cgameGlob->clients[cent->nextState.number].team
		== cgameGlob->clients[cgameGlob->clientNum].team)
		if (!Variables::friendlyESP)
			return;
		else
			color = Colors::green;
	else if (AimTarget_IsTargetVisible(cent, "j_head"))
		color = Colors::blue;

	AimTarget_GetTagPos(cent, "j_helmet", head);
	feet = cent->pose.origin;

	WorldPosToScreenPos(0, head, headScreen);
	WorldPosToScreenPos(0, feet, feetScreen);

	DrawName(cent, headScreen, Colors::white);
	DrawBorderBox(headScreen, feetScreen, color);

	WeaponVariantDef *weap = BG_GetWeaponVariantDef(cent->nextState.weapon);

	rectDef_s rect = { feetScreen[0], feetScreen[1], 20, 20, 0, 0 };
	ScaleWeapon(weap, &rect);

	CG_DrawRotatedPicPhysical(scrPlace, rect.x - rect.w / 2,
		rect.y, rect.w, rect.h, 0, Colors::white,
		weap->weapDef->hudIcon);

	CalcCompassFriendlySize(0, &rect.w, &rect.h);

	WorldPosToCompass(cent, compassType, mapRect, &rect);

	float angle;
	if (!compassType)
		angle = AngleNormalize360(cgameGlob->refdefViewAngles[1] -
			cent->pose.angles[1]);
	else
		angle = AngleNormalize360(cgameGlob->compassNorthYaw -
			cent->pose.angles[1]);

	CG_DrawRotatedPic(scrPlace, rect.x, rect.y, rect.w, rect.h, 0, 0,
		angle, color, Material_RegisterHandle("compassping_player", 0));
}

void RenderESP(int compassType, const rectDef_s *parentRect, const rectDef_s *rect)
{
	if (InGame())
	{
		CG_CompassDrawPlayerMap(0, compassType, parentRect, rect,
			cgameGlob->compassMapMaterial, Colors::white, 0);
		CG_CompassDrawBorder(0, compassType, parentRect, rect,
			Material_RegisterHandle("white", 0), Colors::red);

		for (__int32 i = 1023; i >= 0; --i)
		{
			centity_s *cent = CG_GetEntity(0, i);
			
			if (!(cent->alive & 2)) 
				continue;

			if (Aimbot::ValidateTarget(cent))
				DrawPlayerESP(cent, compassType, parentRect, rect);
			else if (cent->nextState.eType == 3 || cent->nextState.eType == 4)
				DrawEntityESP(cent, compassType, parentRect, rect);
		}

		CG_CompassDrawVehicles(0, compassType, 14, parentRect, rect, Colors::white);
		CG_CompassDrawVehicles(0, compassType, 13, parentRect, rect, Colors::white);
		CG_CompassDrawHelicopter(0, compassType, 12, parentRect, rect, Colors::white);
		CG_CompassDrawGuidedMissle(0, compassType, 4, parentRect, rect, Colors::white);
		CG_CompassDrawTurrets(0, compassType, 11, parentRect, rect, Colors::white);
		CG_CompassDrawIncomingArtilleryIcon(0, compassType, parentRect, rect, 
			Colors::white);
		CG_CompassDrawArtilleryIcon(0, compassType, parentRect, rect, Colors::white);
		CG_CompassDrawPlayerPointers_MiniMap(0, compassType, parentRect, rect,
			0, Colors::white);
		CG_CompassDrawDogs(0, compassType, 17, parentRect, rect, (Material*)0x2686568,
			Colors::white);
		CG_CompassDrawPlayer(0, compassType, parentRect, rect,
			Material_RegisterHandle("compassping_player", 0), Colors::white);
	}
}