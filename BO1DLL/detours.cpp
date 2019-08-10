#include "detours.h"

void(__cdecl *Menu_PaintAll)(int localClientNum, UiContext *dc)
= (void(__cdecl*)(int, UiContext*))Menu_PaintAll_a;
void(__cdecl *CL_CreateNewCommands)(int localClientNum)
= (void(__cdecl*)(int))CL_CreateNewCommands_a;
void(__cdecl *CL_WritePacket)(int localClientNum)
= (void(__cdecl*)(int))CL_WritePacket_a;
void(__cdecl *CL_DrawStretchPic)(ScreenPlacement *scrPlace, float x,
	float y, float w, float h, int horzAlign, int vertAlign,
	float s1, float t1, float s2, float t2,
	const float *color, Material *material)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, int, int,
	float, float, float, float, const float*, Material*))CL_DrawStretchPic_a;
void(__cdecl *Menu_HandleKey)(int localClientNum, UiContext *dc,
	struct menuDef_t *menu, int key, int down)
= (void(__cdecl*)(int, UiContext*, menuDef_t*, int, int))Menu_HandleKey_a;
void(__cdecl *Menu_HandleMouseMove)(int localClientNum, UiContext *dc,
	struct menuDef_t *menu)
= (void(__cdecl*)(int, UiContext*, menuDef_t*))Menu_HandleMouseMove_a;
void(__cdecl *Menus_ShowByName)(UiContext *dc, const char *windowName)
= (void(__cdecl*)(UiContext*, const char*))Menus_ShowByName_a;
__usercall CL_MouseMove = (__usercall)CL_MouseMove_a;
bool(__cdecl *UI_KeysBypassMenu)(int localClientNum)
= (bool(__cdecl*)(int))UI_KeysBypassMenu_a;
void(__cdecl *CG_DrawNightVisionOverlay)(int localClientNum)
= (void(__cdecl*)(int))CG_DrawNightVisionOverlay_a;
void(__cdecl *Menu_Paint)(int localClientNum, UiContext *dc,
	struct ScreenPlacementStack *scrPlaceViewStack, struct menuDef_t *menu,
	int UI3OverrideID)
= (void(__cdecl*)(int, UiContext*, ScreenPlacementStack*, menuDef_t*,
	int))Menu_Paint_a;
float(__cdecl *CG_GetScramblerEnemyAlpha)(int localClientNum)
= (float(__cdecl*)(int))CG_GetScramblerEnemyAlpha_a;

void DetourFunction(DWORD targetFunction, DWORD detourFunction)
{
	/*Initiate Detour Transcation API*/
	DetourTransactionBegin();

	/*Enlists Current Thread in Transaction to Appropriately Update
	  Instruction Pointers for That Thread*/
	DetourUpdateThread(GetCurrentThread());

	/*Allocates the Detour for the Target Function*/
	DetourAttach(reinterpret_cast<PVOID*>(targetFunction),
		reinterpret_cast<PVOID*>(detourFunction));

	/*Overwrites the first instruction in the target function to jmp
	  to Detour before returning to target function to restore program flow*/
	DetourTransactionCommit();
}

void DetourRemove(DWORD targetFunction, DWORD detourFunction)
{
	/*Initiate Detour Transcation API*/
	DetourTransactionBegin();

	/*Enlists Current Thread in Transaction to Appropriately Update
	  Instruction Pointers for That Thread*/
	DetourUpdateThread(GetCurrentThread());

	/*Deallocates the Detour for the Target Function*/
	DetourDetach(reinterpret_cast<PVOID*>(targetFunction),
		reinterpret_cast<PVOID*>(detourFunction));

	/*Restores overwritten instructions of Target Function
	  and restores Target Function Pointer to point to original
	  function*/
	DetourTransactionCommit();
}

void RemoveDetour(QWORD bytes)
{
	DetourRemove((bytes & ((QWORD)UINT_MAX << 32)) >> 32,
		(bytes & UINT_MAX));
}

void InsertDetour(LPVOID targetFunction, LPVOID detourFunction)
{
	GameData::detours.push_back(((QWORD)targetFunction << 32) | (QWORD)detourFunction);
	DetourFunction((DWORD)targetFunction, (DWORD)detourFunction);
}

void Menu_PaintAllDetour(int localClientNum, UiContext *dc)
{
	Menu_PaintAll(localClientNum, dc);
	
	static bool built;
	if (!built)
		Menu::Build(), built = true;

	if (IN_IsForegroundWindow())
		Menu::MonitorKeys();

	if (Menu::open)
	{
		char nopSled[0x17];
		memset(nopSled, 0x90, 0x17);
		WriteBytes(0x680210, nopSled, 0x17);
		
		Menu::Execute();

		if (!(*(char*)0xE6DA38 & 0x10) && *(int*)0x3779330 != 7)
			UI_DrawHandlePic(
				scrPlace,
				GameData::dc->cursorPos[0] 
				- ((32.0f * scrPlace->scaleVirtualToReal[0])
					/ scrPlace->scaleVirtualToFull[0]) * 0.5f,
				GameData::dc->cursorPos[1]
				- ((32.0f * scrPlace->scaleVirtualToReal[1])
					/ scrPlace->scaleVirtualToFull[1]) * 0.5f,
				32.0f * scrPlace->scaleVirtualToReal[0] 
					/ scrPlace->scaleVirtualToFull[0],
				32.0f * scrPlace->scaleVirtualToReal[1]
					/ scrPlace->scaleVirtualToFull[1],
				4, 4, Colors::white, (Material*)0x26536E8);
	}
	else
		WriteBytes(0x680210, (const char*)prevOps, 0x17);

	WriteBytes(0x5DADFC, 1 ? "\xEB" : "\x74", 1);
}

void CL_CreateNewCommandsDetour(int localClientNum)
{
	CL_CreateNewCommands(localClientNum);
}

void CL_WritePacketDetour(int localClientNum)
{
	usercmd_s *ccmd = &clientActive->cmds[clientActive->cmdNumber & 0x7F],
		*ocmd = &clientActive->cmds[clientActive->cmdNumber - 1 & 0x7F];

	Variables::enableAimbot = true;
	Variables::autoShoot = true;
	Variables::aimType = 1;
	Variables::noRecoil = true;
	Variables::noSpread = true;
	Variables::enemyESP = true;
	Variables::friendlyESP = true;
	Variables::scavengerESP = true;
	Variables::missileESP = true;
	Variables::pickupESP = true;

	ocmd->serverTime += 2;

	if (Variables::enableAimbot)
	{
		if (Aimbot::Execute())
		{
			float oldAngle = ShortToAngle(ocmd->angles[1]);

			switch (Variables::aimType)
			{
			case 0:
				clientActive->viewangles[0] = Aimbot::targetAngles.pitch;
				clientActive->viewangles[1] = Aimbot::targetAngles.yaw;
				clientActive->viewangles[2] = Aimbot::targetAngles.roll;
				break;
			case 1:
				ocmd->angles[0] = AngleToShort(Aimbot::targetAngles.pitch);
				ocmd->angles[1] = AngleToShort(Aimbot::targetAngles.yaw);
				break;
			case 2:
				ccmd->angles[0] = AngleToShort(Aimbot::targetAngles.pitch);
				ccmd->angles[1] = AngleToShort(Aimbot::targetAngles.yaw);
				break;
			}

			if (Variables::noSpread)
				Aimbot::RemoveSpread(&cgameGlob->predictedPlayerState, ocmd);

			Aimbot::FixMovement(ocmd, ShortToAngle(ocmd->angles[1]), oldAngle,
				(float)ocmd->forwardmove, (float)ocmd->rightmove);

			if (Variables::autoAim)
				clientActive->usingAds = true;

			if (Variables::autoShoot)
			{
				ocmd->button_bits[0] &= ~0x80000000;
				ccmd->button_bits[0] |=  0x80000000;
			}
		}
		else
			clientActive->usingAds = false;
	}

	CL_WritePacket(localClientNum);
}

void CL_DrawStretchPicDetour(ScreenPlacement *scrPlace, float x,
	float y, float w, float h, int horzAlign, int vertAlign,
	float s1, float t1, float s2, float t2,
	const float *color, Material *material)
{
	CL_DrawStretchPic(scrPlace, x, y, w, h, horzAlign, vertAlign,
		s1, t1, s2, t2, color, material);
}

void CG_DrawNightVisionOverlayDetour(int localClientNum)
{
	(*(dvar_s**)0xD5864C)->current.value = 0.0f;
	(*(dvar_s**)0xC9D75C)->current.value = 0.7f;

	rectDef_s parentRect = { 0, 0, 0x44200000, 0x43F00000, 0, 0 };
	rectDef_s rect =
	{
		GameData::dc->screenDimensions[0] / 2 
			/ scrPlace->scaleVirtualToFull[0] - 375,
		GameData::dc->screenDimensions[1] / 2 
			/ scrPlace->scaleVirtualToFull[1] - 235,
		150, 150, 0, 0
	};
	RenderESP(1, &parentRect, &rect);

	CG_DrawNightVisionOverlay(localClientNum);
}

void Menu_HandleKeyDetour(int localClientNum, UiContext *dc,
	struct menuDef_t *menu, int key, unsigned int down)
{
	if (!Menu::open)
		Menu_HandleKey(localClientNum, dc, menu, key, down);
}

void Menu_HandleMouseMoveDetour(int localClientNum, UiContext *dc,
	struct menuDef_t *menu)
{
	if (!Menu::open)
		Menu_HandleMouseMove(localClientNum, dc, menu);
}

void Menus_ShowByNameDetour(UiContext *dc, const char *windowName)
{
	if (strcmp(windowName, "Compass"))
		return Menus_ShowByName(dc, windowName);
}

DWORD CL_MouseMoveRet = CL_MouseMoveRet_a;
void __declspec(naked) CL_MouseMoveStub(int localClientNum, usercmd_s *cmd)
{
	__asm
	{
		push		edi
		push		localClientNum
		call		CL_MouseMoveDetour
		pop			edi
		pop			edi
		test		al, al
		jnz			CONTINUE_FLOW
		pop			ecx
		jmp			ecx
CONTINUE_FLOW:
		sub			esp, 58h
		mov			ecx, 00E703D0h
		movss		xmm0, [ecx]
		jmp			CL_MouseMoveRet
	}
}

bool CL_MouseMoveDetour(int localClientNum, usercmd_s *cmd)
{
	return !Menu::open;
}

void Menu_PaintDetour(int localClientNum, UiContext *dc,
	struct ScreenPlacementStack *scrPlaceViewStack, struct menuDef_t *menu,
	int UI3OverrideID)
{
	if (strcmp(*(const char **)menu, "compass_old"))
		Menu_Paint(localClientNum, dc, scrPlaceViewStack, menu, UI3OverrideID);
}

float CG_GetScramblerEnemyAlphaDetour(int localClientNum)
{
	return 1.0f;
}