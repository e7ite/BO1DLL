#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <detours.h>
#include <sstream>

struct vec3_t
{
	union
	{
		struct { float x, y, z; };
		struct { float pitch, yaw, roll; };
	};

	vec3_t(float x = 0.0f, float y = 0.0f, float z = 0.0f)
		: x(x), y(y), z(z) {}
	vec3_t(float *vec) { memcpy(this, vec, 0xC); }
	vec3_t(const vec3_t &vec) { memcpy(this, &vec, 0xC); }

	operator float*() { return (float*)this; }

	vec3_t operator+(const vec3_t &vec) const;
	vec3_t operator+(float vec[3]) const;
	vec3_t operator-(const vec3_t &vec) const;
	vec3_t operator-(float vec[3]) const;
	vec3_t& operator+=(const vec3_t &vec);
	vec3_t& operator+=(float vec[3]);
	vec3_t& operator-=(const vec3_t &vec);
	vec3_t& operator-=(float vec[3]);
};

struct UiContext
{
	int contentIndex;						//0x00
	float bias;								//0x04
	int realtime;							//0x08
	int frameTime;							//0x0C
	float cursorPos[2];						//0x10
	float prevCursorPos[2];					//0x18
	int isCursorVisible;					//0x20
	int screenDimensions[2];				//0x24
	float screenAspect;						//0x2C
	float fps;								//0x30
}; //Size = 0x34

union DvarValue
{
	bool enabled;							//0x01
	int integer;							//0x04
	unsigned int unsignedInt;				//0x04
	__int64 integer64;						//0x08
	unsigned __int64 unsignedInt64;			//0x08
	float value;							//0x04
	float vector[4];						//0x10
	const char *string;						//0x04
	char color[4];							//0x04
}; //Size = 0x10

struct dvar_s
{
	const char *name;						//0x00
	const char *description;				//0x04
	int hash;								//0x08
	unsigned int flags;						//0x0C
	int type;								//0x10
	int modified;							//0x14
	DvarValue current;						//0x18
	DvarValue latched;						//0x28
	DvarValue reset;						//0x28
	DvarValue saved;						//0x48
	char pad00[0x10];						//0x58
	dvar_s *next;							//0x68
}; //Size = 0x6C

struct Font_s
{
	const char *fontName;					//0x00
	int pixelHeight;						//0x04
	int glyphCount;							//0x08
	struct Material *material;				//0x0C
	struct Material *glowMaterial;			//0x10
	struct Glyph *glyphs;					//0x14
}; //Size = 0x18

struct ScreenPlacement
{
	float scaleVirtualToReal[2];			//0x00
	float scaleVirtualToFull[2];			//0x08
	float scaleRealToVirtual[2];			//0x10
	float virtualViewableMin[2];			//0x18
	float virtualViewableMax[2];			//0x20
	float virtualTweakableMin[2];			//0x28
	float virtualTweakableMax[2];			//0x30
	float realViewportBase[2];				//0x38
	float realViewportSize[2];				//0x40
	float realViewportMid[2];				//0x48
	float realViewableMin[2];				//0x50
	float realViewableMax[2];				//0x58
	float realTweakableMin[2];				//0x60
	float realTweakableMax[2];				//0x68
	float subScreen[2];						//0x70
}; //Size = 0x78

struct playerState_s
{
	int commandTime;						//0x0000
	int pm_type;							//0x0004
	int bobCycle;							//0x0008
	int pm_flags;							//0x000C
	char pad00[0x18];						//0x0010
	float origin[3];						//0x0028
	float velocity[3];						//0x0034
	char pad01[0x44];						//0x0040
	float delta_angles[3];					//0x0084
	char pad02[0xA0];						//0x0090
	char clientNum;							//0x0130
	char pad03[0x13];						//0x0131
	unsigned short weapon;					//0x0144
	unsigned short lastStandPrevWeapon;		//0x0146
	char pad04[0x38];						//0x0148
	float viewangles[3];					//0x0180
	char pad05[0x255C];						//0x018C
}; //Size = 0x26E8

struct cpose_t
{
	unsigned short lightningHandle;			//0x00
	char eType;								//0x02
	char eTypeUnion;						//0x03
	char pad00[0x2C];						//0x04
	float origin[3];						//0x30
	float angles[3];						//0x3C
	float absmin[3];						//0x48
	float absmax[3];						//0x54
}; //Size = 0x60

struct LerpEntityState
{
	int eFlags;								//0x000
	int eFlags2;							//0x004
}; 

struct entityState_s
{
	int number;								//0x00
	LerpEntityState lerp;					//0x04
	char pad00[0xB2];						//0x0C
	char eType;								//0xBE
	char pad01[0x16];						//0xBF
	char clientNum;							//0xD5
	char pad02[0xA];						//0xD6
}; //Size = 0xE0

struct centity_s
{
	cpose_t pose;							//0x000
	LerpEntityState lerp;					//0x060
	char pad00[0x180];						//0x068
	entityState_s nextState;				//0x1E8
	char pad01[0x60];						//0x2C8
}; //Size = 0x328

struct refdef_s
{
	unsigned int x;							//0x0000
	unsigned int y;							//0x0004
	unsigned int width;						//0x0008
	unsigned int height;					//0x000C
	float tanHalfFovX;						//0x0010
	float tanHalfFovY;						//0x0014
	float fov_x;							//0x0018
	char pad00[0x4];						//0x001C
	float vieworg[3];						//0x0020
	float yaw;								//0x002C
	char pad01[0xC];						//0x0030
	float viewaxis[3][3];					//0x003C
	int time;								//0x0060
}; //Size = 0x64

struct clientInfo_t
{

}; //Size = 0x5D0

struct snapshot_s
{

};

struct cg_s
{
	int clientNum;							//0x000000
	int localClientNum;						//0x000004
	char pad00[0x20];						//0x000008
	snapshot_s *snap;						//0x000028
	snapshot_s *nextSnap;					//0x00002C
	char pad01[0x40658];					//0x000030
	int time;								//0x040688
	int oldTime;							//0x04068C
	int physicsTime;						//0x040690
	int mapRestart;							//0x040694
	int renderingThirdPerson;				//0x040698
	playerState_s predictedPlayerState;		//0x04069C
	char pad02[0x37C];						//0x042D84
	refdef_s refdef;						//0x043100
	char pad03[0x1BCEC];					//0x043164
	int weaponSelect;						//0x05EE50
	char pad04[0x284];						//0x05EE54
	int	inKillCam;							//0x05F0D8
	char pad05[0x15C];						//0x05F0DC
	clientInfo_t clients[0x12];				//0x05F238
	char pad06[0xA038];						//0x065AD8
	float aimSpreadScale;					//0x06FB10
}; 

struct clSnapshot_t
{
	int valid;								//0x000000		
	int snapFlags;							//0x000004
	int serverTime;							//0x000008
	int physicsTime;						//0x00000C
	int messageNum;							//0x000010
	int deltaNum;							//0x000014
	int ping;								//0x000018
	int cmdNum;								//0x00001C
	playerState_s ps;						//0x000020
	int numEntities;						//0x002708
	int numClients;							//0x00270C
	int parseMatchStateNum;					//0x002710
	int parseEntitiesNum;					//0x002714
	int parseClientsNum;					//0x002718
	int serverCommandNum;					//0x00271C
}; //Size = 0x2720

struct usercmd_s
{
	int serverTime;							//0x00
	int button_bits[2];						//0x04
	int angles[3];							//0x0C
	unsigned short weapon;					//0x18
	unsigned short offHandIndex;			//0x20
	unsigned short lastWeaponAltModeSwitch; //0x22
	char forwardmove;						//0x23
	char rightmove;							//0x24
	char upmove;							//0x25
	char pitchmove;							//0x26
	char yawmove;							//0x27
	float meleeChargeYaw;					//0x28
	char meleeChargeDist;					//0x2C
	float rollmove;							//0x2D
	char selectedLocation[2];				//0x31
	char selectedYaw;						//0x33
}; //Size = 0x34

struct clientActive_t
{
	bool usingAds;							//0x000000
	char pad00[0x3];						//0x000001
	int timeoutcount;						//0x000004
	clSnapshot_t snap;						//0x000008
	char pad01[0x124];						//0x002728
	float viewangles[3];					//0x00284C
	char pad02[0xEADB90];					//0x002858
	usercmd_s cmds[0x80];					//0xEB03E8
	int cmdNumber;							//0xEB1DE8
};

enum FuncAddresses : DWORD
{
	UI_DrawText_a							= 0x4D65A0,
	CL_RegisterFont_a						= 0x57D940,
	UI_DrawRect_a							= 0x4ED000,
	UI_FillRect_a							= 0x529C40,
	CG_GetEntity_a							= 0x5A5CC0,
	WorldPosToScreenPos_a					= 0x49BA00,
	Material_RegisterHandle_a				= 0x6DBEF0,
	CG_DrawRotatedPicPhysical_a				= 0x449300,
	Dvar_FindVar_a							= 0x4D01A0,
	SL_FindString_a							= 0x4E3FC0,
	Com_GetClientDObj_a						= 0x6ACD60,
	CG_DObjGetLocalTagMatrix_a				= 0x5D63B0,
	vectoangles_a							= 0x4D5230,
};

using QWORD = unsigned long long;

namespace GameData
{
	extern std::vector<QWORD> detours;
	extern UiContext *dc;
	extern ScreenPlacement *scrPlace;
	extern cg_s *cgameGlob;
	extern clientActive_t *clientActive;
}

using namespace GameData;

namespace Colors
{
	struct Color
	{
		float r, g, b, a;

		Color() : r(0.0F), g(0.0f), b(0.0f), a(1.0f) {}
		Color(float r, float g, float b, float a)
			: r(r / 255), g(g / 255), b(b / 255), a(a / 255) {}

		operator float*() { return reinterpret_cast<float*>(this); }
	};

	extern Color white;
	extern Color black;
	extern Color red;
	extern Color green;
	extern Color blue;
	extern Color transparentBlack;
}

extern void(__cdecl *UI_DrawText)(ScreenPlacement *scrPlace,
	const char *text, int maxChars, Font_s *font, float x, float y,
	int horzAlign, int vertAlign, float scale, const float *color, int style);
extern bool(__cdecl *WorldPosToScreenPos)(int localClientNum, const float *worldPos,
	float *outScreenPos);
extern centity_s*(__cdecl *CG_GetEntity)(int localClientNum, int entityIndex);
extern Font_s*(__cdecl* CL_RegisterFont)(const char *name, int imageTrack);
extern void(__cdecl *UI_FillRect)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, int horzAlign, int vertAlign, const float *color);
extern void(__cdecl *UI_DrawRect)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, int horzAlign, int vertAlign,
	float size, const float *color);
extern struct Material*(__cdecl *Material_RegisterHandle)(const char *name, 
	int imageTrack);
extern void(__cdecl *CG_DrawRotatedPicPhysical)(ScreenPlacement *scrPlace,
	float x, float y, float width, float height, float angle, const float *color,
	struct Material *material);
extern dvar_s*(__cdecl *Dvar_FindVar)(const char *name);
extern unsigned short(__cdecl *SL_FindString)(const char *str, int localClientNum);
extern int(__cdecl *CG_DObjGetLocalTagMatrix)(cpose_t *pose, struct DObj *obj,
	int tagname, float *out);
extern void(__cdecl *vectoangles)(const float *vec, float *angles);

bool AimTarget_GetTagPos(centity_s *cent, const char *tagname, float *pos);