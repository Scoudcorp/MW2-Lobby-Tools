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

#include <map>
#include "WeaponConfig.h"

#define CONFIG_FILE "%appdata%/mw2lt.cfg"

class ConfigManager
{
public:
	static std::vector<std::string> weaponlist;

	static std::vector<std::string> launchers;
	static std::vector<std::string>	handguns;
	static std::vector<std::string>	machinepistols;
	static std::vector<std::string>	submachineguns;
	static std::vector<std::string>	assaultrifles;
	static std::vector<std::string>	sniperrifles;
	static std::vector<std::string>	shotguns;
	static std::vector<std::string>	lightmachineguns;
	static std::vector<std::string> attachments;

	static void InitWeaponList();

	static WEAPON_STATE ParseWeapon(std::string weapon);

	static void AddWeapon(std::string weapon, WEAPON_STATE state);
	static WeaponConfig *GetWeapon(std::string weapon);

	static WEAPON_STATE GetWeaponState(std::string weapon);
	static void SetWeaponState(std::string weapon, WEAPON_STATE state);

	static bool IsWeaponAllowed(std::string weapon);
	static bool IsWeaponStrippable(std::string weapon);
	static bool IsWeaponReplaceable(std::string weapon);
	static bool IsWeaponKillable(std::string weapon);

	static WEAPON_STATE GetAttachmentStateForWeapon(std::string weapon, std::string attachment);
	static void SetAttachmentStateForWeapon(std::string weapon, std::string attachment, WEAPON_STATE state);
	
	static void SetGlobalAttachmentState(std::string attachment, WEAPON_STATE state);

	static nlohmann::json GetWeaponDump();
	static void ParseWeaponDump(nlohmann::json weapons);

	static std::string ConfigManager::GetConfigPath();
	static void LoadConfig();
	static void SaveConfig();
	static void CreateDefaultConfig();

private:
	static std::map<std::string, WeaponConfig*> weapons;
};
