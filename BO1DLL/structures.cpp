#include "structures.h"

std::vector<QWORD> GameData::detours;
UiContext *GameData::dc				   =		(UiContext*)0x3777160;
ScreenPlacement *GameData::scrPlace	   =  (ScreenPlacement*)0x23C04F0;
cg_s *GameData::cgameGlob			   =			*(cg_s**)0xD55078;
clientActive_t *GameData::clientActive =	(clientActive_t*)0xE6DB80;
cgs_t *GameData::cgs				   =		   *(cgs_t**)0xD55044;
KeyState *GameData::keys			   =		  (KeyState*)0xDB497C;
HWND *GameData::hWnd				   =			 (HWND*)0x39A9904;
Fonts GameData::normalFont			   = { 1,    "fonts/normalFont" };
Fonts GameData::smallFont 			   = { 3,     "fonts/smallFont" };
Fonts GameData::consoleFont  		   = { 5,   "fonts/consoleFont" };
Fonts GameData::objectiveFont   	   = { 6, "fonts/objectiveFont" };

const unsigned char GameData::prevOps[0x17] =
{
	0xF6, 0x05, 0x38, 0xDA, 0xE6, 0x00, 0x10, // test byte ptr clUI[1], 10h
	0x74, 0x25,								  // jz short 68023Eh	 
	0x6A, 0x00,								  // push 0
	0xE8, 0x30, 0x33, 0xE7, 0xFF,			  // call UI_KeysBypassMenu
	0x83, 0xC4, 0x04,						  // add esp, 4
	0x84, 0xC0,								  // test al, al
	0x75, 0x17								  // jnz short 68023Eh
};

Colors::Color Colors::white			   = { 255, 255, 255, 255 };
Colors::Color Colors::black			   = {   0,   0,   0, 255 };
Colors::Color Colors::red			   = { 255,   0,   0, 255 };
Colors::Color Colors::green            = {   0, 255,   0, 255 };
Colors::Color Colors::blue			   = {   0,   0, 255, 255 };
Colors::Color Colors::transparentBlack = {   0,   0,   0, 100 };

void(__cdecl *UI_DrawText)(ScreenPlacement *scrPlace,
	const char *text, int maxChars, struct Font_s *font, float x, float y,
	int horzAlign, int vertAlign, float scale, const float *color, int style)
= (void(__cdecl*)(ScreenPlacement*, const char*, int, Font_s*, float, float,
	int, int, float, const float*, int))UI_DrawText_a;
bool(__cdecl *WorldPosToScreenPos)(int localClientNum, const float *worldPos,
	float *outScreenPos)
= (bool(__cdecl*)(int, const float*, float*))WorldPosToScreenPos_a;
centity_s*(__cdecl *CG_GetEntity)(int localClientNum, int entityIndex)
= (centity_s*(__cdecl*)(int, int))CG_GetEntity_a;
Font_s*(__cdecl* CL_RegisterFont)(const char *name, int imageTrack)
= (Font_s*(__cdecl*)(const char*, int))CL_RegisterFont_a;
void(__cdecl *UI_FillRect)(ScreenPlacement *scrPlace, float x, float y,
float width, float height, int horzAlign, int vertAlign, const float *color)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, int, int,
	const float*))UI_FillRect_a;
void(__cdecl *UI_DrawRect)(ScreenPlacement *scrPlace, float x, float y,
	float width, float height, int horzAlign, int vertAlign,
	float size, const float *color)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, int, int,
	float, const float*))UI_DrawRect_a;
void(__cdecl *CG_DrawRotatedPicPhysical)(ScreenPlacement *scrPlace,
	float x, float y, float width, float height, float angle, const float *color,
	struct Material *material)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, float, const float*,
	Material*))CG_DrawRotatedPicPhysical_a;
Material*(__cdecl *Material_RegisterHandle)(const char *name, int imageTrack)
= (Material*(__cdecl*)(const char *name, int imageTrack))Material_RegisterHandle_a;
dvar_s*(__cdecl *Dvar_FindVar)(const char *name) 
= (dvar_s*(__cdecl*)(const char*))Dvar_FindVar_a;
DObj*(__cdecl *Com_GetClientDObj)(int handle, int localClientNum)
= (DObj*(__cdecl*)(int, int))Com_GetClientDObj_a;
unsigned short(__cdecl *SL_FindString)(const char *str, int localClientNum)
= (unsigned short(__cdecl*)(const char*, int))SL_FindString_a;
int(__cdecl *CG_DObjGetLocalTagMatrix)(cpose_t *pose, DObj *obj,
	int tagname, float *out)
= (int(__cdecl*)(cpose_t*, DObj*, int, float*))CG_DObjGetLocalTagMatrix_a;
void(__cdecl *vectoangles)(const float *vec, float *angles)
= (void(__cdecl*)(const float*, float*))vectoangles_a;
void(__cdecl *R_AddCmdDrawText)(const char *text, int maxChars,
	Font_s *font, float x, float y, float xScale, float yScale, float rotation,
	const float *color, int style)
= (void(__cdecl*)(const char*, int, Font_s*, float, float, float, float, float,
	const float*, int))R_AddCmdDrawText_a;	
int(__cdecl *R_TextWidth)(const char *text, int maxChars, Font_s *font)
= (int(__cdecl*)(const char*, int, Font_s*))R_TextWidth_a;
int(__cdecl *R_TextHeight)(Font_s *font) = (int(__cdecl*)(Font_s*))R_TextHeight_a;
Font_s*(__cdecl *UI_GetFontHandle)(ScreenPlacement *scrPlace, int fontEnum,
	float scale)
= (Font_s*(__cdecl*)(ScreenPlacement*, int, float))UI_GetFontHandle_a;
int(__cdecl *UI_TextWidth)(const char *text, int maxChars, Font_s *font,
	float scale)
= (int(__cdecl*)(const char*, int, Font_s*, float))UI_TextWidth_a;
int(__cdecl *UI_TextHeight)(Font_s *font, float scale)
= (int(__cdecl*)(Font_s*, float))UI_TextHeight_a;
void(__cdecl *BG_GetSpreadForWeapon)(playerState_s *ps, WeaponDef *weap,
	float *minSpread, float *maxSpread)
= (void(__cdecl*)(playerState_s*, WeaponDef*, float*, float*))BG_GetSpreadForWeapon_a;
WeaponDef*(__cdecl *BG_GetWeaponDef)(int weaponIndex)
= (WeaponDef*(__cdecl*)(int))BG_GetWeaponDef_a;
WeaponVariantDef*(__cdecl *BG_GetWeaponVariantDef)(int weaponIndex)
= (WeaponVariantDef*(__cdecl*)(int))BG_GetWeaponVariantDef_a;
bool(__cdecl *CG_GetPlayerViewOrigin)(int localClientNum,
	playerState_s *ps, float *origin)
= (bool(__cdecl*)(int, playerState_s*, float*))CG_GetPlayerViewOrigin_a;
void(__cdecl *AngleVectors)(const float *angles, float *forward,
	float *right, float *up)
= (void(__cdecl*)(const float*, float*, float*, float*))AngleVectors_a;
char(__cdecl *ClampChar)(int c) = (char(__cdecl*)(int c))ClampChar_a;
void(__cdecl *RandomBulletDir)(int randSeed, float *x, float *y)
= (void(__cdecl*)(int, float*, float*))RandomBulletDir_a;
unsigned int(__cdecl *BG_GetPerkIndexForName)(const char *perkName)
= (unsigned int(__cdecl*)(const char*))BG_GetPerkIndexForName_a;
int(*Sys_Milliseconds)() = (int(*)())Sys_Milliseconds_a;
void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace,
	float x, float y, float width, float height, int horzAlign, int vertAlign,
	float angle, const float *color, struct Material *material)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, int, int,
		float, const float*, Material*))CG_DrawRotatedPic_a;
void(__cdecl *CL_DrawTextWithEffects)(ScreenPlacement *scrPlace,
	const char *text, int maxChars, Font_s *font, float x, float y,
	float rotation, int horzAlign, int vertAlign, float xScale, float yScale,
	const float *color, int style, const float *glowColor,
	struct Material *fxMaterial, Material *fxMaterialGlow, int fxBirthTime,
	int fxLetterTime, int fxDecayStartTime, int fxDecayDuration)
= (void(__cdecl*)(ScreenPlacement *,const char*, int, Font_s*, float, float,
	float, int, int, float, float, const float*, int, const float*,
	Material*, Material*, int, int, int, int))CL_DrawTextWithEffects_a;
void(__cdecl *UI_DrawTextWithGlow)(ScreenPlacement *scrPlace,
	const char *text, int maxChars, Font_s *font, float x, float y,
	int horzAlign, int vertAlign, float scale, const float *color,
	int style, const float *glowColor, bool subtitle, bool cinematic)
= (void(__cdecl*)(ScreenPlacement*, const char*, int, Font_s*, float,
	float, int, int, float, const float*, int, const float*,
	bool, bool))UI_DrawTextWithGlow_a;
void(__cdecl *ScrPlace_ApplyRect)(ScreenPlacement *scrPlace,
	float *x, float *y, float *w, float *h, int horzAlign, int vertAlign)
= (void(__cdecl*)(ScreenPlacement*, float*, float*, float*, float*, int, int))
	ScrPlace_ApplyRect_a;
float(*ScrPlace_HiResGetScale)() = (float(*)())ScrPlace_HiResGetScale_a;
void(__cdecl *Menus_HideByName)(UiContext *dc, const char *menuName)
= (void(__cdecl*)(UiContext*, const char*))Menus_HideByName_a;
struct menuDef_t*(__cdecl *Menus_FindByName)(UiContext *dc, int hashName)
= (menuDef_t*(__cdecl*)(UiContext*, int))Menus_FindByName_a;
int(__cdecl *Com_HashString)(const char *fname, int len)
= (int(__cdecl*)(const char*, int))Com_HashString_a;
void(__cdecl *UI_DrawHandlePic)(ScreenPlacement *scrPlace, float x, float y,
	float w, float h, int horzAlign, int vertAlign, const float *color,
	Material *material)
= (void(__cdecl*)(ScreenPlacement*, float, float, float, float, int, int,
	const float*, Material*))UI_DrawHandlePic_a;
void(__cdecl *CG_CompassDrawPlayerMap)(int localClientNum, int compassType,
	rectDef_s *parentRect, rectDef_s *rect, struct Material *material,
	const float *color, bool grid)
= (void(__cdecl*)(int, int, rectDef_s*, rectDef_s*, Material*, const float*,
	bool))CG_CompassDrawPlayerMap_a;
bool(__cdecl *CG_WorldPosToCompass)(int compassType, cg_s *cgameGlob,
	rectDef_s *mapRect, const float *north, const float *playerWorldPos,
	const float *in, float *out, float *outClipped)
= (bool(__cdecl*)(int, cg_s*, rectDef_s*, const float*, const float*,
	const float*, float*, float*))CG_WorldPosToCompass_a;
void(__cdecl *CG_CompassUpYawVector)(cg_s *cgameGlob, float *result)
= (void(__cdecl*)(cg_s*, float*))CG_CompassUpYawVector_a;
void(__cdecl *CalcCompassFriendlySize)(int compassType, float *w,
	float *h)
= (void(__cdecl*)(int, float*, float*))CalcCompassFriendlySize_a;	
void(__cdecl *CG_CompassCalcDimensions)(int compassType,
	cg_s *cgameGlob, rectDef_s *parentRect, rectDef_s *rect,
	float *x, float *y, float *w, float *h)
= (void(__cdecl*)(int, cg_s*, rectDef_s*, rectDef_s*,
	float*, float*, float*, float*))CG_CompassCalcDimensions_a;
float(__cdecl *AngleNormalize360)(const float angle)
= (float(__cdecl*)(const float))AngleNormalize360_a;
void(__cdecl *CG_CompassDrawVehicles)(int localClientNum, int compassType,
	int eType, const rectDef_s *parentRect, const rectDef_s *rect, const float *color)
= (void(__cdecl*)(int, int, int, const rectDef_s*,
	const rectDef_s*, const float*))CG_CompassDrawVehicles_a;
void(__cdecl *CG_CompassDrawHelicopter)(int localClientNum, int compassType,
	int eType, const rectDef_s *parentRect, const rectDef_s *rect, const float *color)
= (void(__cdecl*)(int, int, int, const rectDef_s*, const rectDef_s*,
	const float*))CG_CompassDrawHelicopter_a;
void(__cdecl *CG_CompassDrawPlayer)(int localClientNum, int compassType,
	const rectDef_s *parentRect, const rectDef_s *rect, Material *material,
	const float *color)
= (void(__cdecl*)(int, int, const rectDef_s*, const rectDef_s*, Material*,
	const float*))CG_CompassDrawPlayer_a;

vec3_t vec3_t::operator+(const vec3_t &vec) const
{
	return vec3_t(this->x + vec.x, this->y + vec.y, this->z + vec.z);
}

vec3_t vec3_t::operator+(float vec[3]) const
{
	return vec3_t(this->x + vec[0], this->y + vec[1], this->z + vec[2]);
}

vec3_t vec3_t::operator-(const vec3_t &vec) const
{
	return vec3_t(this->x - vec.x, this->y - vec.y, this->z - vec.z);
}

vec3_t vec3_t::operator-(float vec[3]) const
{
	return vec3_t(this->x - vec[0], this->y - vec[1], this->z - vec[2]);
}

vec3_t vec3_t::operator*(const vec3_t &vec) const
{
	return vec3_t(this->x * vec.x, this->y * vec.y, this->z * vec.z);
}

vec3_t vec3_t::operator*(float vec[3]) const
{
	return vec3_t(this->x * vec[0], this->y * vec[1], this->z * vec[2]);
}

vec3_t& vec3_t::operator+=(const vec3_t &vec)
{
	this->x += vec.x;
	this->y += vec.y;
	this->z += vec.z;

	return *this;
}

vec3_t& vec3_t::operator+=(float vec[3])
{
	this->x += vec[0];
	this->y += vec[1];
	this->z += vec[2];

	return *this;
}

vec3_t& vec3_t::operator-=(const vec3_t &vec)
{
	this->x -= vec.x;
	this->y -= vec.y;
	this->z -= vec.z;

	return *this;
}

vec3_t& vec3_t::operator-=(float vec[3])
{
	this->x -= vec[0];
	this->y -= vec[1];
	this->z -= vec[2];

	return *this;
}

vec3_t& vec3_t::operator*=(const vec3_t &vec)
{
	this->x *= vec.x;
	this->y *= vec.y;
	this->z *= vec.z;

	return *this;
}

vec3_t& vec3_t::operator*=(float vec[3])
{
	this->x *= vec[0];
	this->y *= vec[1];
	this->z *= vec[2];

	return *this;
}

bool AimTarget_GetTagPos(centity_s *cent, const char *tagname, float *pos)
{
	struct DObj *obj = Com_GetClientDObj(cent->nextState.number, 0);
	if (!obj)
		return false;
	
	if (!CG_DObjGetLocalTagMatrix(&cent->pose, obj, 
		SL_FindString(tagname, 0), pos))
		return false;

	return true;
}

bool AimTarget_IsTargetVisible(centity_s *targetEnt, const char *visbone)
{
	DWORD addr = AimTarget_IsTargetVisible_a;
	unsigned short bone = SL_FindString(visbone, 0);

	__asm
	{
		mov			ecx, targetEnt
		movzx		eax, bone
		push		0
		call		addr
		add			esp, 4
	}
}

bool AimTarget_IsTargetValid(centity_s *targetEnt, cg_s *cgameGlob)
{
	DWORD addr = AimTarget_IsTargetValid_a;
	__asm
	{
		mov			edi, targetEnt
		mov			esi, cgameGlob
		call		addr
	}
}

void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
	const float *color, struct Material *material, int ratio)
{
	DWORD addr = DrawSketchPicGun_a;
	__asm
	{
		mov			eax, rect
		push		ratio
		push		material
		push		color
		push		scrPlace
		call		addr
		add			esp, 10h
	}
}

void CG_BulletEndpos(int randSeed, const float spread, const float *start,
	float *end, float *dir, const float *forwardDir, const float *rightDir,
	const float *upDir, const float maxRange)
{
	DWORD addr = CG_BulletEndpos_a;
	__asm
	{
		movss		xmm0, spread
		mov			edi, start
		mov			esi, end
		push		maxRange
		push		upDir
		push		rightDir
		push		forwardDir
		push		dir
		push		randSeed
		call		addr
		add			esp, 1Ch
	}
}

bool BG_HasPerk(int index, const char *perkName)
{
	return (*(long long*)cgameGlob->clients[index].perks 
		& (1i64 << BG_GetPerkIndexForName(perkName)));
}

bool Key_IsDown(const char *bind)
{
	for (__int16 i = 0; i < 256; ++i)
		if (keys[i].binding)
			if (!strcmp(keys[i].binding, bind))
				if (keys[i].down)
					return true;

	return false;
}

bool IN_IsForegroundWindow()
{
	return GetForegroundWindow() == *hWnd;
}

bool WorldPosToCompass(const centity_s *cent, rectDef_s *mapRect, rectDef_s *itemRect)
{
	bool clipped;
	float yawVector[2];

	CG_CompassUpYawVector(cgameGlob, yawVector);
	clipped = CG_WorldPosToCompass(0, cgameGlob, mapRect, yawVector,
		cgameGlob->refdef.vieworg, cent->pose.origin, 0, &itemRect->x);

	float centerX = mapRect->w * 0.5 + mapRect->x;
	float centerY = mapRect->h * 0.5 + mapRect->y;

	itemRect->x += centerX - itemRect->w * 0.5;
	itemRect->y += centerY - itemRect->h * 0.5;

	return clipped;
}