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

#include <vector>
#include <map>
#include "types.h"
#include "json.h"

class ClientManager
{
public:
	static void InitClientInfos();

	static void InitSpecialClients();
	static void AddSpecialClient(long long xuid, std::string message);
	static specialclient_t* GetSpecialClient(long long xuid);
	static bool IsSpecialClient(int id);
	static bool IsSpecialClient(long long xuid);

	static void CheckAllClients();
	static void CheckClient(int id);

	static clientinfo_t* GetClientById(int id);
	static clientinfo_t* GetClientByName(const char* name);

	static void KickIdReason(int id, std::string reason);
	static void BanIdReason(int id, std::string reason);
	static void KickId(int id);
	static void BanId(int id);
	static bool IsBannedId(int id);

	static void KickXuid(long long xuid);
	static void BanXuid(long long guid);
	static bool IsBannedXuid(long long guid);

	static void UnbanXuid(long long guid);

	static nlohmann::json GetBanDump();
	static void ParseBanDump(nlohmann::json bans);

private:
	static clientinfo_t* ClientInfos[MAX_CLIENTS];
	static std::vector<specialclient_t*> specialclients;
	static std::map<long long, std::string> bannedclients;
};

