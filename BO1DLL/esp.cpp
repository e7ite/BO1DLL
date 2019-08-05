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

void DrawName(centity_s *cent, float head[2],const float *color)
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

void DrawEntityESP(centity_s *cent, rectDef_s *mapRect)
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

	WorldPosToCompass(cent, mapRect, &rect);

	UI_DrawHandlePic(scrPlace, rect.x, rect.y, rect.w, rect.h,
		0, 0, Colors::white, material);
}

void DrawPlayerESP(centity_s *cent, rectDef_s *mapRect)
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
	WorldPosToCompass(cent, mapRect, &rect);

	CG_DrawRotatedPic(scrPlace, rect.x, rect.y, rect.w, rect.h, 0, 0,
		AngleNormalize360(cgameGlob->refdefViewAngles[1] - 
		cent->pose.angles[1]),
		color, Material_RegisterHandle("compassping_player", 0));
}

void RenderESP()
{
	if (InGame())
	{
		rectDef_s rect =
		{
			dc->screenDimensions[0] / 2 / scrPlace->scaleVirtualToFull[0],
			dc->screenDimensions[1] / 2 / scrPlace->scaleVirtualToFull[1],
			100, 100, 0, 0
		};
		rectDef_s parentRect = { 0, 0, 0x44200000, 0x43F00000, 0, 0 };

		CG_CompassDrawPlayerMap(0, 0, &parentRect, &rect,
			cgameGlob->compassMapMaterial, Colors::white, 1);
		CG_CompassDrawVehicles(0, 0, 14, &rect, &rect, Colors::white);
		CG_CompassDrawVehicles(0, 0, 13, &rect, &rect, Colors::white);
		CG_CompassDrawHelicopter(0, 0, 12, &rect, &rect, Colors::white);
		CG_CompassDrawPlayer(0, 0, &rect, &rect, 
			Material_RegisterHandle("compassping_player", 0), Colors::white);

		for (__int32 i = 0; i < 1024; ++i)
		{w
			centity_s *cent = CG_GetEntity(0, i);
			
			if (!(cent->alive & 2)) 
				continue;

			if (Aimbot::ValidateTarget(cent))
				DrawPlayerESP(cent, &rect);
			else if (cent->nextState.eType == 3 || cent->nextState.eType == 4)
				DrawEntityESP(cent, &rect);
		}
	}
}