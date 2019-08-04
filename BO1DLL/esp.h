#pragma once

#include "aimbot.h"
#include "menu.h"

void DrawBorderBox(float head[2], float foot[2], const float *color);
void DrawName(centity_s *cent, float head[2], const float *color);
void ScaleWeapon(WeaponVariantDef *weap, rectDef_s *rect);
void DrawEntityESP(centity_s *cent, rectDef_s *mapRect);
void DrawPlayerESP(centity_s *cent, rectDef_s *mapRect);
void RenderESP();