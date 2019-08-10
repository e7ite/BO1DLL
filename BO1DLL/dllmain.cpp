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

		InsertDetour(&Menu_PaintAll,			 Menu_PaintAllDetour);
		InsertDetour(&CL_CreateNewCommands,		 CL_CreateNewCommandsDetour);
		InsertDetour(&CL_WritePacket,			 CL_WritePacketDetour);
		InsertDetour(&CL_DrawStretchPic,		 CL_DrawStretchPicDetour);
		InsertDetour(&Menu_HandleKey,			 Menu_HandleKeyDetour);
		InsertDetour(&Menu_HandleMouseMove,		 Menu_HandleMouseMoveDetour);
		InsertDetour(&Menus_ShowByName,			 Menus_ShowByNameDetour);
		InsertDetour(&CL_MouseMove,				 CL_MouseMoveStub);
		InsertDetour(&CG_DrawNightVisionOverlay, CG_DrawNightVisionOverlayDetour);
		InsertDetour(&Menu_Paint,				 Menu_PaintDetour);
		InsertDetour(&CG_GetScramblerEnemyAlpha, CG_GetScramblerEnemyAlphaDetour);
		break;
    case DLL_PROCESS_DETACH:
		FreeConsole();

		for (const auto &i : GameData::detours)
			RemoveDetour(i);
		break;
    }

    return TRUE;
}