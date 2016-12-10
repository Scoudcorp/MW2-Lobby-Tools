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

#include <unordered_map>
#include "Punishment.h"

class PunishmentManager
{
public:
	static Punishment *AddPunishment(Punishment *punishment);
	static Punishment *AddPunishment(long long xuid);
	static Punishment *AddPunishment(long long xuid, unsigned long flags);

	static bool Exists(long long xuid);

	static Punishment *GetPunishment(long long xuid);
	static unsigned int RemovePunishment(long long xuid);

	static void EnablePunishment(Punishment *punishment, unsigned long flags);
	static void EnablePunishment(long long xuid, unsigned long flags);

	static void DisablePunishment(Punishment *punishment, unsigned long flags);
	static void DisablePunishment(long long xuid, unsigned long flags);

	static void TogglePunishment(Punishment *punishment, unsigned long flags);
	static void TogglePunishment(long long xuid, unsigned long flags);

	static bool IsUserPunished(Punishment *punishment, unsigned long flags);
	static bool IsUserPunished(long long xuid, unsigned long flags);

private:
	static std::unordered_map<long long, Punishment*> punishments;
};

