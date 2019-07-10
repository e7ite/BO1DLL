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

	/*std::stringstream str;
	str << std::hex << BG_GetWeaponDef(cgameGlob->weaponSelect);

	RenderUITextWithBackground(std::to_string(
			BG_GetWeaponDef(cgameGlob->weaponSelect)->fMinDamageRange
	).data(),
		400, 200, 0.6f, ALIGN_RIGHT, Colors::red, Colors::red, normalFont);
	RenderUITextWithBackground(str.str().data(),
		400, 400, 0.6f, ALIGN_RIGHT, Colors::red, Colors::red, normalFont);
*/
	RenderESP();
	WriteBytes(0x5DADFC, 1 ? "\xEB" : "\x74", 1);
}

void CL_CreateNewCommandsDetour(int localClientNum)
{
	//usercmd_s *ccmd = &clientActive->cmds[clientActive->cmdNumber & 0x7F],
	//	*ocmd = &clientActive->cmds[clientActive->cmdNumber - 1 & 0x7F];

	//if (Aimbot::gotTarget)
	//{
	//	ocmd->button_bits[0] |= 0x80000000;
	//	ccmd->button_bits[0] &= ~0x80000000;
	//}
	CL_CreateNewCommands(localClientNum);

}

void CL_WritePacketDetour(int localClientNum)
{
	usercmd_s *ccmd = &clientActive->cmds[clientActive->cmdNumber & 0x7F],
		*ocmd = &clientActive->cmds[clientActive->cmdNumber - 1 & 0x7F];

	ocmd->serverTime += 2;

	if (ExecuteAimbot())
	{
		float oldAngle = ShortToAngle(ocmd->angles[1]);

		ocmd->angles[0] = AngleToShort(Aimbot::targetAngles.x);
		ocmd->angles[1] = AngleToShort(Aimbot::targetAngles.y);

		//SetAngles(Aimbot::targetAngles);
		RemoveSpread(&cgameGlob->predictedPlayerState, ocmd);

		FixMovement(ocmd, ShortToAngle(ocmd->angles[1]), oldAngle,
			(float)ocmd->forwardmove, (float)ocmd->rightmove);

		ocmd->button_bits[0] &= ~0x80000000;
		ccmd->button_bits[0] |=  0x80000000;

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