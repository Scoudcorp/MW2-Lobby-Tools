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

#include "StdAfx.h"
#include "IW4.h"
#include "EntityManager.h"

#define IW4DECLARE(SPACE, NAME, ADDRESS) SPACE##::##NAME##_t SPACE##::##NAME = (SPACE##::##NAME##_t)(ADDRESS)

namespace IW4
{

#pragma region SV
	IW4DECLARE(SV, MapRestart, 0x584FB0);
	IW4DECLARE(SV, GetConfigStringConst, 0x588DC0);
	IW4DECLARE(SV, GameSendServerCommand, 0x5882D0);
	IW4DECLARE(SV, GentityNum, 0x51F390);
	IW4DECLARE(SV, UserinfoChanged, 0x5865A0);
	IW4DECLARE(SV, DropClient, 0x585CA0);
	IW4DECLARE(SV, SendDisconnect, 0x585C20);
	IW4DECLARE(SV, GameDropClient, 0x588320);
	IW4DECLARE(SV, GetGuid, 0x588A10);
	IW4DECLARE(SV, GameClientNum, 0x5882B0);
	IW4DECLARE(SV, AddTestClient, 0x5880D0);
	IW4DECLARE(SV, SpawnServer, 0x589D20);
	IW4DECLARE(SV, MatchEnd, 0x58BBA0);
	IW4DECLARE(SV, SetBrushModel, 0x5884D0);
	IW4DECLARE(SV, LinkEntity, 0x590030);
	IW4DECLARE(SV, UnlinkEntity, 0x58FF90);
#pragma endregion

#pragma region DVAR
	IW4DECLARE(DVAR, FindDvar, 0x5BCE00);
	IW4DECLARE(DVAR, SetStringByName, 0x5BE730);
	IW4DECLARE(DVAR, SetFromStringByName, 0x5BE810);
	IW4DECLARE(DVAR, SetString, 0x5BE4B0);
	IW4DECLARE(DVAR, GetString, 0x5BCED0);
	IW4DECLARE(DVAR, SetBoolByName, 0x5BE5C0);
	IW4DECLARE(DVAR, GetBool, 0x5BCE30);
	IW4DECLARE(DVAR, RegisterFloat, 0x5BDB50);
	IW4DECLARE(DVAR, RegisterInt, 0x5BDB10);
#pragma endregion

#pragma region SL
	IW4DECLARE(SL, ConvertToString, 0x579960);
	IW4DECLARE(SL, GetString, 0x57A310);
#pragma endregion

#pragma region CL
	IW4DECLARE(CL, PickSoundAlias, 0x4D27E5);
	IW4DECLARE(CL, DrawTextPhysical, 0x4C5600);
#pragma endregion

#pragma region PARTY
	IW4DECLARE(PARTY, PlayPlayerJoinSound, 0x4D27C0);
#pragma endregion

#pragma region PARTYHOST
	IW4DECLARE(PARTYHOST, GamestateChanged, 0x4D6610);
	IW4DECLARE(PARTYHOST, StartMatch, 0x4D74D0);
	IW4DECLARE(PARTYHOST, BalanceTeams, 0x4D73B0);
#pragma endregion

#pragma region G
	IW4DECLARE(G, SetEquippedOffHand, 0x5509C0);
	IW4DECLARE(G, GivePlayerWeapon, 0x550D60);
	IW4DECLARE(G, GetClientState, 0x52EE80);
	IW4DECLARE(G, GetWeaponIndexForName, 0x550960);
	IW4DECLARE(G, Say, 0x525F90);
	IW4DECLARE(G, SelectWeaponIndex, 0x550990);
	IW4DECLARE(G, InitializeAmmo, 0x51FD90);
	IW4DECLARE(G, Spawn, 0x54E490);
	IW4DECLARE(G, CallSpawnEntity, 0x54AA60);
	IW4DECLARE(G, FreeEntity, 0x54E700);
	IW4DECLARE(G, SetModel, 0x54DC40);
	IW4DECLARE(G, SetOrigin, 0x54E970);
	IW4DECLARE(G, SetAngle, 0x54E9C0);
	IW4DECLARE(G, ModelIndex, 0x54D700);
	IW4DECLARE(G, EnterWorld, 0x54ED50);
	IW4DECLARE(G, LocationalTrace, 0x52FC80);
	IW4DECLARE(G, GetPlayerViewOrigin, 0x51EE60);
	IW4DECLARE(G, ModelName, 0x54D820);
#pragma endregion

#pragma region SND
	IW4DECLARE(SND, PlaySoundAlias, 0x62E7C0);
	IW4DECLARE(SND, PlayClientSound, 0x498870);
	IW4DECLARE(SND, PlayLocalSoundAliasByName, 0x62ED30);
#pragma endregion

#pragma region PLAYERCMD
	IW4DECLARE(PLAYERCMD, setClientDvar, 0x522070);
	IW4DECLARE(PLAYERCMD, giveWeapon, 0x51FEB0);
	IW4DECLARE(PLAYERCMD, takeAllWeapons, 0x5200C0);
	IW4DECLARE(PLAYERCMD, takeWeapon, 0x520060);
	IW4DECLARE(PLAYERCMD, switchToWeapon, 0x520680);
	IW4DECLARE(PLAYERCMD, setSpawnWeapon, 0x523D40);
	IW4DECLARE(PLAYERCMD, SetActionSlot, 0x522A90);
	IW4DECLARE(PLAYERCMD, UnsetPerk, 0x5233F0);
	IW4DECLARE(PLAYERCMD, SetPerk, 0x523200);
	IW4DECLARE(PLAYERCMD, SayAll, 0x524BA0);
	IW4DECLARE(PLAYERCMD, iprintlnbold, 0x544030);
#pragma endregion

#pragma region R
	IW4DECLARE(R, RegisterFont, 0x40F6D0);
	IW4DECLARE(R, NormalizedTextScale, 0x40F6F0);
	IW4DECLARE(R, TextHeight, 0x40F7C0);
	IW4DECLARE(R, TextWidth, 0x40F710);
	IW4DECLARE(R, AddCmdDrawText, 0x4113D0);
	IW4DECLARE(R, AddCmdDrawText2, 0x4113D0);
	IW4DECLARE(R, AddCmdDrawStretchPic, 0x410E20);
#pragma endregion

#pragma region INFO
	IW4DECLARE(INFO, ValueForKey, 0x5C1FD0);
	IW4DECLARE(INFO, SetValueForKey, 0x5C23F0);
#pragma endregion

#pragma region UI
	IW4DECLARE(UI, TextWidth, 0x59AA60);
	IW4DECLARE(UI, DrawTextW, 0x59AAF0);
	IW4DECLARE(UI, OpenMenu, 0x59DD70);
	IW4DECLARE(UI, PopupScriptMenu, 0x59DC00);
#pragma endregion

#pragma region COM
	IW4DECLARE(COM, Error, 0x569F50);
#pragma endregion

#pragma region CMD
	IW4DECLARE(CMD, ExecuteSingleCommand, 0x564760);
	IW4DECLARE(CMD, AddCommandInternal, 0x5642F0);
	IW4DECLARE(CMD, RemoveCommand, 0x564320);
#pragma endregion

#pragma region CBUF
	IW4DECLARE(CBUF, AddText, 0x563BA0);
#pragma endregion

#pragma region BG
	IW4DECLARE(BG, GetWeaponDef, 0x4794F0);
	IW4DECLARE(BG, TakePlayerWeapon, 0x550C20);
	IW4DECLARE(BG, GetViewmodelWeaponIndex, 0x479580);
	IW4DECLARE(BG, GetPerkIndexForName, 0x46B5A0);
#pragma endregion

#pragma region LIVE
	IW4DECLARE(LIVE, GetPlayerXuid, 0x621300);
#pragma endregion

#pragma region SCR
	IW4DECLARE(SCR, NotifyNum, 0x57FFF0);
	IW4DECLARE(SCR, GetEntity, 0x54AC30);
	IW4DECLARE(SCR, GetEntityRef, 0x5808F0);
	IW4DECLARE(SCR, GetSelf, 0x57B010);
	IW4DECLARE(SCR, SetThreadWaitTime, 0x57ACA0);
	IW4DECLARE(SCR, AddString, 0x580F70);
	IW4DECLARE(SCR, AddInt, 0x580DB0);
	IW4DECLARE(SCR, GetInt, 0x5803B0);
	IW4DECLARE(SCR, ClearOutParams, 0x57F2F0);
	IW4DECLARE(SCR, GetNumParam, 0x580DA0);
	IW4DECLARE(SCR, SetString, 0x57A8A0);
	IW4DECLARE(SCR, GetString, 0x580750);
#pragma endregion

#pragma region TRACE
	IW4DECLARE(TRACE, GetEntityHitId, 0x55D3E0);
#pragma endregion

#pragma region SP
	IW4DECLARE(SP, script_model, 0x5399E0);
#pragma endregion

#pragma region SCRIPTENTCMD
	IW4DECLARE(SCRIPTENTCMD, Solid, 0x54A110);
	IW4DECLARE(SCRIPTENTCMD, NotSolid, 0x54A1B0);
#pragma endregion

#pragma region MATH
	IW4DECLARE(MATH, AngleVectors, 0x5B73B0);
#pragma endregion
	
#pragma region MISC
	IW4DECLARE(MISC, Add_Ammo, 0x52B3C0);
#pragma endregion

#pragma region UNKN
	IW4DECLARE(UNKN, CreateConsole, 0x5CB490);
	IW4DECLARE(UNKN, player_die, 0x526890);
	IW4DECLARE(UNKN, String_Parse, 0x5A2290);
#pragma endregion
}


int IW4::MISC::GetNumParam()
{
	return *reinterpret_cast<int*>(NUMPARAM);
}

void IW4::MISC::SetNumParam(int num)
{
	*reinterpret_cast<int*>(NUMPARAM) = num;
}

long long IW4::MISC::GetXuid(int id)
{
	return static_cast<long long>(IW4::LIVE::GetPlayerXuid(reinterpret_cast<void*>(GAMESTATE_a), id));
}

int IW4::MISC::GetHostId()
{
	auto hostid = reinterpret_cast<int*>(0x10FA938);
	return *hostid;
}

void IW4::MISC::SendClientMessageCenter(int clientid, std::string message)
{
	IW4::SV::GameSendServerCommand(clientid, IW4::SV::CMD_RELIABLE, ("c \"" + message + "\"").c_str());
}

void IW4::MISC::SendClientMessageBold(int clientid, std::string message)
{
	IW4::SV::GameSendServerCommand(clientid, IW4::SV::CMD_RELIABLE, ("f \"" + message + "\"").c_str());
}

void IW4::MISC::SendAllClientsMessageCenter(std::string message)
{
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		if (ClientManager::GetClientById(i)->IsValid)
			IW4::MISC::SendClientMessageCenter(i, message);
	}
}

void IW4::MISC::SendAllClientsMessageBold(std::string message)
{
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		if (ClientManager::GetClientById(i)->IsValid)
			IW4::MISC::SendClientMessageBold(i, message);
	}
}

void IW4::MISC::SendAllClientsChatMessage(int from_id, std::string message)
{
	auto entity = EntityManager::GetGEntity(from_id);
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		IW4::G::Say(entity, EntityManager::GetGEntity(i), 0, message.c_str());
	}
}

void IW4::MISC::SendClientSound(int clientid, std::string sound)
{
	IW4::SV::GameSendServerCommand(clientid, IW4::SV::CMD_RELIABLE, ("o \"" + sound + "\"").c_str());
}

void IW4::MISC::SetClientDvar(unsigned int clientid, std::string dvar, std::string value)
{
	IW4::SV::GameSendServerCommand(clientid, IW4::SV::CMD_RELIABLE, ("s " + dvar + " \"" + value + "\"").c_str());
}

float* IW4::MISC::GetTraceFromEntView(gentity_t *from)
{
	vec3_t origin;
	vec3_t forward;
	vec3_t right;
	vec3_t up;

	IW4::MATH::AngleVectors(from->Angle, forward, right, up);
	IW4::G::GetPlayerViewOrigin(from->playerstate_s, origin);

	vec3_t goal;
	goal[0] = origin[0] + (forward[0] * 900000);
	goal[1] = origin[1] + (forward[1] * 900000);
	goal[2] = origin[2] + (forward[2] * 900000);

	auto trace = new trace_t;
	IW4::G::LocationalTrace(trace, origin, goal, from->EntityNumber, 0x806831, nullptr);

	auto hitlocation = new float[3]();
	for (auto i = 0; i < 3; i++)
	{
		hitlocation[i] = ((goal[i] - origin[i]) * trace->fraction) + origin[i];
	}

	return hitlocation;
}

void IW4::DVAR::PrintDvar(dvar_t * dvar)
{
	printf("===========DVAR=DEBUG===========\n");
	printf("DvarName: %s\n", dvar->name);

	switch (dvar->type)
	{
	case DVAR_TYPE_BOOL:
		printf("Type: BOOL\n");
		printf("Values:\n - %s\n - %s\n - %s\n", dvar->current.enabled ? "true" : "false", dvar->latched.enabled ? "true" : "false", dvar->reset.enabled  ? "true" : "false");
		printf("Values:\n - %g\n - %g\n - %g\n", dvar->current.value, dvar->latched.value, dvar->reset.value);
		break;
	case DVAR_TYPE_FLOAT:
		printf("Type: FLOAT\n");
		printf("Values:\n - %g\n - %g\n - %g\n", dvar->current.value, dvar->latched.value, dvar->reset.value);
		break;
	case DVAR_TYPE_INT:
		printf("Type: INT\n");
		printf("Values:\n - %d\n - %d\n - %d\n", dvar->current.integer, dvar->latched.integer, dvar->reset.integer);
		break;
	case DVAR_TYPE_STRING:
		printf("Type: STRING\n");
		printf("Values:\n - %s\n - %s\n - %s\n", dvar->current.string, dvar->latched.string, dvar->reset.string);
		break;
	default:
		printf("Dvar is not a BOOL, FLOAT, INT, STRING but is %d\n", dvar->type);
		break;
	}
	printf("================================\n");
}
