#include "structures.h"

std::vector<QWORD> GameData::detours;
UiContext *GameData::dc				   = (UiContext*)0x3777160;
ScreenPlacement *GameData::scrPlace	   = (ScreenPlacement*)0x23C04F0;
cg_s *GameData::cgameGlob			   = *(cg_s**)0xD55078;
clientActive_t *GameData::clientActive = (clientActive_t*)0xE6DB80;
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

bool AimTarget_GetTagPos(centity_s *cent, const char *tagname, float *pos)
{
	struct DObj *obj = Com_GetClientDObj(cent->nextState.number, 0);
	if (!obj)
		return false;
	
	if (!CG_DObjGetLocalTagMatrix(&cent->pose, obj, SL_FindString(tagname, 0),
		pos))
		return false;

	return true;
}