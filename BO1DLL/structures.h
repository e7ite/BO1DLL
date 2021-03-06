#pragma once

#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <vector>
#include <functional>
#include <detours.h>
#include <sstream>
#pragma pack(push, 1)

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
    operator const float*() const { return (const float*)this; }

    vec3_t operator+(const vec3_t &vec) const;
    vec3_t operator+(float vec[3]) const;
    vec3_t operator-(const vec3_t &vec) const;
    vec3_t operator-(float vec[3]) const;
    vec3_t operator*(const vec3_t &vec) const;
    vec3_t operator*(float vec[3]) const;
    vec3_t& operator+=(const vec3_t &vec);
    vec3_t& operator+=(float vec[3]);
    vec3_t& operator-=(const vec3_t &vec);
    vec3_t& operator-=(float vec[3]);
    vec3_t& operator*=(const vec3_t &vec);
    vec3_t& operator*=(float vec[3]);
};

struct UiContext
{
    int contentIndex;                           //0x00
    float bias;                                 //0x04
    int realtime;                               //0x08
    int frameTime;                              //0x0C
    float cursorPos[2];                         //0x10
    float prevCursorPos[2];                     //0x18
    int isCursorVisible;                        //0x20
    int screenDimensions[2];                    //0x24
    float screenAspect;                         //0x2C
    float fps;                                  //0x30
}; //Size = 0x34

union DvarValue
{
    bool enabled;                               //0x01
    int integer;                                //0x04
    unsigned int unsignedInt;                   //0x04
    __int64 integer64;                          //0x08
    unsigned __int64 unsignedInt64;             //0x08
    float value;                                //0x04
    float vector[4];                            //0x10
    const char *string;                         //0x04
    char color[4];                              //0x04
}; //Size = 0x10

union DvarLimits
{
    struct
    {
        int stringCount;
        const char **strings;
    } enumeration;                              //0x08
    struct
    {
        int min;
        int max;
    } integer;                                  //0x08
    struct
    {
        __int64 min;
        __int64 max;
    } integer64;                                //0x10
    struct
    {
        float min;
        float max;
    } value, vector;                            //0x08
}; //Size = 0x10

struct dvar_s
{
    const char *name;                           //0x00
    const char *description;                    //0x04
    int hash;                                   //0x08
    unsigned int flags;                         //0x0C
    int type;                                   //0x10
    int modified;                               //0x14
    DvarValue current;                          //0x18
    DvarValue latched;                          //0x28
    DvarValue reset;                            //0x38
    DvarValue saved;                            //0x48
    DvarLimits domain;                          //0x58
    dvar_s *next;                               //0x68
}; //Size = 0x6C

struct Font_s
{
    const char *fontName;                       //0x00
    int pixelHeight;                            //0x04
    int glyphCount;                             //0x08
    struct Material *material;                  //0x0C
    struct Material *glowMaterial;              //0x10
    struct Glyph *glyphs;                       //0x14
}; //Size = 0x18

struct ScreenPlacement
{
    float scaleVirtualToReal[2];                //0x00
    float scaleVirtualToFull[2];                //0x08
    float scaleRealToVirtual[2];                //0x10
    float virtualViewableMin[2];                //0x18
    float virtualViewableMax[2];                //0x20
    float virtualTweakableMin[2];               //0x28
    float virtualTweakableMax[2];               //0x30
    float realViewportBase[2];                  //0x38
    float realViewportSize[2];                  //0x40
    float realViewportMid[2];                   //0x48
    float realViewableMin[2];                   //0x50
    float realViewableMax[2];                   //0x58
    float realTweakableMin[2];                  //0x60
    float realTweakableMax[2];                  //0x68
    float subScreen[2];                         //0x70
}; //Size = 0x78

struct playerState_s
{
    int commandTime;                            //0x0000
    int pm_type;                                //0x0004
    int bobCycle;                               //0x0008
    int pm_flags;                               //0x000C
    char pad00[0x18];                           //0x0010
    float origin[3];                            //0x0028
    float velocity[3];                          //0x0034
    char pad01[0x44];                           //0x0040
    float delta_angles[3];                      //0x0084
    char pad02[0xA0];                           //0x0090
    char clientNum;                             //0x0130
    char pad03[0x13];                           //0x0131
    unsigned short weapon;                      //0x0144
    unsigned short lastStandPrevWeapon;         //0x0146
    char pad04[0x20];                           //0x0148
    float fWeaponPosFrac;                       //0x0168
    int adsDelayTime;                           //0x016C
    int spreadOverride;                         //0x0170
    int spreadOverrideState;                    //0x0174
    float weaponSpinLerp;                       //0x0178
    int viewmodelIndex;                         //0x017C
    float viewangles[3];                        //0x0180
    char pad05[0x3D4];                          //0x018C
    float aimSpreadScale;                       //0x0560
    char pad06[0x2184];                         //0x0564
}; //Size = 0x26E8

struct cpose_t
{
    unsigned short lightningHandle;             //0x00
    char eType;                                 //0x02
    char eTypeUnion;                            //0x03
    char pad00[0x2C];                           //0x04
    float origin[3];                            //0x30
    float angles[3];                            //0x3C
    float absmin[3];                            //0x48
    float absmax[3];                            //0x54
}; //Size = 0x60

struct LerpEntityState
{
    int eFlags;                                 //0x00
    int eFlags2;                                //0x04
    char pad00[0x6C];                           //0x08
}; //Size = 0x74 

struct entityState_s
{
    int number;                                 //0x00
    LerpEntityState lerp;                       //0x04
    int time2;                                  //0x78
    unsigned int loopSoundId;                   //0x7C
    int solid;                                  //0x80
    int renderOptions;                          //0x84
    char pad01[0x10];                           //0x88
    int itemIndex;                              //0x98
    char pad02[0x14];                           //0x9C
    char events[4];                             //0xB0
    unsigned short eventParms[4];               //0xB4
    unsigned short eventParm;                   //0xBC
    short eType;                                //0xBE
    char pad04[0xA];                            //0xC0
    unsigned short weapon;                      //0xCA
    unsigned short lastStandPrevWeapon;         //0xCC
    unsigned short targetname;                  //0xCE
    short loopSoundFade;                        //0xD0
    short eventSequence;                        //0xD2
    char surfType;                              //0xD4
    char clientNum;                             //0xD5
    char pad05[0xA];                            //0xD6
}; //Size = 0xE0

struct centity_s
{
    cpose_t pose;                               //0x000
    LerpEntityState lerp;                       //0x060
    char pad00[0x114];                          //0x0D4
    entityState_s nextState;                    //0x1E8
    short previousEventSequence;                //0x2C8
    char pad01[0x5A];                           //0x2CA
    int alive;                                  //0x324

    unsigned short GetEvent(int i) { return this->nextState.events[i & 3]; }
}; //Size = 0x328

struct refdef_s
{
    unsigned int x;                             //0x0000
    unsigned int y;                             //0x0004
    unsigned int width;                         //0x0008
    unsigned int height;                        //0x000C
    float tanHalfFovX;                          //0x0010
    float tanHalfFovY;                          //0x0014
    float fov_x;                                //0x0018
    char pad00[0x4];                            //0x001C
    float vieworg[3];                           //0x0020
    float yaw;                                  //0x002C
    char pad01[0xC];                            //0x0030
    float viewaxis[3][3];                       //0x003C
    int time;                                   //0x0060
}; //Size = 0x64

struct score_s
{
    int ping;                                   //0x00
    int status_icon;                            //0x04
    int place;                                  //0x08
    int score;                                  //0x0C
    int kills;                                  //0x10
    int assists;                                //0x14
    int deaths;                                 //0x18
    int scoreboardColumns[4];                   //0x1C
}; //Size = 0x2C

struct clientInfo_t
{
    int infoValid;                              //0x000
    int nextValid;                              //0x004
    int clientNum;                              //0x008
    char name[0x20];                            //0x00C
    int team;                                   //0x02C
    int oldteam;                                //0x030
    int ffateam;                                //0x034
    int rank;                                   //0x038
    int prestige;                               //0x03C
    int needsRevive;                            //0x040
    unsigned int perks[2];                      //0x044
    char pad00[0x8];                            //0x04C
    unsigned __int64 xuid;                      //0x054
    char clanAbbrev[8];                         //0x05C
    score_s score;                              //0x064
    struct Material *hStatusIcon;               //0x090
    struct Material *hRankIcon;                 //0x094
    char pad02[0xC];                            //0x098
    char model[0x40];                           //0x0A4
    char attachModelNames[6][0x40];            //0x0E4
    char attachTagNames[6][0x40];               //0x264
    char pad03[0x6C];                           //0x3E4
    float playerAngles[3];                      //0x450
    char pad04[0x174];                          //0x45C
}; //Size = 0x5D0

struct snapshot_s
{
    int snapFlags;                              //0x00000
    int ping;                                   //0x00004
    int serverTime;                             //0x00008
    int physicsTime;                            //0x0000C
    playerState_s ps;                           //0x00010
    int numEntities;                            //0x026F8
    int numClients;                             //0x026FC
    entityState_s entities[512];                //0x02700
};

struct cg_s
{
    int clientNum;                              //0x00000
    int localClientNum;                         //0x00004
    char pad00[0x20];                           //0x00008
    snapshot_s *snap;                           //0x00028
    snapshot_s *nextSnap;                       //0x0002C
    char pad01[0x40658];                        //0x00030
    int time;                                   //0x40688
    int oldTime;                                //0x4068C
    int physicsTime;                            //0x40690
    int mapRestart;                             //0x40694
    int renderingThirdPerson;                   //0x40698
    playerState_s predictedPlayerState;         //0x4069C
    char pad02[0x37C];                          //0x42D84
    refdef_s refdef;                            //0x43100
    char pad03[0x184BC];                        //0x43164
    float refdefViewAngles[3];                  //0x5B620
    char pad04[0x3824];                         //0x5B62C
    int weaponSelect;                           //0x5EE50
    char pad05[0x140];                          //0x5EE54    
    float gunPitch;                             //0x5EF94
    float gunYaw;                               //0x5EF98
    char pad06[0x50];                           //0x5EF9C
    float compassNorthYaw;                      //0x5EFEC
    float compassNorth[2];                      //0x5EFF0
    struct Material *compassMapMaterial;        //0x5EFF8
    float compassMapUpperLeft[2];               //0x5EFFC
    float compassMapWorldSize[2];               //0x5F004
    char pad07[0x40];                           //0x5F00C
    float zoomSensitivity;                      //0x5F04C
    char pad08[0x88];                           //0x5F050
    int    inKillCam;                           //0x5F0D8
    char pad09[0x15C];                          //0x5F0DC
    clientInfo_t clients[0x12];                 //0x5F238
    char pad10[0xA038];                         //0x65AD8
    float aimSpreadScale;                       //0x6FB10
    char pad11[0x1CB0];                         //0x6FB14
    int globalScramblerActive;                  //0x717C4
}; 

struct cgs_t
{
    int viewX;                                  //0x000
    int viewY;                                  //0x004
    int viewWidth;                              //0x008
    int viewHeight;                             //0x00C
    float viewAspect;                           //0x010
    int serverCommandSequence;                  //0x014
    int processSnapshotNum;                     //0x018
    int localServer;                            //0x01C
    char gametype[0x20];                        //0x020
    char szHostName[0x100];                     //0x040
    int maxclients;                             //0x140
    char mapname[0x40];                         //0x144
    char pad00[0x3044];                         //0x184
}; //Size = 0x31C8

struct clSnapshot_t
{
    int valid;                                  //0x000000        
    int snapFlags;                              //0x000004
    int serverTime;                             //0x000008
    int physicsTime;                            //0x00000C
    int messageNum;                             //0x000010
    int deltaNum;                               //0x000014
    int ping;                                   //0x000018
    int cmdNum;                                 //0x00001C
    playerState_s ps;                           //0x000020
    int numEntities;                            //0x002708
    int numClients;                             //0x00270C
    int parseMatchStateNum;                     //0x002710
    int parseEntitiesNum;                       //0x002714
    int parseClientsNum;                        //0x002718
    int serverCommandNum;                       //0x00271C
}; //Size = 0x2720

struct usercmd_s
{
    int serverTime;                             //0x00
    int button_bits[2];                         //0x04
    int angles[3];                              //0x0C
    unsigned short weapon;                      //0x18
    unsigned short offHandIndex;                //0x1A
    unsigned short lastWeaponAltModeSwitch;     //0x1C
    char forwardmove;                           //0x1E
    char rightmove;                             //0x1F
    char upmove;                                //0x20
    char pitchmove;                             //0x21
    char yawmove;                               //0x22
    float meleeChargeYaw;                       //0x23
    char meleeChargeDist;                       //0x27
    float rollmove;                             //0x28
    char selectedLocation[2];                   //0x2C
    char selectedYaw;                           //0x2E
    char pad00[0x5];                            //0x2F
}; //Size = 0x34

struct clientActive_t
{
    bool usingAds;                              //0x000000
    char pad00[0x3];                            //0x000001
    int timeoutcount;                           //0x000004
    clSnapshot_t snap;                          //0x000008
    char pad01[0x124];                          //0x002728
    float viewangles[3];                        //0x00284C
    char pad02[0x40010];                        //0x002858
    usercmd_s cmds[0x80];                       //0x042868
    int cmdNumber;                              //0x044268
};  //Size = 0x04426C

struct WeaponDef
{
    const char *szOverlayName;                  //0x000
    struct XModel **gunXModel;                  //0x004
    struct XModel *handXModel;                  //0x008
    const char *szModeName;                     //0x00C
    unsigned short *notetrackSoundMapKeys;      //0x010
    unsigned short *notetrackSoundMapValues;    //0x014
    int playerAnimType;                         //0x018
    int weapType;                               //0x01C
    int weapClass;                              //0x020
    int penetrateType;                          //0x024
    int impactType;                             //0x028
    int inventoryType;                          //0x02C
    char pad00[0x5C];                           //0x030
    const char *projectileSound;                //0x08C
    const char *pullbackSound;                  //0x090
    const char *pullbackSoundPlayer;            //0x094
    const char *fireSound;                      //0x098
    const char *fireSoundPlayer;                //0x09C
    char pad01[0x280];                          //0x0A0
    struct Material *hudIcon;                   //0x320
    int hudIconRatio;                           //0x324
    struct Material *indicatorIcon;             //0x328
    int indicatorIconRatio;                     //0x32C
    struct Material *ammoCounterIcon;           //0x330
    int ammoCounterIconRation;                  //0x334
    int ammoCounterClip;                        //0x338
    int iStartAmmo;                             //0x33C
    int iHeatIndex;                             //0x340
    int iMaxAmmo;                               //0x344
    int shotCount;                              //0x348
    char pad02[0x180];                          //0x34C
    float fHipSpreadDuckedMin;                  //0x4CC
    float fHipSpreadStandMin;                   //0x4D0
    float fHipSpreadProneMin;                   //0x4D4
    float hipSpreadDuckedMax;                   //0x4D8
    float hipSpreadStandMax;                    //0x4DC
    float hipSpreadProneMax;                    //0x4E0
    char pad03[0x68];                           //0x4E4
    bool sharedAmmo;                            //0x54C
    bool bRifleBullet;                          //0x54D
    bool armorPiercing;                         //0x54E
    bool bBoltAction;                           //0x54F
    char pad04[0x168];                          //0x550
    float fAdsViewKickYawMin;                   //0x6B8
    float fAdsViewKickYawMax;                   //0x6BC
    float fAdsViewScatterMin;                   //0x6C0
    float fAdsViewScatterMax;                   //0x6C4
    float fAdsSpread;                           //0x6C8
    char pad05[0x88];                           //0x6CC
    float aiSpread;                             //0x754
    float playerSpread;                         //0x758
    char pad06[0x44];                           //0x75C
    int minDamage;                              //0x7A0
    int minPlayerDamage;                        //0x7A4
    float fMaxDamageRange;                      //0x7A8
    float fMinDamageRange;                      //0x7AC
}; //Size = 0x7B0

struct WeaponVariantDef
{
    const char *szInternalName;                 //0x000
    int iVariantCount;                          //0x004
    WeaponDef *weapDef;                         //0x008
    const char *szDisplayName;                  //0x00C
    const char **szXAnims;                      //0x010
    const char *szAltWeaponName;                //0x014
    unsigned __int16 *hideTags;                 //0x018
    unsigned int altWeaponIndex;                //0x01C
    int iClipSize;                              //0x020
    int iReloadTime;                            //0x024
    int iReloadEmptyTime;                       //0x028
    int iReloadQuickTime;                       //0x02C
    int iReloadQuickEmptyTime;                  //0x030
    int iAdsTransInTime;                        //0x034
    int iAdsTransOutTime;                       //0x038
    int iAltRaiseTime;                          //0x03C
    const char *szAmmoName;                     //0x040
    int iAmmoIndex;                             //0x044
    const char *szClipName;                     //0x048
    int iClipIndex;                             //0x04C
    float fAimAssistRangeAds;                   //0x050
    float fAdsSwayHorizScale;                   //0x054
    float fAdsSwayVertScale;                    //0x058
    float fAdsViewKickCenterSpeed;              //0x05C
    float fHipViewKickCenterSpeed;              //0x060
    float fAdsZoomFov1;                         //0x064
    float fAdsZoomFov2;                         //0x068
    float fAdsZoomFov3;                         //0x06C
    float fAdsZoomInFrac;                       //0x070
    float fAdsZoomOutFrac;                      //0x074
    float fOverlayAlphaScale;                   //0x078
    float fOOPosAnimLength[2];                  //0x07C
    bool bSilenced;                             //0x084
    bool bDualMag;                              //0x085
    bool bFullMetalJacket;                      //0x086
    bool bHollowPoint;                          //0x087
    bool bRapidFire;                            //0x088
    char pad00[0x3];                            //0x089
    Material *overlayMaterial;                  //0x08C
    Material *overlayMaterialLowRes;            //0x090
    Material *dpadIcon;                         //0x094
    int dpadIconRatio;                          //0x098
}; //Size = 0x9C

struct rectDef_s
{
    float x;                                    //0x00
    float y;                                    //0x04
    float w;                                    //0x08
    float h;                                    //0x0C
    int horzAlign;                              //0x10
    int vertAlign;                              //0x14
}; // Size = 0x18

struct KeyState
{
    int down;                                   //0x00
    int repeats;                                //0x04
    const char *binding;                        //0x08
    const char *binding2;                       //0x0C
}; //Size = 0x10

struct snd_asset
{
    const char *filename;                       //0x00
    unsigned int version;                       //0x04
    unsigned int frame_count;                   //0x08
    unsigned int frame_rate;                    //0x0C
    unsigned int channel_count;                 //0x10
    unsigned int header_count;                  //0x14
    unsigned int buffer_size;                   //0x18
    int format;                                 //0x1C
    int channel_flags;                          //0x20
    int unsigned seek_table_count;              //0x24
    unsigned int *seek_table;                   //0x28
    char pad00[0x4];                            //0x2C
    unsigned int data_size;                     //0x30
    char *data;                                 //0x34
};

struct SoundFile
{
    snd_asset *ref;                             //0x00
};

struct snd_alias_t
{
    const char *name;                           //0x00
    unsigned int id;                            //0x04
    const char *subtitle;                       //0x0C
    const char *secondaryname;                  //0x10
    SoundFile *soundFile;                       //0x14
};

struct snd_alias_list_t
{
    const char *name;                           //0x00
    unsigned int id;                            //0x04
    snd_alias_t *head;                          //0x08
    int count;                                  //0x0C
    int sequence;                               //0x10
};

struct SndBank
{
    const char *name;                           //0x00
    unsigned int aliasCount;                    //0x04
    struct snd_alias_list_t *alias;             //0x08
    struct SndIndexEntry
    {
        unsigned __int16 value;
        unsigned __int16 next;
    } *aliasIndex;                              //0x0C
    unsigned int packHash;                      //0x10
    unsigned int packLocation;                  //0x14
    unsigned int radverbCount;                  //0x18
    struct snd_radverb *radverbs;               //0x1C
    unsigned int snapshotCount;                 //0x20
    struct snd_snapshot *snapshots;             //0x24
};

enum FuncAddresses : DWORD
{
    UI_DrawText_a                               = 0x4D65A0,
    CL_RegisterFont_a                           = 0x57D940,
    UI_DrawRect_a                               = 0x4ED000,
    UI_FillRect_a                               = 0x529C40,
    CG_GetEntity_a                              = 0x5A5CC0,
    WorldPosToScreenPos_a                       = 0x49BA00,
    Material_RegisterHandle_a                   = 0x6DBEF0,
    CG_DrawRotatedPicPhysical_a                 = 0x449300,
    Dvar_FindVar_a                              = 0x4D01A0,
    SL_FindString_a                             = 0x4E3FC0,
    Com_GetClientDObj_a                         = 0x6ACD60,
    CG_DObjGetLocalTagMatrix_a                  = 0x5D63B0,
    vectoangles_a                               = 0x4D5230,
    R_AddCmdDrawText_a                          = 0x6F9090,
    R_TextWidth_a                               = 0x6F7050,
    R_TextHeight_a                              = 0x6F7140,
    UI_GetFontHandle_a                          = 0x552F20,
    UI_TextWidth_a                              = 0x6847A0,
    UI_TextHeight_a                             = 0x4436E0,
    AimTarget_IsTargetVisible_a                 = 0x7AF2F0,
    AimTarget_IsTargetValid_a                   = 0x7AEF50,
    BG_GetSpreadForWeapon_a                     = 0x604A60,
    BG_GetWeaponDef_a                           = 0x516C40,
    BG_GetWeaponVariantDef_a                    = 0x40F2C0,
    DrawSketchPicGun_a                          = 0x7CE270,
    CG_GetPlayerViewOrigin_a                    = 0x679940,
    CG_BulletEndpos_a                           = 0x803020,
    AngleVectors_a                              = 0x657D30,
    ClampChar_a                                 = 0x6624F0,
    RandomBulletDir_a                           = 0x802FA0,
    CG_CompassUpYawVector_a                     = 0x476540,
    CG_WorldPosToCompass_a                      = 0x4C45E0,
    CG_CompassDrawPlayer_a                      = 0x403500,
    CG_CompassDrawDogs_a                        = 0x4BD180,
    CG_CompassDrawHelicopter_a                  = 0x4C9DA0,
    CG_CompassDrawPlayerPointers_MiniMap_a      = 0x566D30,
    CG_CompassCalcDimensions_a                  = 0x65DEE0,
    CG_CompassDrawFriendlies_a                  = 0x67E4E0,
    CG_CompassDrawEnemies_a                     = 0x6951D0,
    CG_CompassDrawTurrets_a                     = 0x6AB4D0,
    CG_CompassDrawVehicles_a                    = 0x55A600,
    CG_CompassDrawGuidedMissle_a                = 0x4FCA30,
    CG_CompassDrawIncomingArtilleryIcon_a       = 0x55DCB0,
    CG_CompassDrawBorder_a                      = 0x6AA850,
    CG_CompassDrawFakeFire_a                    = 0x44D6E0,
    CG_CompassDrawRadarEffects_a                = 0x6A9D90,
    CG_CompassDrawArtilleryIcon_a               = 0x5C9600,
    CG_CompassDrawPlayerMap_a                   = 0x55FBC0,
    BG_GetPerkIndexForName_a                    = 0x5E6C80,
    CG_PlaySound_a                              = 0x411260,
    SND_FindAlias_a                             = 0x575A60,
    Sys_Milliseconds_a                          = 0x675F60,
    CG_DrawRotatedPic_a                         = 0x68FA00,
    CL_DrawTextWithEffects_a                    = 0x5674D0,
    UI_DrawTextWithGlow_a                       = 0x42D970,
    ScrPlace_ApplyRect_a                        = 0x4B11A0,
    ScrPlace_HiResGetScale_a                    = 0x43A090,
    Menus_ShowByName_a                          = 0x5C29C0,
    Menus_HideByName_a                          = 0x4EFE90,
    Menus_FindByName_a                          = 0x4C3440,
    Com_HashString_a                            = 0x6ABFC0,
    UI_DrawHandlePic_a                          = 0x42E6C0,
    CalcCompassFriendlySize_a                   = 0x634620,
    AngleNormalize360_a                         = 0x43E060,
};

namespace Colors
{
    struct Color
    {
        float r, g, b, a;

        Color() : r(0.0F), g(0.0f), b(0.0f), a(1.0f) {}
        Color(float r, float g, float b, float a)
            : r(r / 255.0f), g(g / 255.0f), b(b / 255.0f), a(a / 255.0f) {}

        operator float*() { return reinterpret_cast<float*>(this); }
    };

    extern Color white;
    extern Color black;
    extern Color red;
    extern Color green;
    extern Color blue;
    extern Color transparentBlack;
}

struct Fonts
{
    int index;
    const char *dir;
};

using QWORD = unsigned long long;
using __usercall = void*;

namespace GameData
{
    extern std::vector<QWORD> detours;
    extern UiContext *dc;
    extern ScreenPlacement *scrPlace;
    extern cg_s *cgameGlob;
    extern clientActive_t *clientActive;
    extern cgs_t *cgs;
    extern KeyState *keys;
    extern Fonts normalFont;
    extern Fonts smallFont;
    extern Fonts consoleFont;
    extern Fonts objectiveFont;
    extern HWND *hWnd;
    extern SndBank **sndBank;
    extern int *g_sndBankCount;
    extern const unsigned char prevOps[0x17];
}
using namespace GameData;

extern void(__cdecl *UI_DrawText)(ScreenPlacement *scrPlace,
    const char *text, int maxChars, Font_s *font, float x, float y,
    int horzAlign, int vertAlign, float scale, const float *color, int style);
extern bool(__cdecl *WorldPosToScreenPos)(int localClientNum, const float *worldPos,
    float *outScreenPos);
extern centity_s*(__cdecl *CG_GetEntity)(int localClientNum, int entityIndex);
extern Font_s*(__cdecl *CL_RegisterFont)(const char *name, int imageTrack);
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
extern void(__cdecl *R_AddCmdDrawText)(const char *text, int maxChars,
    Font_s *font, float x, float y, float xScale, float yScale, float rotation,
    const float *color, int style);
extern int(__cdecl *R_TextWidth)(const char *text, int maxChars, Font_s *font);
extern int(__cdecl *R_TextHeight)(Font_s *font);
extern Font_s*(__cdecl *UI_GetFontHandle)(ScreenPlacement *scrPlace, int fontEnum,
    float scale);
extern int(__cdecl *UI_TextWidth)(const char *text, int maxChars, Font_s *font,
    float scale);
extern int(__cdecl *UI_TextHeight)(Font_s *font, float scale);
extern void(__cdecl *BG_GetSpreadForWeapon)(playerState_s *ps, WeaponDef *weap,
    float *minSpread, float *maxSpread);
extern WeaponDef*(__cdecl *BG_GetWeaponDef)(int weaponIndex);
extern WeaponVariantDef*(__cdecl *BG_GetWeaponVariantDef)(int weaponIndex);
extern bool(__cdecl *CG_GetPlayerViewOrigin)(int localClientNum,
    playerState_s *ps, float *origin);
extern void(__cdecl *AngleVectors)(const float *angles, float *forward,
    float *right, float *up);
extern char(__cdecl *ClampChar)(int c);
extern void(__cdecl *RandomBulletDir)(int randSeed, float *x, float *y);
extern unsigned int(__cdecl *BG_GetPerkIndexForName)(const char *perkName);
extern int(*Sys_Milliseconds)();
extern void(__cdecl *CG_DrawRotatedPic)(ScreenPlacement *scrPlace,
    float x, float y, float width, float height, int horzAlign, int vertAlign,
    float angle, const float *color, struct Material *material);
extern void(__cdecl *CL_DrawTextWithEffects)(ScreenPlacement *scrPlace,
    const char *text, int maxChars, Font_s *font, float x, float y,
    float rotation, int horzAlign, int vertAlign, float xScale, float yScale,
    const float *color, int style, const float *glowColor,
    struct Material *fxMaterial, Material *fxMaterialGlow, int fxBirthTime,
    int fxLetterTime, int fxDecayStartTime, int fxDecayDuration);
extern void(__cdecl *UI_DrawTextWithGlow)(ScreenPlacement *scrPlace,
    const char *text, int maxChars, Font_s *font, float x, float y,
    int horzAlign, int vertAlign, float scale, const float *color,
    int style, const float *glowColor, bool subtitle, bool cinematic);
extern void(__cdecl *ScrPlace_ApplyRect)(ScreenPlacement *scrPlace,
    float *x, float *y, float *w, float *h, int horzAlign, int vertAlign);
extern float(*ScrPlace_HiResGetScale)();
extern void(__cdecl *Menus_HideByName)(UiContext *dc, const char *menuName);
extern struct menuDef_t*(__cdecl *Menus_FindByName)(UiContext *dc, 
    int hash);
extern int(__cdecl *Com_HashString)(const char *fname, int len);
extern void(__cdecl *UI_DrawHandlePic)(ScreenPlacement *scrPlace, float x, float y,
    float w, float h, int horzAlign, int vertAlign, const float *color,
    struct Material *material);
extern void(__cdecl *CG_CompassDrawPlayerMap)(int localClientNum, int compassType,
    const rectDef_s *parentRect, const rectDef_s *rect, struct Material *material,
    float *color, bool grid);
extern bool(__cdecl *CG_WorldPosToCompass)(int compassType, cg_s *cgameGlob,
    const rectDef_s *mapRect, const float *north, const float *playerWorldPos,
    const float *in, float *out, float *outClipped);
extern void(__cdecl *CG_CompassUpYawVector)(cg_s *cgameGlob, float *result);
extern void(__cdecl *CalcCompassFriendlySize)(int compassType, float *w,
    float *h);
extern void(__cdecl *CG_CompassCalcDimensions)(int compassType,
    cg_s *cgameGlob, const rectDef_s *parentRect, const rectDef_s *rect,
    float *x, float *y, float *w, float *h);
extern float(__cdecl *AngleNormalize360)(const float angle);
extern void(__cdecl *CG_CompassDrawVehicles)(int localClientNum, int compassType,
    int eType, const rectDef_s *parentRect, const rectDef_s *rect, const float *color);
extern void(__cdecl *CG_CompassDrawHelicopter)(int localClientNum, int compassType,
    int eType, const rectDef_s *parentRect, const rectDef_s *rect, const float *color);
extern void(__cdecl *CG_CompassDrawPlayer)(int localClientNum, int compassType,
    const rectDef_s *parentRect, const rectDef_s *rect, struct Material *material,
    const float *color);
extern void(__cdecl *CG_CompassDrawPlayerPointers_MiniMap)(int localClientNum,
    int compassType, const rectDef_s *parentRect, const rectDef_s *rect,
    struct Material *material, const float *color);
extern void(__cdecl *CG_CompassDrawDogs)(int localClientNum, int compassType,
    int eType, const rectDef_s *parentRect, const rectDef_s *rect,
    struct Material *dogMaterial, const float *color);
extern void(__cdecl *CG_CompassDrawGuidedMissle)(int localClientNum, int compassType,
    int eType, const rectDef_s *parentRect, const rectDef_s *rect,
    const float *color);
extern void(__cdecl *CG_CompassDrawTurrets)(int localClientNum, int compassType,
    int eType, const rectDef_s *parentRect, const rectDef_s *rect,
    const float *color);
extern void(__cdecl *CG_CompassDrawIncomingArtilleryIcon)(int localClientNum,
    int compassType, const rectDef_s *parentRect, const rectDef_s *rect,
    const float *color);
extern void(__cdecl *CG_CompassDrawArtilleryIcon)(int localClientNum, int compassType,
    const rectDef_s *parentRect, const rectDef_s *rect, const float *color);
extern void(__cdecl *CG_CompassDrawBorder)(int localClientNum, int compassType,
    const rectDef_s *parentRect, const rectDef_s *rect, struct Material *material,
    const float *color);
extern void(__cdecl *CG_CompassDrawFakeFire)(int localClientNum, int compassType,
    const rectDef_s *parentRect, const rectDef_s *rect, const float *color);
extern void(__cdecl *CG_CompassDrawRadarEffects)(int localClientNum,
    int compassType, const rectDef_s *parentRect, const rectDef_s *rect,
    const float *color);
extern void(__cdecl *CG_PlaySound)(int localClientNum, int entityNum,
    const float *origin, int fadeMs, bool doNotify, 
    float attentuation, unsigned int id);
extern int(__cdecl *SND_FindAlias)(const char *name);

bool InGame();
bool AimTarget_GetTagPos(centity_s *cent, const char *tagname, float *pos);
bool AimTarget_IsTargetVisible(centity_s *cent, const char *visbone);
bool AimTarget_IsTargetValid(centity_s *cent, cg_s *cgameGlob);
void DrawSketchPicGun(ScreenPlacement *scrPlace, rectDef_s *rect,
    const float *color, struct Material *material, int ratio);
void CG_BulletEndpos(int randSeed, const float spread, const float *start,
    float *end, float *dir, const float *forwardDir, const float *rightDir,
    const float *upDir, const float maxRange);
bool BG_HasPerk(int index, const char *perkName);
bool Key_IsDown(const char *bind);
bool IN_IsForegroundWindow();
bool WorldPosToCompass(const centity_s *cent, int compassType,
    const rectDef_s *mapRect, rectDef_s *itemRect);

#pragma pack(pop)
