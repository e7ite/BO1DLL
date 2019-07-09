#include "detours.h"

void(__cdecl *Menu_PaintAll)(int localClientNum, UiContext *dc)
= (void(__cdecl*)(int, UiContext*))Menu_PaintAll_a;
void(__cdecl *CL_CreateNewCommands)(int localClientNum)
= (void(__cdecl*)(int))CL_CreateNewCommands_a;
void(__cdecl *CL_WritePacket)(int localClientNum)
= (void(__cdecl*)(int))CL_WritePacket_a;

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

	RenderUITextWithBackground(
		BG_HasPerk(cgameGlob->clientNum, "specialty_scavenger") ? "true" : "false",
		400, 400, 0.4f, ALIGN_RIGHT, Colors::green, Colors::transparentBlack,
		normalFont);

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

	if (Aimbot::gotTarget = ExecuteAimbot())
	{
		float oldAngle = ShortToAngle(ocmd->angles[1]);

		//ocmd->angles[0] = AngleToShort(Aimbot::targetAngles.x);
		//ocmd->angles[1] = AngleToShort(Aimbot::targetAngles.y);

		SetAngles(Aimbot::targetAngles);
		RemoveSpread(&cgameGlob->predictedPlayerState, ocmd);

		ocmd->serverTime += 2;

		//ocmd->button_bits[0] |= 0x80000000;
		//ccmd->button_bits[0] &= ~0x80000000;

		FixMovement(ocmd, ShortToAngle(ocmd->angles[1]), oldAngle,
			(float)ocmd->forwardmove, (float)ocmd->rightmove);
	}

	CL_WritePacket(localClientNum);
}