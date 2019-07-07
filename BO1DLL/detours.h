#pragma once

#include "esp.h"

enum DetourAddresses : DWORD
{
	Menu_PaintAll_a						= 0x6034B0,
	CL_CreateNewCommands_a				= 0x0080E510
};

extern void(__cdecl *Menu_PaintAll)(int localClientNum, UiContext *dc);
extern void(__cdecl *CL_CreateNewCommands)(int localClientNum);

void DetourFunction(DWORD targetFunction, DWORD detourFunction);
void DetourRemove(DWORD targetFunction, DWORD detourFunction);
void RemoveDetour(QWORD bytes);
void InsertDetour(LPVOID targetFunction, LPVOID detourFunction);
void Menu_PaintAllDetour(int localClientNum, UiContext *dc);
void CL_CreateNewCommandsDetour(int localClientNum);