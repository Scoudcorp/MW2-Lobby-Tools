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
#include "ConfigManager.h"
#include <fstream>
#include <Shlobj.h>
#include <locale>
#include "json.h"

using json = nlohmann::json;

std::map<std::string, WeaponConfig*> ConfigManager::weapons;
std::vector<std::string> ConfigManager::weaponlist
({
	"m79", "rpg", "at4", "stinger", "javelin", "riotshield",
	"beretta", "usp", "coltanaconda", "deserteagle", "deserteaglegold",
	"pp2000", "tmp", "glock", "beretta393",
	"mp5k", "uzi", "p90", "kriss", "ump45",
	"ak47", "m16",  "m4", "fn2000", "masada",  "famas",	"fal", "scar", "tavor",
	"barrett", "wa2000", "m21", "cheytac",
	"ranger", "model1887", "striker", "aa12", "m1014", "spas12",
	"rpd", "sa80", "mg4", "m240", "aug"
});
std::vector<std::string> ConfigManager::launchers({ "m79", "rpg", "at4", "stinger", "javelin", "riotshield" });
std::vector<std::string> ConfigManager::handguns({ "beretta", "usp", "coltanaconda", "deserteagle", "deserteaglegold" });
std::vector<std::string> ConfigManager::machinepistols({ "pp2000", "tmp", "glock", "beretta393" });
std::vector<std::string> ConfigManager::submachineguns({ "mp5k", "uzi", "p90", "kriss", "ump45" });
std::vector<std::string> ConfigManager::assaultrifles({ "ak47", "m16",  "m4", "fn2000", "masada",  "famas",	"fal", "scar", "tavor" });
std::vector<std::string> ConfigManager::sniperrifles({ "barrett", "wa2000", "m21", "cheytac" });
std::vector<std::string> ConfigManager::shotguns({ "ranger", "model1887", "striker", "aa12", "m1014", "spas12" });
std::vector<std::string> ConfigManager::lightmachineguns({ "rpd", "sa80", "mg4", "m240", "aug" });
std::vector<std::string> ConfigManager::attachments({ "acog", "akimbo", "eotech", "fmj", "gl", "grip", "heartbeat", "reflex", "rof", "shotgun", "silencer", "tactical", "thermal", "xmags" });

void ConfigManager::InitWeaponList()
{
	for (auto weapon : ConfigManager::weaponlist)
	{
		ConfigManager::AddWeapon(weapon, WEAPON_DEFAULT);
	}
}

WEAPON_STATE ConfigManager::ParseWeapon(std::string weapon)
{
	std::string delim = "_";
	std::vector<std::string> parts;

	auto start = 0U;
	auto end = weapon.find(delim);
	while (end != std::string::npos)
	{
		parts.push_back(std::string(weapon.substr(start, end - start)));
		start = end + delim.length();
		end = weapon.find(delim, start);
	}

	for (unsigned int i = 0; i < parts.size(); i++)
	{
		auto part = parts.at(i);

		if (i == 0)
		{
			if (ConfigManager::IsWeaponAllowed(part))
				continue;
			else
				return ConfigManager::GetWeaponState(part);
		}
		else
		{
			if (ConfigManager::GetAttachmentStateForWeapon(parts.at(0), part) == WEAPON_DEFAULT)
				continue;
			else
				return ConfigManager::GetAttachmentStateForWeapon(parts.at(0), part);
		}

	}

	return WEAPON_INVALID;
}

void ConfigManager::AddWeapon(std::string weapon, WEAPON_STATE state)
{
	ConfigManager::weapons[weapon] = new WeaponConfig(state);
}

WeaponConfig * ConfigManager::GetWeapon(std::string weapon)
{
	auto elem = ConfigManager::weapons.find(weapon);
	if (elem != ConfigManager::weapons.end())
		return elem->second;
	else
		return nullptr;
}

WEAPON_STATE ConfigManager::GetWeaponState(std::string weapon)
{
	auto elem = ConfigManager::GetWeapon(weapon);
	if (elem != nullptr)
		return elem->state;
	else
		return WEAPON_INVALID;
}

void ConfigManager::SetWeaponState(std::string weapon, WEAPON_STATE state)
{
	auto elem = ConfigManager::GetWeapon(weapon);

	if (elem != nullptr)
		elem->SetState(state);
	else
		Logger::LogWarning("SetWeaponState: invalid weapon " + weapon);
}

bool ConfigManager::IsWeaponAllowed(std::string weapon)
{
	return ConfigManager::GetWeaponState(weapon) == WEAPON_DEFAULT;
}

bool ConfigManager::IsWeaponStrippable(std::string weapon)
{
	return ConfigManager::GetWeaponState(weapon) == WEAPON_STRIP;
}

bool ConfigManager::IsWeaponReplaceable(std::string weapon)
{
	return ConfigManager::GetWeaponState(weapon) == WEAPON_REPLACE;
}

bool ConfigManager::IsWeaponKillable(std::string weapon)
{
	return ConfigManager::GetWeaponState(weapon) == WEAPON_KILL;
}

WEAPON_STATE ConfigManager::GetAttachmentStateForWeapon(std::string weapon, std::string attachment)
{
	auto elem = ConfigManager::GetWeapon(weapon);
	if (elem != nullptr)
		return elem->attachments[attachment];
	else
		return WEAPON_INVALID;
}

void ConfigManager::SetAttachmentStateForWeapon(std::string weapon, std::string attachment, WEAPON_STATE state)
{
	auto elem = ConfigManager::GetWeapon(weapon);
	if (elem != nullptr)
		elem->attachments[attachment] = state;
	else
		Logger::LogWarning("SetAttachmentStateForWeapon: invalid weapon " + weapon);
}

void ConfigManager::SetGlobalAttachmentState(std::string attachment, WEAPON_STATE state)
{
	for (const auto &weapon : ConfigManager::weapons)
	{
		weapon.second->SetAttachmentState(attachment, state);
	}
}

nlohmann::json ConfigManager::GetWeaponDump()
{
	json weapons;
	for (auto wep : ConfigManager::weapons)
	{
		json attachments;
		for (auto att : wep.second->attachments)
			attachments[att.first] = att.second;

		weapons[wep.first] = {
			{ "state", wep.second->state },
			{ "attachments", attachments }
		};
	}

	return weapons;
}

void ConfigManager::ParseWeaponDump(nlohmann::json weapons)
{
	for (auto wep = weapons.begin(); wep != weapons.end(); ++wep)
	{
		auto weaponname = wep.key();
		ConfigManager::SetWeaponState(weaponname, WEAPON_STATE(wep.value()["state"].get<int>()));

		auto attachments = wep.value()["attachments"];
		for (auto att = attachments.begin(); att != attachments.end(); ++att)
			ConfigManager::SetAttachmentStateForWeapon(weaponname, att.key(), WEAPON_STATE(att.value().get<int>()));
	}
}

std::string ConfigManager::GetConfigPath()
{
	WCHAR szPath[MAX_PATH];

	std::string path = "INVALID_PATH";
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA | CSIDL_FLAG_CREATE, NULL, 0, szPath)))
	{
		//convert from wide char to narrow char array
		char ch[MAX_PATH];
		auto DefChar = ' ';
		WideCharToMultiByte(CP_ACP, 0, szPath, -1, ch, 260, &DefChar, nullptr);
		path = std::string(ch) + "\\mw2lt.cfg";
	}

	return path;
}

void ConfigManager::LoadConfig()
{
	auto path = ConfigManager::GetConfigPath();
	if (path == "INVALID_PATH")
	{
		Logger::LogError("Invalid path, RIP ");
		return;
	}

	std::ifstream configfile(path);
	if (!configfile.is_open())
	{
		Logger::LogWarning("Unable to read " + path + ", trying to create a new one.");
		ConfigManager::CreateDefaultConfig();
	}
	else
	{
		std::string line;
		std::getline(configfile, line);
		configfile.close();

		json config;
		try
		{
			 config = json::parse(line);
		}
		catch (const std::exception&)
		{
			Logger::LogError("Unable to parse config file, creating default");
			ConfigManager::CreateDefaultConfig();
		}

		/* set all configs */
		ConfigManager::ParseWeaponDump(config["weapons"]);
		ClientManager::ParseBanDump(config["bans"]);
				
		Logger::Log("Loaded config " + path);
	}
}

void ConfigManager::SaveConfig()
{
	auto path = ConfigManager::GetConfigPath();
	if (path == "INVALID_PATH")
	{
		Logger::LogError("Invalid path, RIP");
		return;
	}

	std::ofstream configfile(path, std::ofstream::out | std::ofstream::trunc);
	if (!configfile.is_open())
	{
		Logger::LogError("Unable to write " + path);
		//exit(-1);
	}

	json config = {
		{ "admins", json::array() },
		{ "bans", ClientManager::GetBanDump() },
		{ "punished", json::array() },
		{ "weapons", ConfigManager::GetWeaponDump() },
	};

	configfile << config.dump();
	configfile.close();

	Logger::Log("Saved config " + path);
}

void ConfigManager::CreateDefaultConfig()
{
	auto path = ConfigManager::GetConfigPath();
	if (path == "INVALID_PATH")
	{
		Logger::LogError("Invalid path, RIP");
		return;
	}

	std::ofstream configfile(path, std::ofstream::out | std::ofstream::trunc);
	if (!configfile.is_open())
	{
		Logger::LogError("Unable to create " + path);
		//exit(-1);
	}

	/* Set the default disables */
	ConfigManager::SetGlobalAttachmentState("gl", WEAPON_REPLACE);
	ConfigManager::SetWeaponState("rpg", WEAPON_REPLACE);
	ConfigManager::SetWeaponState("at4", WEAPON_REPLACE);
	ConfigManager::SetWeaponState("m79", WEAPON_REPLACE);
	ConfigManager::SetAttachmentStateForWeapon("glock", "akimbo", WEAPON_REPLACE);

	json defaultconfig = {
		{ "admins", json::array() },
		{ "bans", ClientManager::GetBanDump() },
		{ "punished", json::array() },
		{ "weapons", ConfigManager::GetWeaponDump() },
	};

	configfile << defaultconfig.dump();
	configfile.close();

	Logger::Log("Created default config " + path);
}
