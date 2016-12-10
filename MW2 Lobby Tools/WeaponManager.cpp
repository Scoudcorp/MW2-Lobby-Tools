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

#include "stdafx.h"
#include "WeaponManager.h"
#include "EntityManager.h"
#include "PlayerStateManager.h"

void WeaponManager::GiveWeapon(int id, int weaponid, int camo, int ammo, bool akimbo, bool switchto)
{
	auto playerstate = PlayerstateManager::GetPlayerState(id);
	auto gentity = EntityManager::GetGEntity(id);

	IW4::G::GivePlayerWeapon(playerstate, weaponid, camo, akimbo ? 1 : 0);
	IW4::MISC::Add_Ammo(gentity, weaponid, 0, ammo, 1);

	if (switchto)
		SelectWeapon(id, weaponid);
}

void WeaponManager::GiveWeapon(int id, char * weapon, int camo, int ammo, bool akimbo, bool switchto)
{
	auto weaponid = IW4::G::GetWeaponIndexForName(weapon);
	GiveWeapon(id, weaponid, camo, ammo, akimbo, switchto);
}

void WeaponManager::RemoveWeapon(int id, char * weapon)
{
	auto playerstate = reinterpret_cast<int*>(IW4::SV::GameClientNum(id));
	auto weaponid = IW4::G::GetWeaponIndexForName(weapon);
	IW4::BG::TakePlayerWeapon(playerstate, weaponid, 1);
}

void WeaponManager::StripWeapons(int id)
{
	// We can use this playercmd function since it does not modify the VM stack
	IW4::PLAYERCMD::takeAllWeapons(id);

	// ===== PlayerCmd_takeAllWeapons(scr_entref_t) reversing =====
	// Current problems: seems to only take some weapons,
	// takes wrong weapons when "if (something <= 0)",
	// crashes when "something < 0" since something is used as array index
	/*
	auto entity = EntityManager::GetGEntity(id);
	auto playerstate = IW4::SV::GameClientNum(id);

	*(DWORD *)(*((DWORD *)entity + 0x56) + 0x2B4) = 0;

	int i = 0x220;
	do
	{
		auto something = *(((char*)playerstate) + i);

		if (something == 0)
		{
			printf("Something is 0 for i=%d\n", i);
			goto next; // item is nothing?
		}

		auto result = ((int(__cdecl*)(int))0x4794D0)(something); // call the unknown function
		printf("Requesting something with %d, got %d\n", something, result);

		auto something_else = *((DWORD*)(result + 0x38));
		if (something_else == 3)
		{
			printf("Something_else is 3 for i=%d\n", i);
			goto next; // item is not a weapon?
		}

		auto weapon_id = (unsigned int)(something);
		auto weapon_name = static_cast<char*>(IW4::BG::GetWeaponDef(weapon_id));
		printf("Removing weapon %s\n", weapon_name);

		IW4::BG::TakePlayerWeapon(playerstate, (unsigned int)(something), 1);

	next:
		i += 0x04;
	} while (i < 0x25C);
	*/


	// ===== Version from hexrays =====
	/*
	char *v2; // esi@2
	signed int v3; // ebx@5
	int result; // eax@6
	int v5; // edi@6

	if (a2 >> 16)
	{
		sub_57F2B0(a1);
		v2 = 0;
	}
	else
	{
		v2 = (char *)&unk_194B950 + 628 * (unsigned __int16)a2;
		if (!*((_DWORD *)v2 + 86))
			sub_57F2B0(a1);
	}
	*(_DWORD *)(*((_DWORD *)v2 + 86) + 692) = 0;
	v3 = 544;
	do
	{
		result = *((_DWORD *)v2 + 86);
		v5 = *(_DWORD *)(v3 + result);
		if (v5)
		{
			result = sub_4794D0(*(_DWORD *)(v3 + result));
			if (*(_DWORD *)(result + 56) != 3)
				result = sub_550C20(*((_DWORD *)v2 + 86), v5);
		}
		v3 += 4;
	} while ((unsigned int)v3 < 0x25C);
	return result;
	*/

	// ===== Old first try at reversing =====
	/*
	for (auto i = 0x220; i < 0x25C; i += 0x04)
	{
		auto something = (char*)(((char*)playerstate) + i);
		
		if (*something == 0)
		{
			printf("Something is 0 for i=%d\n", i);
			continue; // item is nothing?
		}

		printf("Playerstate @ %p, Playerstate+%d @ %p\n", playerstate, i, something);

		((void*(__cdecl*)(int))0x4794D0)(*something); // call the unknown function FAK

		auto something_else = (char*)(((char*)playerstate) + 0x38); 
		if (*something_else == 3)
		{
			printf("Something_else is 3 for i=%d\n", i);
			continue; // item is not a weapon?
		}
		IW4::BG::TakePlayerWeapon(playerstate, (unsigned int)(*something), 1);
	}
	*/
}

void WeaponManager::SelectWeapon(int id, int weaponid)
{
	IW4::G::SelectWeaponIndex(id, weaponid);
}

void WeaponManager::SelectWeapon(int id, char * weapon)
{
	auto weaponid = IW4::G::GetWeaponIndexForName(weapon);
	SelectWeapon(id, weaponid);
}

void WeaponManager::SwitchWeapon(int id, int times)
{
	for (auto i = 0; i < times; i++)
	{
		IW4::SV::GameSendServerCommand(id, IW4::SV::CMD_RELIABLE, "a 3216 1");
	}
}

std::string WeaponManager::TranslateWeaponName(std::string weapon)
{
	std::string result = "";

	if (weapon == "m79") result = "Thumper";
	else if (weapon == "rpg") result = "RPG-7";
	else if (weapon == "at4") result = "AT4-HS";
	else if (weapon == "stinger") result = "Stinger";
	else if (weapon == "javelin") result = "Javelin";
	else if (weapon == "riotshield") result = "Riot Shield";
	else if (weapon == "beretta") result = "M9";
	else if (weapon == "usp") result = "USP .45";
	else if (weapon == "coltanaconda") result = ".44 Magnum";
	else if (weapon == "deserteagle") result = "Desert Eagle";
	else if (weapon == "deserteaglegold") result = "Desert Eagle Gold";
	else if (weapon == "pp2000") result = "PP2000";
	else if (weapon == "tmp") result = "TMP";
	else if (weapon == "glock") result = "G18";
	else if (weapon == "beretta393") result = "M93 Raffica";
	else if (weapon == "mp5k") result = "MP5K";
	else if (weapon == "uzi") result = "Mini-Uzi";
	else if (weapon == "p90") result = "P90";
	else if (weapon == "kriss") result = "Vector";
	else if (weapon == "ump45") result = "UMP45";
	else if (weapon == "ak47") result = "AK-47";
	else if (weapon == "m16") result = "M16A4";
	else if (weapon == "m4") result = "M4A1";
	else if (weapon == "fn2000") result = "F2000";
	else if (weapon == "masada") result = "ACR";
	else if (weapon == "famas") result = "FAMAS";
	else if (weapon == "fal") result = "FAL";
	else if (weapon == "scar") result = "SCAR-H";
	else if (weapon == "tavor") result = "TAR-21";
	else if (weapon == "barrett") result = "Barett .50cal";
	else if (weapon == "wa2000") result = "WA2000";
	else if (weapon == "m21") result = "M21 EBR";
	else if (weapon == "cheytac") result = "Intervention";
	else if (weapon == "ranger") result = "Ranger";
	else if (weapon == "model1887") result = "Model 1887";
	else if (weapon == "striker") result = "Striker";
	else if (weapon == "aa12") result = "AA-12";
	else if (weapon == "m1014") result = "M1014";
	else if (weapon == "spas12") result = "SPAS-12";
	else if (weapon == "rpd") result = "RPD";
	else if (weapon == "sa80") result = "L86 LSW";
	else if (weapon == "mg4") result = "MG4";
	else if (weapon == "m240") result = "M240";
	else if (weapon == "aug") result = "AUG HBAR";
	else result = "UNDEFINED";

	return result;
}

std::string WeaponManager::TranslateAttachmentName(std::string attachment)
{
	std::string result = "";

	if (attachment == "acog") result = "ACOG scope";
	else if (attachment == "akimbo") result = "Akimbo";
	else if (attachment == "eotech") result = "Holographic Sight";
	else if (attachment == "fmj") result = "FMJ";
	else if (attachment == "gl") result = "Grenade Launcher";
	else if (attachment == "grip") result = "Grip";
	else if (attachment == "heartbeat") result = "Heartbeat Sensor";
	else if (attachment == "reflex") result = "Red Dot Sight";
	else if (attachment == "rof") result = "Rapid Fire";
	else if (attachment == "shotgun") result = "Shotgun";
	else if (attachment == "silencer") result = "Silencer";
	else if (attachment == "tactical") result = "Tactical Knife";
	else if (attachment == "thermal") result = "Thermal";
	else if (attachment == "xmags") result = "Extended Mags";
	else result = "UNDEFINED";

	return result;
}
