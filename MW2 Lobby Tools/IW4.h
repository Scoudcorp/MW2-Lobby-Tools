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

#include "ClientManager.h"

namespace IW4
{
	class SV
	{
	public:
		typedef void(__cdecl* MapRestart_t)(bool fast_restart);
		static MapRestart_t MapRestart;

		typedef unsigned int(__cdecl* GetConfigStringConst_t)(int index);
		static GetConfigStringConst_t GetConfigStringConst;

		enum svscmd_type {
			CMD_CAN_IGNORE,
			CMD_RELIABLE
		};
		typedef void(__cdecl* GameSendServerCommand_t)(int clientNum, svscmd_type type, const char *text);
		static GameSendServerCommand_t GameSendServerCommand;

		typedef gentity_t *(__cdecl* GentityNum_t)(int num);
		static GentityNum_t GentityNum;

		typedef void(__cdecl* UserinfoChanged_t)(void *client_s);
		static UserinfoChanged_t UserinfoChanged;

		typedef void(__cdecl* DropClient_t)(void *client_s, const char *reason, BOOL tellThem);
		static DropClient_t DropClient;

		typedef void(__cdecl* SendDisconnect_t)(void *client_s, int state, const char *reason);
		static SendDisconnect_t SendDisconnect;
		
		typedef void(__cdecl* GameDropClient_t)(int clientNum, const char *reason);
		static GameDropClient_t GameDropClient;

		typedef int(__cdecl* GetGuid_t)(int clientNum);
		static GetGuid_t GetGuid;

		typedef playerstate_t*(__cdecl* GameClientNum_t)(int num);
		static GameClientNum_t GameClientNum;

		typedef void*(__cdecl* AddTestClient_t)();
		static AddTestClient_t AddTestClient;

		typedef void(__cdecl* SpawnServer_t)(const char *server);
		static SpawnServer_t SpawnServer;

		typedef void(__cdecl* MatchEnd_t)();
		static MatchEnd_t MatchEnd;

		typedef BOOL(__cdecl *SetBrushModel_t)(gentity_t *ent);
		static SetBrushModel_t SetBrushModel;

		typedef void(__cdecl *LinkEntity_t)(gentity_t *gEnt);
		static LinkEntity_t LinkEntity;

		typedef void(__cdecl *UnlinkEntity_t)(gentity_t *gEnt);
		static UnlinkEntity_t UnlinkEntity;
	};

	class DVAR
	{
	public:
		typedef void(__cdecl* SetFromStringByName_t)(const char *dvarName, const char *string);
		static SetFromStringByName_t SetFromStringByName;

		typedef void(__cdecl* SetStringByName_t)(const char *dvarName, const char *value);
		static SetStringByName_t SetStringByName;

		typedef dvar_t* (__cdecl* FindDvar_t)(const char* dvarname);
		static FindDvar_t FindDvar;

		typedef void(__cdecl* SetString_t)(void *dvar_s, const char *value);
		static SetString_t SetString;

		typedef const char*(__cdecl* GetString_t)(const char *dvarName);
		static GetString_t GetString;

		typedef void(__cdecl* SetBoolByName_t)(const char *dvarName, bool value);
		static SetBoolByName_t SetBoolByName;

		typedef bool(__cdecl* GetBool_t)(const char *dvarName);
		static GetBool_t GetBool;

		typedef dvar_t*(__cdecl* RegisterFloat_t)(const char *dvarName, float value, float min, float max, unsigned __int16 flags, const char *description);
		static RegisterFloat_t RegisterFloat;

		typedef dvar_t*(__cdecl* RegisterInt_t)(const char *dvarName, int value, int min, int max, unsigned __int16 flags, const char *description);
		static RegisterInt_t RegisterInt;

		static void IW4::DVAR::PrintDvar(dvar_t * dvar);
	};

	class SL
	{
	public:
		typedef const char* (__cdecl* ConvertToString_t)(unsigned int stringValue);
		static ConvertToString_t ConvertToString;

		typedef unsigned int(__cdecl* GetString_t)(const char *str, unsigned int user);
		static GetString_t GetString;
	};

	class CL
	{
	public:
		typedef void* (__cdecl* PickSoundAlias_t)(const char *aliasname);
		static PickSoundAlias_t PickSoundAlias;

		typedef void(__cdecl* DrawTextPhysical_t)(const char *text, int maxChars, void *Font_s, float x, float y, float xScale, float yScale, color_t *color, int style);
		static DrawTextPhysical_t DrawTextPhysical;
	};

	class PARTY
	{
	public:
		typedef void(__cdecl* PlayPlayerJoinSound_t)(const int localClientNum);
		static PlayPlayerJoinSound_t PlayPlayerJoinSound;

		//TODO: find "Live_GetPlayerInAddr"
	};

	class PARTYHOST
	{
	public:
		typedef void(__cdecl* GamestateChanged_t)(void *PartyData_s_party);
		static GamestateChanged_t GamestateChanged;

		typedef void(__cdecl* StartMatch_t)(void *PartyData_s_party, int localControllerIndex);
		static StartMatch_t StartMatch;

		typedef void(__cdecl* BalanceTeams_t)(void *PartyData_s_party);
		static BalanceTeams_t BalanceTeams;
	};

	class G
	{
	public:
		typedef void(__cdecl* SetEquippedOffHand_t)(int clientNum, int offHandIndex);
		static SetEquippedOffHand_t SetEquippedOffHand;

		typedef int(__cdecl* GivePlayerWeapon_t)(playerstate_t *playerState_s, int iWeaponIndex, char altModelIndex, int Akimbo);
		static GivePlayerWeapon_t GivePlayerWeapon;

		typedef playerstate_t *(__cdecl* GetClientState_t)(int clientNum);
		static GetClientState_t GetClientState;

		typedef int(__cdecl* GetWeaponIndexForName_t)(const char *name);
		static GetWeaponIndexForName_t GetWeaponIndexForName;

		typedef void(__cdecl* Say_t)(gentity_t *from, gentity_t *to, int mode, const char *chatText);
		static Say_t Say;

		typedef void(__cdecl* SelectWeaponIndex_t)(int clientNum, int iWeaponIndex);
		static SelectWeaponIndex_t SelectWeaponIndex;

		typedef void(__cdecl* InitializeAmmo_t)(gentity_t *pSelf, int weaponIndex, char weaponModel, int hadWeapon);
		static InitializeAmmo_t InitializeAmmo;

		typedef gentity_t*(__cdecl* Spawn_t)();
		static Spawn_t Spawn;

		typedef int(__cdecl* CallSpawnEntity_t)(gentity_t *ent);
		static CallSpawnEntity_t CallSpawnEntity;

		typedef void(__cdecl* FreeEntity_t)(gentity_t *ed);
		static FreeEntity_t FreeEntity;

		typedef void(__cdecl* SetModel_t)(gentity_t *ent, const char *modelName);
		static SetModel_t SetModel;

		typedef void(__cdecl* SetOrigin_t)(gentity_t *ent, const float *origin);
		static SetOrigin_t SetOrigin;

		typedef void(__cdecl* SetAngle_t)(gentity_t *ent, const float *angle);
		static SetAngle_t SetAngle;

		typedef int(__cdecl* ModelIndex_t)(const char *name);
		static ModelIndex_t ModelIndex;

		typedef void(__cdecl* EnterWorld_t)(gentity_t *ent, int flag); // TODO: find actual name
		static EnterWorld_t EnterWorld;

		typedef void(__cdecl* LocationalTrace_t)(trace_t *results, const float *start, const float *end, int passEntityNum, int contentmask, char *priorityMap);
		static LocationalTrace_t LocationalTrace;

		typedef void(__cdecl* GetPlayerViewOrigin_t)(playerstate_t *playerState_s, float *origin);
		static GetPlayerViewOrigin_t GetPlayerViewOrigin;
		
		typedef unsigned int(__cdecl *ModelName_t)(int index);
		static ModelName_t ModelName;
	};

	class SND
	{
	public:
		typedef int(__cdecl* PlaySoundAlias_t)(void* alias, int sndEnt, int org, int timeshift, int system);
		static PlaySoundAlias_t PlaySoundAlias;

		typedef void(__cdecl* PlayClientSound_t)(int a1, int localclient, char* alias);
		static PlayClientSound_t PlayClientSound;

		typedef int(__cdecl* PlayLocalSoundAliasByName_t)(int localClientNum, const char *aliasname, int snd_alias_system_t_system);
		static PlayLocalSoundAliasByName_t PlayLocalSoundAliasByName;
	};

	class PLAYERCMD
	{
	public:
		typedef void(__cdecl* setClientDvar_t)(int scr_entref_t);
		static setClientDvar_t setClientDvar;

		typedef void(__cdecl* giveWeapon_t)(int scr_entref_t);
		static giveWeapon_t giveWeapon;

		typedef void(__cdecl* takeAllWeapons_t)(int scr_entref_t);
		static takeAllWeapons_t takeAllWeapons;

		typedef void(__cdecl* takeWeapon_t)(int scr_entref_t);
		static takeWeapon_t takeWeapon;

		typedef void(__cdecl* switchToWeapon_t)(int scr_entref_t);
		static switchToWeapon_t switchToWeapon;

		typedef void(__cdecl* setSpawnWeapon_t)(int scr_entref_t);
		static setSpawnWeapon_t setSpawnWeapon;

		typedef void(__cdecl* SetActionSlot_t)(int scr_entref_t);
		static SetActionSlot_t SetActionSlot;

		typedef void(__cdecl* UnsetPerk_t)(int scr_entref_t);
		static UnsetPerk_t UnsetPerk;

		typedef void(__cdecl* SetPerk_t)(int scr_entref_t);
		static SetPerk_t SetPerk;

		typedef void(__cdecl* SayAll_t)(int scr_entref_t);
		static SayAll_t SayAll;

		typedef void(__cdecl* iprintlnbold_t)();
		static iprintlnbold_t iprintlnbold;
	};

	class R
	{
	public:
		typedef void*(__cdecl* RegisterFont_t)(const char *name, int imageTrack);
		static RegisterFont_t RegisterFont;

		typedef float(__cdecl* NormalizedTextScale_t)(void *Font_s, float scale);
		static NormalizedTextScale_t NormalizedTextScale;
		
		typedef int(__cdecl* TextHeight_t)(void *Font_s_font);
		static TextHeight_t TextHeight;

		typedef int (__cdecl* TextWidth_t)(const char *text, int maxChars, void *Font_s_font);
		static TextWidth_t TextWidth;

		typedef void (__cdecl* AddCmdDrawText_t)(const char *text, int maxChars, void *Font_s, float x, float y, float xScale, float yScale, float rotation, color_t *color, int style);
		static AddCmdDrawText_t AddCmdDrawText;

		typedef void(__cdecl* AddCmdDrawText2_t)(const char *text, int maxChars, void *Font_s, float x, float y, float xScale, float yScale, float rotation, color *color, int style);
		static AddCmdDrawText2_t AddCmdDrawText2;

		typedef void(__cdecl* AddCmdDrawStretchPic_t)(float x, float y, float w, float h, float s0, float t0, float s1, float t1, color_t *color, void *Material);
		static AddCmdDrawStretchPic_t AddCmdDrawStretchPic;
	};

	class INFO
	{
	public:
		typedef char*(__cdecl* ValueForKey_t)(const char *s, const char *key);
		static ValueForKey_t ValueForKey;

		typedef void(__cdecl* SetValueForKey_t)(char *s, const char *key, const char *value);
		static SetValueForKey_t SetValueForKey;
	};

	class UI
	{
	public:
		typedef int(__cdecl* TextWidth_t)(const char *text, int maxChars, void *Font_s, float scale);
		static TextWidth_t TextWidth;

		typedef void(__cdecl* DrawTextW_t)(void *ScreenPlacement, const char *text, int maxChars, void *Font_s, float x, float y, int horzAlign, int vertAlign, float scale, const float *color, int style);
		static DrawTextW_t DrawTextW;

		typedef void(__cdecl* OpenMenu_t)(int localClientNum, const char *menuName);
		static OpenMenu_t OpenMenu;

		typedef int(__cdecl* PopupScriptMenu_t)(int localClientNum, const char *menuName, bool useMouse);
		static PopupScriptMenu_t PopupScriptMenu;
	};

	class COM
	{
	public:
		typedef void(__cdecl* Error_t)(int errorParm_t, const char *fmt);
		static Error_t Error;
	};

	class CMD
	{
	public:
		typedef void(__cdecl* ExecuteSingleCommand_t)(int localClientNum, int delay, const char *text);
		static ExecuteSingleCommand_t ExecuteSingleCommand;

		typedef void(__cdecl* AddCommandInternal_t)(const char *cmdName, void(__cdecl *function)(), void *cmd_function_s_allocedCmd);
		static AddCommandInternal_t AddCommandInternal;

		typedef void(__cdecl* RemoveCommand_t)(const char *cmdName);
		static RemoveCommand_t RemoveCommand;
	};

	class CBUF
	{
	public:
		typedef void(__cdecl* AddText_t)(int localClientNum, const char *text);
		static AddText_t AddText;
	};

	class BG
	{
	public:
		typedef void *(__cdecl* GetWeaponDef_t)(unsigned int weaponIndex);
		static GetWeaponDef_t GetWeaponDef;

		typedef int(__cdecl* TakePlayerWeapon_t)(void *playerState_s, unsigned int weaponIndex, int takeAwayAmmo);
		static TakePlayerWeapon_t TakePlayerWeapon;

		typedef int(__cdecl* GetViewmodelWeaponIndex_t)(void *playerState_s);
		static GetViewmodelWeaponIndex_t GetViewmodelWeaponIndex;

		typedef unsigned int(__cdecl* GetPerkIndexForName_t)(const char *perkName);
		static GetPerkIndexForName_t GetPerkIndexForName;
	};

	class LIVE
	{
	public:
		typedef unsigned __int64(__cdecl* GetPlayerXuid_t)(void* SessionData_s, const int clientNum);
		static GetPlayerXuid_t GetPlayerXuid;
	};

	class SCR
	{
	public:
		typedef void(__cdecl *NotifyNum_t)(int entnum, unsigned int classnum, unsigned int stringValue, unsigned int paramcount);
		static NotifyNum_t NotifyNum;

		typedef gentity_t *(__cdecl* GetEntity_t)(unsigned int index);
		static GetEntity_t GetEntity;

		typedef int(__cdecl* GetEntityRef_t)(unsigned int index);
		static GetEntityRef_t GetEntityRef;

		typedef unsigned int(__cdecl* GetSelf_t)(unsigned int threadId);
		static GetSelf_t GetSelf;

		typedef void(__cdecl* SetThreadWaitTime_t)(unsigned int startLocalId, unsigned int waitTime);
		static SetThreadWaitTime_t SetThreadWaitTime;

		typedef void(__cdecl* AddString_t)(const char* value);
		static AddString_t AddString;

		typedef void(__cdecl* AddInt_t)(int value);
		static AddInt_t AddInt;

		typedef int(__cdecl* GetInt_t)(unsigned int index);
		static GetInt_t GetInt;

		typedef void(__cdecl* ClearOutParams_t)();
		static ClearOutParams_t ClearOutParams;

		typedef unsigned int(__cdecl *GetNumParam_t)();
		static GetNumParam_t GetNumParam;

		typedef void(__cdecl* SetString_t)(unsigned __int16 *to, unsigned int from);
		static SetString_t SetString;

		typedef const char*(__cdecl* GetString_t)(unsigned int index);
		static GetString_t GetString;
	};

	class TRACE
	{
	public:
		typedef unsigned __int16(__cdecl *GetEntityHitId_t)(void *trace_t_trace);
		static GetEntityHitId_t GetEntityHitId; // returns 0x7FF or 0x7FE if no entity
	};

	class SP
	{
	public:
		typedef void(__cdecl *script_model_t)(gentity_t *pSelf);
		static script_model_t script_model;
	};

	class SCRIPTENTCMD
	{
	public:
		typedef void(__cdecl *Solid_t)(int scr_entref_t);
		static Solid_t Solid;
		typedef void(__cdecl *NotSolid_t)(int scr_entref_t);
		static NotSolid_t NotSolid;
	};

	class MATH
	{
	public:
		typedef void(__cdecl *AngleVectors_t)(const float *angles, float *forward, float *right, float *up);
		static AngleVectors_t AngleVectors;
	};

	class MISC
	{
	public:
		typedef int(__cdecl* Add_Ammo_t)(void *gentity_s, unsigned int weaponIndex, char weaponModel, int count, int fillClip);
		static Add_Ammo_t Add_Ammo;

		static int GetNumParam();
		static void SetNumParam(int num);
		static long long GetXuid(int id);
		static int GetHostId();
		static void SendClientMessageCenter(int clientid, std::string message);
		static void SendClientMessageBold(int clientid, std::string message);
		static void SendAllClientsMessageCenter(std::string message);
		static void SendAllClientsMessageBold(std::string message);
		static void SendAllClientsChatMessage(int from_id, std::string message);
		static void SendClientSound(int clientid, std::string sound);
		static void SetClientDvar(unsigned int clientid, std::string dvar, std::string value);

		static float *GetTraceFromEntView(gentity_t *from);
	};

	class UNKN
	{
	public:
		typedef void(__cdecl* CreateConsole_t)();
		static CreateConsole_t CreateConsole;

		typedef void(__cdecl* player_die_t)(void *gentity_s_self, void *gentity_s_inflictor, void *gentity_s_attacker, int damage, int meansOfDeath, int iWeapon, const float *vDir, int hitLocation_t_hitLoc, int psTimeOffset);
		static player_die_t player_die;

		typedef int(__cdecl* String_Parse_t)(char **p, char *out, int len);
		static String_Parse_t String_Parse;
	};

}

