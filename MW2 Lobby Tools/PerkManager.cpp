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
#include "PerkManager.h"
#include "EntityManager.h"

void PerkManager::UnsetPerk(unsigned int id, std::string perkname)
{
	auto entity = EntityManager::GetGEntity(id);
	auto playerstate = entity->playerstate_s;

	auto perkindex = IW4::BG::GetPerkIndexForName(perkname.c_str()); // 0x24 = bad perk
	if (perkindex == 0x24)
	{
		Logger::LogWarning("Invalid perk " + perkname + " for UnsetPerk. PerkIndex is " + std::to_string(perkindex));
		return;
	}

	__asm
	{
		pushad

		MOV esi, playerstate
		MOV ecx, perkindex
		AND ecx, 0x1F
		MOV edx, 1
		SHL edx, cl
		MOV eax, ebx
		ADD esi, 0x428
		SHR eax, 5
		NOT edx
		AND[esi + eax * 4], edx
		MOV ecx, playerstate
		ADD ecx, 0x3238
		AND[ecx + eax * 4], edx

		popad
	}
}

void PerkManager::SetPerk(unsigned int id, std::string perkname)
{
	auto entity = EntityManager::GetGEntity(id);
	auto playerstate = entity->playerstate_s;

	auto perkindex = IW4::BG::GetPerkIndexForName(perkname.c_str()); // 0x24 = bad perk
	if (perkindex == 0x24)
	{
		Logger::LogWarning("Invalid perk " + perkname + " for SetPerk. PerkIndex is " + std::to_string(perkindex));
		return;
	}

	__asm
	{
		pushad

		MOV edx, playerstate
		MOV ecx, perkindex
		MOV eax, perkindex
		AND ecx, 0x1F
		MOV edi, 1
		SHL edi, cl
		ADD edx, 0x428
		SHR eax, 5
		OR  [edx + eax * 4], edi
		MOV ecx, playerstate
		ADD ecx, 0x3238
		OR  [ecx + eax * 4], edi

		popad
	}
}

bool PerkManager::HasPerk(unsigned int id, std::string perkname)
{
	// TODO

	return false;
}
