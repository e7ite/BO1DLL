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
			if (ValidTarget(cent))
			{
				const float *color = Colors::blue;
				AimTarget_GetTagPos(cent, "j_head", head);

				foot = cent->pose.origin;

				WorldPosToScreenPos(0, head, headScreen);
				WorldPosToScreenPos(0, foot, feetScreen);

				DrawBorderBox(headScreen, feetScreen, color);
			}
		}
	}
}