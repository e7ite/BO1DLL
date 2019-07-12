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

void DrawName(centity_s *entity, float head[2], float foot[2], const float *color)
{
	Font_s *fontPointer;
	float namePos[2];
	const char *name = cgameGlob->clients[entity->nextState.number].name;
	WorldPosToScreenPos(0, entity->pose.origin, namePos);

	RenderGameText(name,
		AlignText(name, normalFont, 0.3f, 
			namePos[0], ALIGN_CENTER, 0, 0, &fontPointer),
		namePos[1], 0.3f, Colors::white, fontPointer, 0);
}

void RenderESP()
{
	if (InGame())
	{
		vec3_t head, foot;
		float headScreen[2], feetScreen[2];
		float closest = static_cast<float>(INT_MAX);

		for (__int32 i = 0; i < 1024; ++i)
		{
			centity_s *cent = CG_GetEntity(0, i);

			if (!(cent->alive & 2))
				continue;

			if (ValidTarget(cent))
			{	
				const float *color = Colors::red;

				if (strcmp(cgs->gametype, "dm") 
					&& cgameGlob->clients[i].team
						== cgameGlob->clients[cgameGlob->clientNum].team)
					color = Colors::green;
				else if (AimTarget_IsTargetVisible(cent, "j_head"))
					color = Colors::blue;

				AimTarget_GetTagPos(cent, "j_helmet", head);
				foot = cent->pose.origin;

				WorldPosToScreenPos(0, head, headScreen);
				WorldPosToScreenPos(0, foot, feetScreen);

				DrawName(cent, headScreen, feetScreen, Colors::white);
				DrawBorderBox(headScreen, feetScreen, color);
			}
			else if (cent->nextState.eType == 3 || cent->nextState.eType == 4)
			{
				WeaponVariantDef *weap = BG_GetWeaponVariantDef(
					cent->nextState.eType == 3 
					? cent->nextState.itemIndex % 2048
					: cent->nextState.weapon
				);

				if (!weap)
					continue;

				float origin[2];
				WorldPosToScreenPos(0, cent->pose.origin, origin);

				if (!(BG_HasPerk(cgameGlob->clientNum, "specialty_scavenger") 
					&&!strcmp(weap->szInternalName, "scavenger_item_mp")))
				{
					if (!weap->weapDef->hudIcon)
						continue;

					rectDef_s rect = { origin[0], origin[1], 30, 30, 0, 0 };
					if (weap->weapDef->hudIconRatio)
						if (weap->weapDef->hudIconRatio == 1)
						{
							rect.x = rect.x - rect.w;
							rect.w = rect.w * 2.0f;
						}
						else
						{
							rect.x = rect.x - rect.w * 3.0f;
							rect.w = rect.w * 4.0f;
						}

					if (weap)
						CG_DrawRotatedPicPhysical(scrPlace, rect.x, rect.y,
							rect.w, rect.h, 0, Colors::white, 
							weap->weapDef->hudIcon);
				}
				else
				{
					CG_DrawRotatedPicPhysical(scrPlace, origin[0], origin[1],
						30, 30, 0, Colors::white,
						Material_RegisterHandle("perk_scavenger", 0));
				}
			}
		}
	}
}