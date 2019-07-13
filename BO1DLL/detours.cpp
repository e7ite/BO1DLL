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
		Menu::Execute();

	RenderESP();

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

	ocmd->serverTime += 2;

	if (Variables::enableAimbot && Aimbot::Execute())
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
			ccmd->button_bits[0] |= 0x80000000;
		}
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