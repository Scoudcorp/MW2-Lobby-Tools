#include "stdafx.h"
#include "ChatManager.h"
#include <sstream>
#include <iterator>
#include "EntityManager.h"
#include <regex>
#include "CustomEventManager.h"
#include "Punishment.h"
#include "WeaponManager.h"

#define checkallowed(ENTITY)\
if (!(ClientManager::IsSpecialClient(ENTITY->EntityNumber) || ENTITY->EntityNumber == IW4::MISC::GetHostId()))\
{\
	SendLine(ENTITY, "^2You have no permission to do that.");\
	return true;\
}

#define checkargs(NUMARGS, MINARGS, ERRORMESSAGE)\
if (NUMARGS < MINARGS) \
{ \
	SendLine(from, ERRORMESSAGE); \
	return true; \
}

bool ChatManager::HandleChatMessage(gentity_t* from, gentity_t* to, int mode, const char* chatText)
{
	auto tokens = SplitMessage(chatText);
	if (tokens.size() <= 0) return false;

	std::vector<std::string> originaltokens;

	for (auto i = 0u; i < tokens.size(); i++)
	{
		originaltokens.push_back(tokens[i]);
		tokens[i] = SanitizeString(tokens[i]);
	}

	if (!IsCommand(tokens.at(0))) return false;
	auto command = GetCommand(tokens.at(0));
	auto numargs = tokens.size() - 1;

	/* Commands for all clients */
	if (command == "help")
	{
		SendLine(from, "^2MW2 Lobby Tools^7 by ^2Scoud^7 - ^2commands^7");
		SendLine(from, "^2/help - /about - /host - /status - /fov - /fullbright - /killme - /location - /setdvar");

		if (ClientManager::IsSpecialClient(from->EntityNumber) || from->EntityNumber == IW4::MISC::GetHostId())
		{
			SendLine(from, "^2/kick - /ban - /teleport - /fetch - /goto - /center - /sayas - /punish - /give");
		}
	}
	else if (command == "about")
	{
		SendLine(from, "^7Hosted with ^2MW2 Lobby Tools^7 " MW2LTVERSION " by ^2Scoud^7.");
		SendLine(from, "^7You can download this ^2open source^7 tool @ ^2scoudem.github.io^7.");
	}
	else if (command == "host")
	{
		std::string host = ClientManager::GetClientById(IW4::MISC::GetHostId())->szName;
		SendLine(from, "^7The current ^2host^7 is: " + host + "^7");
	}
	else if (command == "status")
	{
		checkargs(numargs, 1, "^2Usage /status <1/2/3>.");

		auto value = GetInt(tokens[1]);
		if (value < 1 || value > 3)
		{
			SendLine(from, "^2Status should be ^11^2-^13^2.");
			return true;
		}

		for (auto i = 6 * (value - 1); i < 6 * value; i++)
		{
			auto client = ClientManager::GetClientById(i);
			if (client->IsValid)
				SendLine(from, "^7Client ^2" + std::to_string(i) + "^7 - ^2" + client->szName + "^7");
			else
				SendLine(from, "^7Client ^2" + std::to_string(i) + "^7 - ^7NOT CONNECTED");
		}
	}
	else if (command == "fov")
	{
		checkargs(numargs, 1, "^2Usage /fov <65-80> (could reset on spawn).");

		auto value = GetInt(tokens[1]);
		if (value < 65 || value > 120)
		{
			SendLine(from, "^2Field of view should be ^165^2-^1120^2.");
			return true;
		}

		if (value > 80)
		{
			/* need to set fovscale */
			float scale = 1 + (1 - (value / 120.0f));
			IW4::MISC::SetClientDvar(from->EntityNumber, "cg_fov", "80");
			IW4::MISC::SetClientDvar(from->EntityNumber, "cg_fovScale", std::to_string(scale));
		}
		else
		{
			IW4::MISC::SetClientDvar(from->EntityNumber, "cg_fov", std::to_string(value));
			IW4::MISC::SetClientDvar(from->EntityNumber, "cg_fovScale", "1");
		}
	}
	else if (command == "fullbright")
	{
		checkargs(numargs, 1, "^2Usage /fullbright <0/1>.");

		auto value = GetInt(tokens[1]);
		if (value != 0 && value != 1)
		{
			SendLine(from, "^2Fullbright should be ^10^2 or ^11^2.");
			return true;
		}

		IW4::MISC::SetClientDvar(from->EntityNumber, "r_fullbright", tokens[1]);
	}
	else if (command == "killme")
	{
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, from->EntityNumber, from->EntityNumber, 0, new punishment_wrapper(PUNISH_KILL));
	}
	else if (command == "location")
	{
		auto origin = from->playerstate_s->Origin;
		auto location = "(" + std::to_string(origin[0]) + ", " + std::to_string(origin[1]) + ", " + std::to_string(origin[2]) + ")";
		SendLine(from, "^7Your ^2location^7 is: " + location);
	}
	else if (command == "setdvar")
	{
		if (from->EntityNumber == IW4::MISC::GetHostId() || ClientManager::IsSpecialClient(from->EntityNumber))
		{
			/* admin version */
			checkargs(numargs, 3, "^2Usage: /setdvar <me/id/all> <dvarname> <value>.");

			/* check dvar */
			auto dvar = IW4::DVAR::FindDvar(tokens[2].c_str());
			if (dvar == NULL)
			{
				SendLine(from, "^7Unknown dvar ^2" + tokens[2] + "^7.");
				return true;
			}

			int id;
			if (tokens[1] == "all")
			{
				for (auto i = 0; i < MAX_CLIENTS; i++)
					IW4::MISC::SetClientDvar(i, tokens[2], tokens[3]);
			}
			else if (tokens[1] == "me")
			{
				IW4::MISC::SetClientDvar(from->EntityNumber, tokens[2], tokens[3]);
			}
			else
			{
				id = GetInt(tokens[1]);
				if (id < 0 || id > 18)
				{
					SendLine(from, "^2Clientid should be ^10^2-^117^2.");
					return true;
				}
				IW4::MISC::SetClientDvar(id, tokens[2], tokens[3]);
			}
		}
		else
		{
			// user version
			checkargs(numargs, 2, "^2Usage: /setdvar <dvarname> <value>.");

			/* check dvar */
			auto dvar = IW4::DVAR::FindDvar(tokens[1].c_str());
			if (dvar == NULL)
			{
				SendLine(from, "^7Unknown dvar ^2" + tokens[1] + "^7.");
				return true;
			}

			IW4::MISC::SetClientDvar(from->EntityNumber, tokens[1], tokens[2]);
		}		
	}

	/* Commands for special clients */
	else if (command == "kick")
	{
		checkallowed(from);
		checkargs(numargs, 2, "^2Usage: /kick <id> <reason>.");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		if (!ClientManager::GetClientById(value)->IsValid)
		{
			SendLine(from, "^2Invalid client ^1" + std::to_string(value) + "^2.");
			return true;
		}

		std::string reason = "";
		for (auto i = 2u; i < originaltokens.size(); i++)
		{
			reason += originaltokens[i];

			if (i != originaltokens.size() - 1)
				reason += " ";
		}

		ClientManager::KickIdReason(value, reason);
	}
	else if (command == "ban")
	{
		checkallowed(from);
		checkargs(numargs, 2, "^2Usage: /ban <id> <reason>.");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		if (!ClientManager::GetClientById(value)->IsValid)
		{
			SendLine(from, "^2Invalid client ^1" + std::to_string(value) + "^2.");
			return true;
		}

		std::string reason = "";

		for (auto i = 2u; i < originaltokens.size(); i++)
		{
			reason += originaltokens[i];

			if (i != originaltokens.size() - 1)
				reason += " ";
		}

		ClientManager::BanIdReason(value, reason);
	}
	else if (command == "teleport")
	{
		checkallowed(from);
		checkargs(numargs, 1, "^2Usage: /teleport <me/id/all>");

		if (tokens[1] == "all")
		{
			auto location = IW4::MISC::GetTraceFromEntView(from);

			for (auto i = 0; i < MAX_CLIENTS; i++)
			{
				auto ent = EntityManager::GetGEntity(i);

				if (ent->EntityNumber == IW4::MISC::GetHostId())
					continue;

				if (ent->playerstate_s)
					ent->playerstate_s->SetOrigin(location);
			}
		}
		else
		{
			int id;
			if (tokens[1] == "me")
			{
				id = from->EntityNumber;
			}
			else
			{
				auto value = GetInt(tokens[1]);
				if (value < 0 || value > 18)
				{
					SendLine(from, "^2Clientid should be ^10^2-^117^2.");
					return true;
				}
				id = value;
			}

			if (!ClientManager::GetClientById(id)->IsValid)
			{
				SendLine(from, "^2Invalid client ^1" + std::to_string(id) + "^2.");
				return true;
			}

			auto location = IW4::MISC::GetTraceFromEntView(from);
			EntityManager::GetGEntity(id)->playerstate_s->SetOrigin(location);
		}
	}
	else if (command == "fetch")
	{
		checkallowed(from);
		checkargs(numargs, 1, "^2Usage: /fetch <id>");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		if (!ClientManager::GetClientById(value)->IsValid)
		{
			SendLine(from, "^2Invalid client ^1" + std::to_string(value) + "^2.");
			return true;
		}

		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_TELEPORT, from->EntityNumber, value, 0);
	}
	else if (command == "goto")
	{
		checkallowed(from);
		checkargs(numargs, 1, "^2Usage: /goto <id>");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		if (!ClientManager::GetClientById(value)->IsValid)
		{
			SendLine(from, "^2Invalid client ^1" + std::to_string(value) + "^2.");
			return true;
		}

		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_TELEPORT, value, from->EntityNumber, 0);
	}
	else if (command == "center")
	{
		checkallowed(from);
		checkargs(numargs, 1, "^2Usage: /center <message>");

		std::string message = "";
		for (auto i = 1u; i < originaltokens.size(); i++)
		{
			message += originaltokens[i];

			if (i != originaltokens.size() - 1)
				message += " ";
		}

		IW4::MISC::SendAllClientsMessageCenter(message);
	}
	else if (command == "sayas")
	{
		checkallowed(from);
		checkargs(numargs, 2, "^2Usage: /sayas <id> <message>");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		std::string message = "";
		for (auto i = 2u; i < originaltokens.size(); i++)
		{
			message += originaltokens[i];

			if (i != originaltokens.size() - 1)
				message += " ";
		}

		IW4::MISC::SendAllClientsChatMessage(value, message);
	}
	else if (command == "punish")
	{
		checkallowed(from);
		checkargs(numargs, 2, "^2Usage: /sayas <id> <kill/strip>");

		auto value = GetInt(tokens[1]);
		if (value < 0 || value > 18)
		{
			SendLine(from, "^2Clientid should be ^10^2-^117^2.");
			return true;
		}

		PUNISHMENT_FLAG punishment;

		if (tokens[2] == "kill")
			punishment = PUNISH_KILL;
		else if (tokens[2] == "strip")
			punishment = PUNISH_STRIP_WEAPON;
		else
			punishment = PUNISH_SLOW; // TODO: make none option

		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, from->EntityNumber, value, 0, new punishment_wrapper(punishment));
	}
	else if (command == "give")
	{
		checkallowed(from);
		checkargs(numargs, 2, "^2Usage: /give <me/id/all> <weaponname> [camo (0-8)] [akimbo (0/1)] [ammo (0-999)]");

		int id;
		if (tokens[1] == "all")
		{
			id = 99;
		}
		else if (tokens[1] == "me")
		{
			id = from->EntityNumber;
		}
		else
		{
			auto value = GetInt(tokens[1]);
			if (value < 0 || value > 18)
			{
				SendLine(from, "^2Clientid should be ^10^2-^117^2.");
				return true;
			}
			id = value;
		}

		auto weaponname = tokens[2];

		auto camo = 0;
		if (numargs >= 3)
		{
			camo = GetInt(tokens[3]);
			if (camo < 0 || camo > 8)
			{
				SendLine(from, "^2Camo should be ^10^2-^18^2.");
				return true;
			}
		}

		auto akimbo = false;
		if (numargs >= 4)
		{
			auto value = GetInt(tokens[4]);
			if (value < 0 || value > 1)
			{
				SendLine(from, "^2Akimbo should be ^10^2 or ^11^2.");
				return true;
			}
			akimbo = value == 0 ? false : true;
		}

		auto ammo = 180;
		if (numargs >= 5)
		{
			ammo = GetInt(tokens[5]);
			if (ammo < 0 || ammo > 999)
			{
				SendLine(from, "^2Ammo should be ^10^2-^1999^2.");
				return true;
			}
		}

		if (id = 99)
		{
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				if (ClientManager::GetClientById(id)->IsValid)
				{
					WeaponManager::GiveWeapon(id, const_cast<char*>(weaponname.c_str()), camo, ammo, akimbo, true);
					IW4::MISC::SendClientMessageCenter(id, "You were given a ^2" + weaponname);
				}
			}
		}
		else
		{
			WeaponManager::GiveWeapon(id, const_cast<char*>(weaponname.c_str()), camo, ammo, akimbo, true);
			IW4::MISC::SendClientMessageCenter(id, "You were given a ^2" + weaponname);
		}
	}

	/* Falltrough case */
	else
	{
		SendLine(from, "^2Invalid command '^1" + command + "^2'^7.");
	}

	auto message = "/" + command + " ";
	for (auto i = 1u; i < originaltokens.size(); i++)
	{
		message += originaltokens[i];

		if (i != originaltokens.size() - 1)
			message += " ";
	}
	printf("%s (%d): \"%s\"\n", ClientManager::GetClientById(from->EntityNumber)->szName, from->EntityNumber, message.c_str());

	return true;
}

std::vector<std::string> ChatManager::SplitMessage(const char* chatText)
{
	std::istringstream buf(chatText);
	std::istream_iterator<std::string> beg(buf), end;
	std::vector<std::string> tokens(beg, end);

	return tokens;
}

bool ChatManager::IsCommand(std::string text)
{
	if (text.length() <= 2) return false;
	return text.at(1) == '/';
}

std::string ChatManager::GetCommand(std::string command)
{
	return command.substr(2);
}

std::string ChatManager::SanitizeString(std::string text)
{
	std::regex pattern("\\^[0-9]");
	return std::regex_replace(text, pattern, "");
}

int ChatManager::GetInt(std::string token)
{
	int result;

	try
	{
		result = std::stoi(token);
	}
	catch (const std::exception&)
	{
		result = -1;
	}

	return result;
}

void ChatManager::SendLine(gentity_t* to, std::string text)
{
	IW4::G::Say(
		EntityManager::GetGEntity(IW4::MISC::GetHostId()),
		to,
		99,
		text.c_str()
	);
}
