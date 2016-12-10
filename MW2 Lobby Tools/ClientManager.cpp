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
#include "ClientManager.h"

clientinfo_t* ClientManager::ClientInfos[MAX_CLIENTS];
std::vector<specialclient_t*> ClientManager::specialclients;
std::map<long long, std::string> ClientManager::bannedclients;

void ClientManager::InitClientInfos()
{
	for (auto id = 0; id < MAX_CLIENTS; id++)
	{
		ClientInfos[id] = reinterpret_cast<clientinfo_t *>(static_cast<uintptr_t>(CLIENT_a) + (id * CLIENT_s));
	}
}

void ClientManager::InitSpecialClients()
{
	
}

void ClientManager::AddSpecialClient(long long xuid, std::string message)
{
	auto client = new specialclient_t();
	client->xuid = xuid;
	client->message = message;
	specialclients.push_back(client);
}

specialclient_t * ClientManager::GetSpecialClient(long long xuid)
{
	for (auto specialclient : specialclients)
	{
		if (specialclient->xuid == xuid)
			return specialclient;
	}

	return nullptr;
}

bool ClientManager::IsSpecialClient(int id)
{
	auto xuid = IW4::MISC::GetXuid(id);
	return IsSpecialClient(xuid);
}

bool ClientManager::IsSpecialClient(long long xuid)
{
	for (auto specialclient : specialclients)
	{
		if (specialclient->xuid == xuid)
			return true;
	}

	return false;
}

void ClientManager::CheckAllClients()
{
	for (auto id = 0; id < MAX_CLIENTS; id++)
		ClientManager::CheckClient(id);
}

void ClientManager::CheckClient(int id)
{
	if (GetClientById(id)->IsValid && IsBannedId(id))
	{
		printf("Client %s (%d) (%lld) is banned\n", ClientManager::GetClientById(id)->szName, id, IW4::MISC::GetXuid(id));
		IW4::SV::GameDropClient(id, "^2MW2LT^7: you have been ^1banned^7 from the server");
	}
}

clientinfo_t* ClientManager::GetClientById(int id)
{
	return ClientInfos[id];
}

clientinfo_t* ClientManager::GetClientByName(const char* name)
{
	for (auto id = 0; id < MAX_CLIENTS; id++)
	{
		if (strcmp(name, ClientInfos[id]->szName) == 0)
		{
			return ClientInfos[id];
		}
	}

	return nullptr;
}

void ClientManager::KickIdReason(int id, std::string reason)
{
	auto me = IW4::MISC::GetHostId();

	if (IsSpecialClient(id) || id == me)
	{
		IW4::MISC::SendClientMessageCenter(me, "^1Unable to kick client");
		return;
	}

	IW4::MISC::SendAllClientsMessageCenter(std::string(ClientManager::GetClientById(id)->szName) + " ^7has been ^1kicked^7 from the server!");
	IW4::SV::GameDropClient(id, ("^2MW2LT^7: you have been ^1kicked^7:^7 " + reason).c_str());
}

void ClientManager::BanIdReason(int id, std::string reason)
{
	auto me = IW4::MISC::GetHostId();

	if (IsSpecialClient(id) || id == me)
	{
		IW4::MISC::SendClientMessageCenter(me, "^1Unable to kick client");
		return;
	}

	auto xuid = IW4::MISC::GetXuid(id);
	auto name = std::string(ClientManager::GetClientById(id)->szName);
	if (!IsBannedXuid(xuid))
		bannedclients[xuid] = name;

	IW4::MISC::SendAllClientsMessageCenter(name + " ^7has been ^1banned^7 from the server!");
	IW4::SV::GameDropClient(id, ("^2MW2LT^7: you have been ^1banned^7: " + reason).c_str());
}

void ClientManager::KickId(int id)
{
	KickIdReason(id, "you were removed from the game");
}

void ClientManager::BanId(int id)
{
	BanIdReason(id, "you were permanently removed from the game");
}

bool ClientManager::IsBannedId(int id)
{
	return IsBannedXuid(IW4::MISC::GetXuid(id));
}

void ClientManager::KickXuid(long long xuid)
{
	for (auto id = 0; id < MAX_CLIENTS; id++)
	{
		if (IW4::MISC::GetXuid(id) == xuid)
		{
			KickId(id);
			return;
		}
	}
}

void ClientManager::BanXuid(long long xuid)
{
	for (auto id = 0; id < MAX_CLIENTS; id++)
	{
		if (IW4::MISC::GetXuid(id) == xuid)
		{
			BanId(id);
			return;
		}
	}
}

bool ClientManager::IsBannedXuid(long long xuid)
{
	auto result = bannedclients.find(xuid);
	return result != bannedclients.end();
}

void ClientManager::UnbanXuid(long long xuid)
{
	auto result = bannedclients.find(xuid);
	if (result == bannedclients.end())
	{
		Logger::LogWarning("Unable to unban xuid " + std::to_string(xuid));
		return;
	}

	bannedclients.erase(result);
}

nlohmann::json ClientManager::GetBanDump()
{
	nlohmann::json jmap;

	if (ClientManager::bannedclients.size() == 0)
	{
		jmap = nlohmann::json::array();
	}
	else
	{
		for (auto bc : ClientManager::bannedclients)
		{
			jmap[std::to_string(bc.first)] = bc.second;
		}
	}

	return jmap;
}

void ClientManager::ParseBanDump(nlohmann::json bans)
{
	for (auto bc = bans.begin(); bc != bans.end(); ++bc)
	{
		auto xuid = atoll(bc.key().c_str());
		auto lastname = bc.value().get<std::string>();
		bannedclients[xuid] = lastname;
	}
}
