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
#include "PunishmentManager.h"

std::unordered_map<long long, Punishment*> PunishmentManager::punishments;

Punishment * PunishmentManager::AddPunishment(Punishment *punishment)
{
	printf("Created punishment with xuid %lld\n", punishment->xuid);
	PunishmentManager::punishments[punishment->xuid] = punishment;
	return punishment;
}

Punishment *PunishmentManager::AddPunishment(long long xuid)
{
	auto *punishment = new Punishment(xuid);
	return PunishmentManager::AddPunishment(punishment);
}

Punishment * PunishmentManager::AddPunishment(long long xuid, unsigned long flags)
{
	auto *punishment = new Punishment(xuid);
	punishment->flags |= flags;
	return PunishmentManager::AddPunishment(punishment);
}

bool PunishmentManager::Exists(long long xuid)
{
	auto punishment = PunishmentManager::punishments.find(xuid);
	return punishment != PunishmentManager::punishments.end();
}

Punishment *PunishmentManager::GetPunishment(long long xuid)
{
	auto punishment = PunishmentManager::punishments.find(xuid);
	if (punishment != PunishmentManager::punishments.end())
	{
		return punishment->second;
	}
	else
	{
		return nullptr;
	}
		
}

unsigned int PunishmentManager::RemovePunishment(long long xuid)
{
	return PunishmentManager::punishments.erase(xuid);
}

void PunishmentManager::EnablePunishment(Punishment *punishment, unsigned long flags)
{
	if (punishment != nullptr)
		punishment->flags |= flags;
}

void PunishmentManager::EnablePunishment(long long xuid, unsigned long flags)
{
	auto punishment = PunishmentManager::GetPunishment(xuid);
	PunishmentManager::EnablePunishment(punishment, flags);
}

void PunishmentManager::DisablePunishment(Punishment *punishment, unsigned long flags)
{
	if (punishment != nullptr)
		punishment->flags &= ~flags;
}

void PunishmentManager::DisablePunishment(long long xuid, unsigned long flags)
{
	auto punishment = PunishmentManager::GetPunishment(xuid);
	PunishmentManager::DisablePunishment(punishment, flags);
}

void PunishmentManager::TogglePunishment(Punishment* punishment, unsigned long flags)
{
	if (punishment != nullptr)
		punishment->flags ^= flags;
}

void PunishmentManager::TogglePunishment(long long xuid, unsigned long flags)
{
	auto punishment = PunishmentManager::GetPunishment(xuid);
	PunishmentManager::TogglePunishment(punishment, flags);
}

bool PunishmentManager::IsUserPunished(Punishment* punishment, unsigned long flags)
{
	if (punishment != nullptr)
		return (punishment->flags & flags) == flags;
	else
		return false;
}

bool PunishmentManager::IsUserPunished(long long xuid, unsigned long flags)
{
	auto punishment = PunishmentManager::GetPunishment(xuid);
	return PunishmentManager::IsUserPunished(punishment, flags);
}
