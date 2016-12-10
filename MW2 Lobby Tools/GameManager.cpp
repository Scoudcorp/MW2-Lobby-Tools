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
#include "GameManager.h"

#include <future>
#include "MemoryManager.h"
#include "EntityManager.h"
#include "ClientManager.h"

bool GameManager::ConsoleOpen = false;
bool GameManager::ForceHost = false;
bool GameManager::PrivateMatch = false;
bool GameManager::MaxPlayers = false;
bool GameManager::FullBright = false;
bool GameManager::FieldOfView = true;
bool GameManager::InfiniteAmmo = false;
MenuEvent GameManager::WeaponMode = WEAPONMODE_DEFAULT;

CurrentGameManager *GameManager::CurrentGame;

void GameManager::SetCurrentGameManager(CurrentGameManager * CurrentGame)
{
	GameManager::CurrentGame = CurrentGame;
}

void GameManager::SetTeamNameAllies(std::string name)
{
	IW4::DVAR::SetStringByName("g_teamname_allies", name.c_str());
}

void GameManager::SetTeamNameAxis(std::string name)
{
	IW4::DVAR::SetStringByName("g_teamname_axis", name.c_str());
}

void GameManager::EndGame()
{
	//IW4::UI::OpenMenu(0, "popup_endgame");
	//IW4::UI::OpenMenu(0, "endgame");
	//IW4::CBUF::AddText(0, "openmenu popup_endgame\n");
}

void GameManager::PauseGame()
{
	// TODO: implement
}

void GameManager::OpenIW4Console()
{
	if (!ConsoleOpen)
	{
		DoBeep(ON);
		ConsoleOpen = true;
		IW4::UNKN::CreateConsole();
	}
	else
	{
		DoBeep(ERR);
	}
}

void GameManager::OpenGameSetup()
{
	DoBeep(ON);
	IW4::UI::OpenMenu(0, "popup_gamesetup");
}

void GameManager::GameForceStart()
{
	DoBeep(ON);

	auto party_teamBased = IW4::DVAR::FindDvar("party_teambased");
	auto xblive_privatematch = IW4::DVAR::FindDvar("xblive_privatematch");
	std::string gamemode(IW4::DVAR::GetString("ui_gametype"));
	IW4::DVAR::SetBoolByName("party_teambased", gamemode != "dm");

	xblive_privatematch->current.enabled = true;

	IW4::PARTYHOST::BalanceTeams(reinterpret_cast<void*>(G_LOBBYDATA));
	IW4::PARTYHOST::BalanceTeams(reinterpret_cast<void*>(PARTYSESSION_P));
	IW4::PARTYHOST::StartMatch(reinterpret_cast<void*>(G_LOBBYDATA), 0);
	//((void(__cdecl*) ())0x4C5170)();

	xblive_privatematch->current.enabled = false;
}

void GameManager::ToggleForceHost()
{
	// FIXME: find a better way, unable to toggle forcehost off
	if (ForceHost)
	{
		DoBeep(OFF);
		IW4::CMD::ExecuteSingleCommand(0, 0, "party_hostmigration 1");
		IW4::CMD::ExecuteSingleCommand(0, 0, "party_connectTimeout 0");
		IW4::DVAR::SetFromStringByName("party_minplayers", "1");
		IW4::DVAR::SetBoolByName("requireOpenNat", false);
	}
	else
	{
		DoBeep(ON);
		IW4::CMD::ExecuteSingleCommand(0, 0, "party_hostmigration 0");
		IW4::CMD::ExecuteSingleCommand(0, 0, "party_connectTimeout 1");
		IW4::DVAR::SetFromStringByName("party_minplayers", "2");
		IW4::DVAR::SetBoolByName("requireOpenNat", true);
	}

	ForceHost = !ForceHost;
}

void GameManager::TogglePrivateMatch()
{
	if (PrivateMatch)
	{
		DoBeep(OFF);
		IW4::CMD::ExecuteSingleCommand(0, 0, "xblive_privatematch 0");
		IW4::CMD::ExecuteSingleCommand(0, 0, "ui_allow_teamchange 0");
	}
	else
	{
		DoBeep(ON);
		IW4::CMD::ExecuteSingleCommand(0, 0, "xblive_privatematch 1");
		IW4::CMD::ExecuteSingleCommand(0, 0, "ui_allow_teamchange 1");
	}

	PrivateMatch = !PrivateMatch;
}

void GameManager::ToggleMaxPlayers()
{

	if (MaxPlayers)
	{
		DoBeep(OFF);
		MemoryManager::WriteValueToAddress(MAXPLAYERS_a, 12);
	}
	else
	{
		DoBeep(ON);
		MemoryManager::WriteValueToAddress(MAXPLAYERS_a, 18);
	}

	PrivateMatch = !PrivateMatch;
}

void GameManager::ToggleFullBright()
{
	GameManager::FullBright = !GameManager::FullBright;
	IW4::MISC::SendClientMessageCenter(IW4::MISC::GetHostId(), "FullBright is " + std::string((GameManager::FullBright ? "^2ON^7!" : "^1OFF^7!")));
}

void GameManager::ToggleFieldOfView()
{
	GameManager::FieldOfView = !GameManager::FieldOfView;
	IW4::MISC::SendClientMessageCenter(IW4::MISC::GetHostId(), "FoV is " + std::string((GameManager::FullBright ? "^280^7!" : "^165^7!")));
}

void GameManager::ToggleInfiniteAmmo()
{
	GameManager::InfiniteAmmo = !GameManager::InfiniteAmmo;
	IW4::MISC::SendClientMessageCenter(IW4::MISC::GetHostId(), "Infinite Ammo is " + std::string((GameManager::InfiniteAmmo ? "^2ON^7!" : "^1OFF^7!")));
}

void GameManager::SetWeaponMode(MenuEvent mevent)
{
	GameManager::WeaponMode = mevent;
}

void GameManager::SendChatMessage(std::string Message)
{
}

void GameManager::SendClientChatMessage(int from, int to, std::string message)
{
	auto froment = EntityManager::GetGEntity(from);
	auto targetent = EntityManager::GetGEntity(to);
	
	IW4::G::Say(froment, targetent, 0, message.c_str());
}

void GameManager::SendKillstreakMessage(int killstreak, int id)
{
	std::string clientname(ClientManager::GetClientById(id)->szName);
	std::string message;
	std::string sound;

	switch (killstreak)
	{
		case 5:
			message = clientname + " is on a ^2killing spree^7!";
			break;
		case 10:
			message = clientname + " is on a ^2killing frenzy^7!";
			break;
		case 15:
			message = clientname + " is on a ^2running riot^7!";
			break;
		case 20:
			message = clientname + " is on a ^2rampage^7!";
			break;
		case 25:
			message = clientname + " is ^2untouchable^7!";
			break;
		case 30:
			message = clientname + " is ^2Invinsible^7!";
			break;
	}

	if (!message.empty())
		IW4::MISC::SendAllClientsMessageCenter(message);
	
	// TODO: implement sounds
	//if (!sound.empty())
	//	IW4::SND::PlayClientSound(0, id, const_cast<char*>(sound.c_str()));
}

void GameManager::SendDeathstreakMessage(int deathstreak, int id)
{
	std::string clientname(ClientManager::GetClientById(id)->szName);
	std::string message;

	switch (deathstreak)
	{
	case 5:
		message = clientname + " is on a ^25 death streak^7!";
		break;
	case 10:
		message = clientname + " is on a ^210 death streak^7!";
		break;
	case 15:
		message = clientname + " is on a ^215 death streak^7!";
		break;
	case 20:
		message = clientname + " is on a ^220 death streak^7!";
		break;
	case 25:
		message = clientname + " is ^225 death streak^7!";
		break;
	case 30:
		message = clientname + " is ^230 death streak^7!";
		break;
	}

	if (!message.empty())
		IW4::MISC::SendAllClientsMessageCenter(message);
}

void GameManager::DoBeep(BeepState state)
{
	switch (state)
	{
	case ON:
		Beep(1000, 50);
		break;
	case OFF:
		Beep(500, 50);
		break;
	default:
		Beep(200, 50);
		break;
	}
}
