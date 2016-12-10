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
#include "CurrentGameManager.h"

#include "GameManager.h"
#include "WeaponManager.h"
#include "CustomEventManager.h"
#include "PunishmentManager.h"
#include "PerkManager.h"
#include <cassert>

#define VALIDID(n)	((n >= 0) && (n < MAX_CLIENTS))

CurrentGameManager::CurrentGameManager()
{
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		this->clients[i] = nullptr;
	}
}

CurrentGameManager::~CurrentGameManager()
{
}

void CurrentGameManager::HandleGamestate()
{
	if (this->gameactive && !this->cl_ingame->current.enabled)
	{
		this->EndGame();
	}
	else if (!this->gameactive && this->cl_ingame->current.enabled)
	{
		this->StartGame();
	}
}

void CurrentGameManager::GameTick()
{
	if (!this->gameactive) return;
		
	tpmanager->CheckAllTeleporters();

	auto current = std::clock();
	auto duration = (current - this->last_time) / static_cast<double>(CLOCKS_PER_SEC);
	if (duration > this->interval_time)
	{
		this->Advertise();
		this->last_time = current;
	}
}

void CurrentGameManager::Advertise() const
{
	IW4::MISC::SendAllClientsChatMessage(IW4::MISC::GetHostId(), "^7Hosted with ^2MW2 Lobby Tools " MW2LTVERSION "^7 by ^2Scoud");
	IW4::MISC::SendAllClientsChatMessage(IW4::MISC::GetHostId(), "^7Download @ ^2scoudem.github.io^7");
	IW4::MISC::SendAllClientsChatMessage(IW4::MISC::GetHostId(), "^7Type '^2/help^7' to view available commands");
}

void CurrentGameManager::StartGame()
{
	Logger::Log("Game has started");

	this->gameactive = true;

	GameManager::SetTeamNameAllies("^2MW2LT^7 Allies");
	GameManager::SetTeamNameAxis("^2MW2LT^7 Axis");

	// mp_afghan
	//tpmanager->RemoveAllTeleporters();
	//auto ui_mapname = IW4::DVAR::GetString("ui_mapname");	
	//if (strcmp(ui_mapname, "mp_afghan") == 0)
	//{
	//	vec3_t from = { 4203, 127, 154 }; // b flag dom
	//	vec3_t to = { 7222, 1541, -74 }; // outside killradius
	//	tpmanager->SetupTeleporterPair(from, to);
	//}
}

void CurrentGameManager::EndGame()
{
	Logger::Log("Game has finished");

	this->gameactive = false;

	for (auto id = 0; id < MAX_CLIENTS; id++)
	{
		if (this->clients[id] != nullptr)
		{
			delete this->clients[id];
			this->clients[id] = nullptr;
		}
	}

	tpmanager->RemoveAllTeleporters();

	ConfigManager::SaveConfig();
}

void CurrentGameManager::ClientConnect(int id) const
{
	//if (!gameactive) return;

	if (VALIDID(id))
	{
		if (this->clients[id] == nullptr)
		{
			printf("Client %d connected with xuid %lld\n", id, IW4::MISC::GetXuid(id));
			this->clients[id] = new Client(id);
		}

		IW4::MISC::SetClientDvar(id, "cl_maxpackets", "100");
		IW4::MISC::SetClientDvar(id, "snaps", "30");
		IW4::MISC::SetClientDvar(id, "cg_chatHeight", "8");
		IW4::MISC::SetClientDvar(id, "cg_hudChatPosition", "5 204");
		IW4::MISC::SetClientDvar(id, "cg_hudChatIntermissionPosition", "5 110");
		IW4::MISC::SetClientDvar(id, "cg_ScoresPing_MaxBars", "10");
		IW4::MISC::SetClientDvar(id, "cg_fov", "80");

		ClientManager::CheckAllClients();
	}
	//else printf("WARNING: invalid id %d for ClientConnect\n", id);
}

void CurrentGameManager::ClientDisconnect(int id) const
{
	if (!gameactive) return;

	if (VALIDID(id))
	{
		if (this->clients[id] != nullptr)
		{
			delete this->clients[id];
			this->clients[id] = nullptr;
		}
	}
	else printf("WARNING: invalid id %d for ClientDisconnect\n", id);

}

void CurrentGameManager::ClientSpawn(int id) const
{
	if (!gameactive) return;

	if (!VALIDID(id))
	{
		//printf("WARNING: invalid id %d for ClientSpawn\n", id);
		return;
	}

	auto *client = this->clients[id];
	if (client != nullptr)
	{
		ClientManager::CheckClient(id);

		if (!client->firstspawned)
		{
			auto xuid = IW4::MISC::GetXuid(id);
			if (ClientManager::IsSpecialClient(xuid))
				IW4::MISC::SendAllClientsMessageCenter(ClientManager::GetSpecialClient(xuid)->message);

			// TODO: generate a temp entity and use a custom name for chat messages (experiment).
			GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7This lobby is running ^2MW2 Lobby Tools ^7v" MW2LTVERSION "^7 (by ^2Scoud^7).");
			GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7This server is in ^180^7 fov mode.");
		}

		client->Spawn();

		this->ClientDvars(id);
		this->ClientWeaponChange(id);
		this->ClientLoadout(id);
		this->ClientPunish(id);

		//PerkManager::UnsetPerk(id, "specialty_pistoldeath"); // last stand
		//PerkManager::UnsetPerk(id, "specialty_combathigh"); // painkiller ?
		//PerkManager::UnsetPerk(id, "specialty_grenadepulldeath"); // martyrdom
		//PerkManager::UnsetPerk(id, "specialty_finalstand"); // finalstand
	}
	else printf("WARNING: client id %d is nullptr ClientSpawn\n", id);
}

void CurrentGameManager::ClientKill(int id) const
{
	if (!gameactive) return;

	auto client = this->clients[id];
	if (client)
	{
		if (client->deathstreak >= 10)
		{
			std::string clientname(ClientManager::GetClientById(id)->szName);
			IW4::MISC::SendAllClientsMessageCenter(clientname + "^7's deathstreak ended on " + std::to_string(client->killstreak) + " deaths!");
		}

		client->Kill();
		GameManager::SendKillstreakMessage(client->killstreak, id);
	}
	else printf("WARNING: invalid id %d for ClientKill\n", id);

}

void CurrentGameManager::ClientDeath(int id) const
{
	if (!gameactive) return;

	auto client = this->clients[id];
	if (client)
	{
		if (client->killstreak >= 5)
		{
			std::string clientname(ClientManager::GetClientById(id)->szName);
			IW4::MISC::SendAllClientsMessageCenter(clientname + "^7's streak ended on " + std::to_string(client->killstreak) + " kills!");
		}

		client->Death();
		GameManager::SendDeathstreakMessage(client->deathstreak, id);
	}
	else printf("WARNING: invalid id %d for ClientDeath\n", id);
}

bool CurrentGameManager::ClientAlive(int id) const
{
	if (!gameactive) return false;

	if (VALIDID(id))
	{
		auto client = this->clients[id];
		if (client)
		{
			return client->alive;
		}
		else
		{
			printf("WARNING: no client for id %d in ClientAlive\n", id);
			goto fail;
		}
	}
	else printf("WARNING: invalid id %d for ClientAlive\n", id);

fail:
	return false;
}

void CurrentGameManager::ClientWeaponChange(int id) const
{
	if (!gameactive) return;

	// TODO: check if switched weapon is the replacement weapon, if it is do nothing

	if (!this->ClientAlive(id)) return;

	auto weapon = static_cast<char*>(IW4::BG::GetWeaponDef(IW4::BG::GetViewmodelWeaponIndex(IW4::SV::GameClientNum(id))));
	if (strcmp(weapon, "deserteagle_mp") == 0)
	{
		WeaponManager::RemoveWeapon(id, weapon);
		WeaponManager::GiveWeapon(id, "deserteaglegold_mp", 0, 180, 0, true);
		GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7Oooh ^3*shiny*^7!");
	}

	if (ClientManager::IsSpecialClient(id))
	{
		if (strcmp(weapon, "deserteagle_tactical_mp") == 0)
		{
			WeaponManager::RemoveWeapon(id, weapon);
			WeaponManager::GiveWeapon(id, "ac130_25mm_mp", 0, 180, 0, true);
			GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7^1Brrrrrrrrrrrrt^7!");
		}
		return;
	}

	auto state = ConfigManager::ParseWeapon(weapon);
	switch (state)
	{
	case WEAPON_DEFAULT:
		break;
	case WEAPON_STRIP:
		IW4::SCR::AddString(weapon);
		IW4::MISC::SetNumParam(1);
		IW4::PLAYERCMD::takeWeapon(id);
		IW4::SCR::ClearOutParams();
		GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7your active weapon is ^1not^7 allowed and was ^2removed^7!");
		break;
	case WEAPON_REPLACE:
		WeaponManager::RemoveWeapon(id, weapon);
		WeaponManager::GiveWeapon(id, "fn2000_mp", 8, 30, 0, true);
		GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7your active weapon is ^1not^7 allowed and was ^2replaced^7!");
		break;
	case WEAPON_KILL:
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, 1, id, 0, static_cast<void*>(new punishment_wrapper(PUNISH_KILL)));
		GameManager::SendClientChatMessage(IW4::MISC::GetHostId(), id, "^7your active weapon is ^1not^7 allowed and you were ^2killed^7!");
		break;
	default:
		break;
	}
}

void CurrentGameManager::ClientDvars(int id) const
{
	if (!gameactive) return;
	/*if (GameManager::FieldOfView)
		IW4::SV::GameSendServerCommand(id, IW4::SV::CMD_RELIABLE, "s cg_fov \"80\"");
	else
		IW4::SV::GameSendServerCommand(id, IW4::SV::CMD_RELIABLE, "s cg_fov \"65\"");

	if (GameManager::FullBright)
		IW4::SV::GameSendServerCommand(id, IW4::SV::CMD_RELIABLE, "s r_fullbright \"1\"");
	else
		IW4::SV::GameSendServerCommand(id, IW4::SV::CMD_RELIABLE, "s r_fullbright \"0\"");
	IW4::MISC::SetClientDvar(id, "cg_fov", "80");*/
}

void CurrentGameManager::ClientLoadout(int id) const
{
	if (!gameactive) return;

	if (GameManager::WeaponMode == WEAPONMODE_DEFAULT) return;
	else if (GameManager::WeaponMode == WEAPONMODE_SNIPER)
	{
		WeaponManager::StripWeapons(id);
		WeaponManager::GiveWeapon(id, "cheytac_fmj_xmags_mp", 8, AMMO_MAX, false, true);
		WeaponManager::GiveWeapon(id, "coltanaconda_akimbo_fmj_mp", 0, AMMO_MAX, true, false);
	}
	else if (GameManager::WeaponMode == WEAPONMODE_AC130)
	{
		WeaponManager::StripWeapons(id);
		WeaponManager::GiveWeapon(id, "ac130_105mm_mp", 0, AMMO_MAX, false, true);
		WeaponManager::GiveWeapon(id, "ac130_40mm_mp", 0, AMMO_MAX, false, false);
		WeaponManager::GiveWeapon(id, "ac130_25mm_mp", 0, AMMO_MAX, false, false);
	}
}

void CurrentGameManager::ClientPunish(int id) const
{
	if (!gameactive) return;

	auto xuid = IW4::MISC::GetXuid(id);
	auto punishment = PunishmentManager::GetPunishment(xuid);

	if (punishment == nullptr || punishment->xuid == 0)
	{
		return;
	}

	if (PunishmentManager::IsUserPunished(punishment, PUNISH_KILL))
	{
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, 0, id, 0, static_cast<void*>(new punishment_wrapper(PUNISH_KILL)));
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_PLANT))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_SLOW))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_INVERSE))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_STRIP_WEAPON))
	{
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, 0, id, 0, static_cast<void*>(new punishment_wrapper(PUNISH_STRIP_WEAPON)));
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_STRIP_PERK))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_STRIP_ALL))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_BLIND))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_AMMO_MAG))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_AMMO_ALL))
	{
		// TODO: implement
	}
	if (PunishmentManager::IsUserPunished(punishment, PUNISH_CRAP))
	{
		// TODO: implement
	}
	
	return;
}
