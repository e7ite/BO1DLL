#pragma once

#include "esp.h"

enum DetourAddresses : DWORD
{
	Menu_PaintAll_a						= 0x6034B0,
	CL_CreateNewCommands_a				= 0x80E510,
	CL_WritePacket_a					= 0x5DEC90,
	CL_DrawStretchPic_a					= 0x4F4650,
};

extern void(__cdecl *Menu_PaintAll)(int localClientNum, UiContext *dc);
extern void(__cdecl *CL_CreateNewCommands)(int localClientNum);
extern void(__cdecl *CL_WritePacket)(int localClientNum);
extern void(__cdecl *CL_DrawStretchPic)(ScreenPlacement *scrPlace, float x,
	float y, float w, float h, int horzAlign, int vertAlign,
	float s1, float t1, float s2, float t2,
	const float *color, struct Material *material);

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(LPVOID targetFunction, LPVOID detourFunction);
void Menu_PaintAllDetour(int localClientNum, UiContext *dc);
void CL_CreateNewCommandsDetour(int localClientNum);
void CL_WritePacketDetour(int localClientNum);
void CL_DrawStretchPicDetour(ScreenPlacement *scrPlace, float x,
	float y, float w, float h, int horzAlign, int vertAlign,
	float s1, float t1, float s2, float t2,
	const float *color, Material *material);