/*
Copyright (c) 2016 Scoud

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#pragma once
#include <string>

// fonts
#define FONT_SMALL_DEV    "fonts/smallDevFont"
#define FONT_BIG_DEV      "fonts/bigDevFont"
#define FONT_CONSOLE      "fonts/consoleFont"
#define FONT_BIG          "fonts/bigFont"
#define FONT_SMALL        "fonts/smallFont"
#define FONT_BOLD         "fonts/boldFont"
#define FONT_NORMAL       "fonts/normalFont"
#define FONT_EXTRA_BIG    "fonts/extraBigFont"
#define FONT_OBJECTIVE    "fonts/objectiveFont"

// general types
#define arg_0 0x4
#define arg_4 0x8
#define arg_8 0xC

#define OJMP 0xEB // JUMP
#define JZ  0x74 // JUMP ZERO
#define JNZ 0x75 // JUMP NOT ZERO
#define NOP 0x90 // NOP

class rectangle
{
public:
	rectangle() : x1(0), y1(0), x2(0), y2(0) {}
	rectangle(float x1, float y1, float x2, float y2) { this->x1 = x1; this->y1 = y1; this->x2 = x2; this->y2 = y2; };
	float x1;
	float y1;
	float x2;
	float y2;
};

class color
{
public:
	color() : r(0), g(0), b(0), a(0) {};
	color(float r, float g, float b, float a) { this->r = r; this->g = g; this->b = b; this->a = a; };
	float r;
	float g;
	float b;
	float a;
};

typedef float vec3_t[3];

// structs
struct color_t
{
	float _r, _g, _b, _a;

	color_t():
		_r(1),
		_g(1),
		_b(1),
		_a(1)
		{}
	color_t(float r, float g, float b, float a) :
		_r(r),
		_g(g),
		_b(b),
		_a(a)
	{}
};

#define COLOR_DEFAULT			color_t(1, 1, 1, 1) // white
#define COLOR_HIGHLIGHTED		color_t(0, 0, 1, 1) // blue
#define COLOR_ENABLED			color_t(0, 1, 0, 1) // green
#define COLOR_DISABLED			color_t(1, 0, 0, 1) // red
#define COLOR_BACKGROUNDMENU	color_t(1, 1, 1, 1) // black-ish

enum scriptType_e
{
	SCRIPT_NONE = 0,
	SCRIPT_OBJECT = 1,
	SCRIPT_STRING = 2,
	SCRIPT_VECTOR = 4,
	SCRIPT_FLOAT = 5,
	SCRIPT_INTEGER = 6
};

struct VariableValue
{
	union
	{
		void* entity;
		float number;
		unsigned short string;
		float* vector;
		int integer;
	};

	scriptType_e type;
};

struct gentity_t
{
	int EntityNumber;						// 0x00, size = 0x04
			
	char _0x4[0x24];						// pad for 0x24
	
	vec3_t Origin;							// 0x28, size = 0x0C
	
	char _0x34[0x18];						// pad for 0x18
	
	vec3_t Angle;							// 0x4C, size = 0x0C

	char _0x58[0xC4];						// pad for 0xC4

	int content;							// 0x11C, size = 4

	char _0x120[0x18];						// pad for 0x18

	vec3_t Position;						// 0x138, size = 0x0C

	char _0x144[0x14];						// pad for 0x14

	struct playerstate_t *playerstate_s;	// 0x158, size = 0x04

	char _0x15C[0x18];						// pad for 0x18

	short modelclass;						// 0x174, size = 2

	char _0x176[0xA];						// pad for 0xA

	int flags;								// 0x180, size = 4

	char _0x184[0xF0];						// pad for 0xF0
}; // size = 0x0274;

struct cg_t {
	__int32 snapshot;
	char	unknown0[96];
	__int32 IsPlaying; 
	char	unknown1[32];
	__int32 localweaponnum; 
	char	unknown2[36];
	__int32 Pose; 
	char	unknown3[80];
	__int32 ClientNumber; 
	char	unknown4[72];
	__int32 health; 
	char	unknown5[408];
	__int32 mag; 
	char	unknown6[124];
	__int32 clip; 
};

typedef struct {
	char	_p00[8];
	__int32 Screen_Width;
	__int32 Screen_Height;
	float	ScreenXBias;
	__int32 serverCommandSequence;
	__int32 processedSnapshotNum;
	__int32 LocalServer;
	char	GameType[4];
	char	unknown36[28];
	char	ServerName[64];
	char	unknown320[196];
	__int32 Max_Clients;
	char	unknown324[4];
	char	MapName[64];
	char	unknown388[4];
	__int32 Active_Vote;
} cgs_t;

enum EntTypes {
	ET_GENERAL,
	ET_PLAYER,
	ET_PLAYER_CORPSE,
	ET_ITEM,
	ET_MISSILE,
	ET_INVISIBLE,
	ET_SCRIPTMOVER,
	ET_SOUND_MOVER,
	ET_FX,
	ET_LOOP_FX,
	ET_PRIMARY_LIGHT,
	ET_TURRET,
	ET_HELICOPTER,
	ET_PLANE,
	ET_VEHICLE,
	ET_VEHICLE_COLLMAP,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_SPAWNER,
};

typedef struct {
	char	_0x0000[24];
	vec3_t	vOrigin;
	char	_0x0024[72];
	__int32 IsZooming;
	char	_0x0070[12];
	vec3_t	vOldOrigin;
	char	_0x0088[84];
	__int32 clientNum;
	__int32 eType;
	__int32 eFlags;
	char	_0x00E8[12];
	vec3_t	vNewOrigin;
	char	_0x0100[108];
	__int32 pickupItemID;
	__int32 clientNum2;
	char	_0x0174[52];
	unsigned char weaponID;
	char	_0x01A9[51];
	__int32 iAlive;
	char	_0x01E0[32];
	__int32 clientNum3;
} entity_t;

struct playerstate_t
{
	char	_0x0[0x1C];
	vec3_t	Origin;
	vec3_t	Velocity;
	char	_0x22[0x32E4];
	int		Score;
	char	_0x331C[0x70];
	char	PlayerName[32];
	int		MaxHealth;
	char	_0x33B0[0x24];
	int		Team;
	char	_0x33D8[0x78];
	char	ClanTag[4];
	char	_0x3454[0x1A8];
	int		MovementFlags;
	char	_0x3600[0x380];

	void SetOrigin(vec3_t NewOrigin)
	{
		this->Origin[0] = NewOrigin[0];
		this->Origin[1] = NewOrigin[1];
		this->Origin[2] = NewOrigin[2];
	}

	void SetVelocity(vec3_t NewVelocity)
	{
		memcpy(static_cast<PVOID>(Origin), static_cast<PVOID>(NewVelocity), sizeof(float) * 3);
	}
};


struct client_s {
	char	_0x0000[0x28];
	__int32 _0x0028;
	__int32 _0x002C;
	__int32 _0x0030;
	__int32 _0x0034;
	__int32 _0x0038;
	__int32 _0x003C;

	char _unknown[0xA6790];
};

typedef struct {
	__int32 unknown;
	__int32 IsValid;		//0x0000 
	__int32 IsValid2;		//0x0004 
	__int32 clientNum;		//0x0008 
	char	szName[16];		//0x000C 
	__int32 iTeam;			//0x001C 
	__int32 iTeam2;			//0x0020 
	__int32 iRank;			//0x0024 rank+1 
	char	_0x0028[4];
	__int32 iPerk;			//0x002C 
	__int32 iKills;			//0x0030
	__int32 iScore;			//0x0034 
	char	_0x0038[968];
	vec3_t	vAngles;		//0x0400 
	char	_0x040C[136];
	__int32 IsShooting;		//0x0494 
	char	_0x0498[4];
	__int32 IsZoomed;		//0x049C 
	char	_0x04A0[68];
	__int32 weaponID;		//0x04E4 
	char	_0x04E8[24];
	__int32 weaponID2;		//0x0500 
	char	_0x0504[40];
} clientinfo_t;

struct clientconnection_t {
	int id;
	std::string name;
	std::string xuid;
	std::string nat;
	std::string rank;
	std::string prestige;
	bool checked;
	bool trash;

	clientconnection_t():
		id(-1),
		name(""),
		xuid(""),
		nat(""),
		rank(""),
		prestige(""),
		checked(false),
		trash(false)
		{}
};

struct kickedplayer_t {
	std::string name;
	std::string xuid;
};

struct specialclient_t {
	long long xuid;
	std::string message;
};

typedef struct  {
	__int32	x;
	__int32 y;
	__int32 iWidth;
	__int32 iHeight; 
	float	fovX; 
	float	fovY; 
	vec3_t	vOrigin;
	vec3_t	vViewAxis[3];
	char	_0x0048[4];
	vec3_t	vViewAngles;
	__int32	iTime;
	__int32 iMenu; 
	char	_0x0060[16128];
	vec3_t	refdefViewAngles;
	char	_0x3F6C[276];
} refdef_t;

typedef enum
{
	DVAR_FLAG_NONE = 0x0,				//no flags
	DVAR_FLAG_SAVED = 0x1,				//saves in config_mp.cfg for clients
	DVAR_FLAG_LATCHED = 0x2,			//no changing apart from initial value (although it might apply on a map reload, I think)
	DVAR_FLAG_CHEAT = 0x4,				//cheat
	DVAR_FLAG_REPLICATED = 0x8,			//on change, this is sent to all clients (if you are host)
	DVAR_FLAG_UNKNOWN10 = 0x10,			//unknown
	DVAR_FLAG_UNKNOWN20 = 0x20,			//unknown
	DVAR_FLAG_UNKNOWN40 = 0x40,			//unknown
	DVAR_FLAG_UNKNOWN80 = 0x80,			//unknown
	DVAR_FLAG_USERCREATED = 0x100,		//a 'set' type command created it
	DVAR_FLAG_USERINFO = 0x200,			//userinfo?
	DVAR_FLAG_SERVERINFO = 0x400,		//in the getstatus oob
	DVAR_FLAG_WRITEPROTECTED = 0x800,	//write protected
	DVAR_FLAG_UNKNOWN1000 = 0x1000,		//unknown
	DVAR_FLAG_READONLY = 0x2000,		//read only (same as 0x800?)
	DVAR_FLAG_UNKNOWN4000 = 0x4000,		//unknown
	DVAR_FLAG_UNKNOWN8000 = 0x8000,		//unknown
	DVAR_FLAG_UNKNOWN10000 = 0x10000,	//unknown
	DVAR_FLAG_DEDISAVED = 0x1000000,	//unknown
	DVAR_FLAG_NONEXISTENT = 0xFFFFFFFF	//no such dvar
} dvar_flag;

enum DvarSetSource
{
	DVAR_SOURCE_INTERNAL = 0,
	DVAR_SOURCE_EXTERNAL = 1,
	DVAR_SOURCE_SCRIPT = 2
};

union DvarLimits {
	struct
	{
		int stringCount;
		char * * strings;
	} enumeration;
	struct
	{
		int min;
		int max;
	} integer;
	struct
	{
		float min;
		float max;
	} decimal;
};

union DvarValue {
	bool enabled;
	int integer;
	unsigned int unsignedInt;
	float value;
	float vector[4];
	char * string;
	BYTE color[4];
};

typedef enum DvarType : char
{
	DVAR_TYPE_BOOL = 0x00,
	DVAR_TYPE_FLOAT = 0x01,
	DVAR_TYPE_VEC2 = 0x02,
	DVAR_TYPE_VEC3 = 0x03,
	DVAR_TYPE_VEC4 = 0x04,
	DVAR_TYPE_INT = 0x05,
	DVAR_TYPE_ENUM = 0x06,
	DVAR_TYPE_STRING = 0x07,
	DVAR_TYPE_COLOR = 0x08,
	DVAR_TYPE_DEVTWEAK = 0x09
} DvarType;

typedef struct dvar_t
{
	char * name;
	dvar_flag flags;
	DvarType type;
	bool modified;

	DvarValue current;
	DvarValue latched;
	DvarValue reset;
	DvarLimits domain;
	dvar_t * next;
} dvar_t;

struct scr_entref_t {
	int entnum;
	int classnum;
};

struct trace_t
{
	float fraction;//0x00 - 0x04
	float normal[3];//0x04 - 0x10
	int surfaceFlags;//0x10 - 0x14
	int contents;//0x14 - 0x18
	const char *material;//0x18 - 0x1C
	int hitType;//0x1C - 0x20
	unsigned __int16 hitId; //0x20 - 0x22
	unsigned __int16 modelIndex;//0x22 - 0x24
	unsigned __int16 partName;//0x24 - 0x26
	unsigned __int16 partGroup;//0x26 - 0x38
	bool allsolid;//0x28 - 0x29
	bool startsolid;//0x29 - 0x2A
	bool walkable;//0x2A-0x2B
	char __padding;//0x2B-0x2C
};