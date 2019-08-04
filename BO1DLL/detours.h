#pragma once

#include "esp.h"

enum DetourAddresses : DWORD
{
	Menu_PaintAll_a						= 0x6034B0,
	CL_CreateNewCommands_a				= 0x80E510,
	CL_WritePacket_a					= 0x5DEC90,
	CL_DrawStretchPic_a					= 0x4F4650,
	Menu_HandleMouseMove_a				= 0x897F60,
	Menu_HandleKey_a					= 0x5AEF80,
	CL_MouseMove_a						= 0x80DB40,
	CL_MouseMoveRet_a					= 0x80DB51,
	UI_KeysBypassMenu_a					= 0x4F3550,
};

extern void(__cdecl *Menu_PaintAll)(int localClientNum, UiContext *dc);
extern void(__cdecl *CL_CreateNewCommands)(int localClientNum);
extern void(__cdecl *CL_WritePacket)(int localClientNum);
extern void(__cdecl *CL_DrawStretchPic)(ScreenPlacement *scrPlace, float x,
	float y, float w, float h, int horzAlign, int vertAlign,
	float s1, float t1, float s2, float t2,
	const float *color, struct Material *material);
extern void(__cdecl *Menu_HandleKey)(int localClientNum, UiContext *dc,
	struct menuDef_t *menu, int key, int down);
extern void(__cdecl *Menu_HandleMouseMove)(int localClientNum, UiContext *dc,
	struct menuDef_t *menu);
extern void(__cdecl *Menus_ShowByName)(UiContext *dc, const char *windowName);
extern __usercall CL_MouseMove;	
extern bool(__cdecl *UI_KeysBypassMenu)(int localClientNum);

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
void Menu_HandleKeyDetour(int localClientNum, UiContext *dc,
	struct menuDef_t *menu, int key, unsigned int down);
void Menu_HandleMouseMoveDetour(int localClientNum, UiContext *dc,
	struct menuDef_t *menu);
void Menus_ShowByNameDetour(UiContext *dc, const char *windowName);
void CL_MouseMoveStub(int localClientNum, usercmd_s *cmd);
bool CL_MouseMoveDetour(int localClientNum, usercmd_s *cmd);