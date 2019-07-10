#include "structures.h"
#include "detours.h"

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
	LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
		AllocConsole();
		SetConsoleTitle("BlackOpsDLL");
		freopen("CONOUT$", "w", stdout);

		InsertDetour(&Menu_PaintAll, Menu_PaintAllDetour);
		InsertDetour(&CL_CreateNewCommands, CL_CreateNewCommandsDetour);
		InsertDetour(&CL_WritePacket, CL_WritePacketDetour);
		InsertDetour(&CL_DrawStretchPic, CL_DrawStretchPicDetour);
		break;
    case DLL_PROCESS_DETACH:
		FreeConsole();

		for (const auto &i : GameData::detours)
			RemoveDetour(i);
		break;
    }

    return TRUE;
}