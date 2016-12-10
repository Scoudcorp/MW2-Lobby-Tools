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
#include "PatchDvars.h"
#include "Memory.h"

void PatchAllDvars()
{
	Patchjump_height();
	Patchbg_fallDamageMaxHeight();
	Patchbg_fallDamageMinHeight();
	Patchg_speed();
	Patchg_gravity();
	
	Unlockcg_fov();
}

void Patchjump_height()
{
	auto dvar = IW4::DVAR::RegisterFloat("jump_height", 39.0f, 0.0f, FLT_MAX, DVAR_FLAG_NONE, "The maximum height of a player's jump");
	Memory::RewriteFloatOp(static_cast<DWORD>(0x465D4F), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x59BFBA), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x59EBEC), &dvar->current.value);
}

void Patchbg_fallDamageMaxHeight()
{
	auto dvar = IW4::DVAR::RegisterFloat("bg_fallDamageMaxHeight", 300.0f, 0.0f, FLT_MAX, DVAR_FLAG_NONE, "The height that a player will take maximum damage when falling");
	Memory::RewriteFloatOp(static_cast<DWORD>(0x46D44A), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x4FC67C), &dvar->current.value);
}

void Patchbg_fallDamageMinHeight()
{
	
	auto dvar = IW4::DVAR::RegisterFloat("bg_fallDamageMinHeight", 128.0f, 0.0f, FLT_MAX, DVAR_FLAG_NONE, "The height that a player will start to take minimum damage if they fall");
	/*Memory::RewriteFloatOp(static_cast<DWORD>(0x465C86), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x48FF08), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x4FEC9A), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x51D366), &dvar->current.value);
	Memory::RewriteFloatOp(static_cast<DWORD>(0x552C42), &dvar->current.value);*/

	// TODO: this makes ents fall to their death on spawn
	//Memory::RewriteFloatOp(static_cast<DWORD>(0x544D55), &dvar->current.value);
}

void Patchg_speed()
{
	// g_speed is used here 0x51D527, default to 0xBE or 190
	//mov dword ptr[ebp + 5Ch], 0BEh
	// C7 45 5C BE 00 00 00
	auto dvar = IW4::DVAR::RegisterInt("g_speed", 190, 0, INT_MAX, DVAR_FLAG_NONE, "Player speed");

	DWORD origProtect;
	VirtualProtect(reinterpret_cast<LPVOID>(0x51D527), 7, PAGE_EXECUTE_READWRITE, &origProtect);
}

void Patchg_gravity()
{
	auto dvar = IW4::DVAR::RegisterFloat("g_gravity", 800.0f, 0.0f, FLT_MAX, DVAR_FLAG_NONE, "Game gravity in inches per second per second");
	
	// 0x0051DDA3 mov dword ptr [ebp+54h], 320h (800)
	DWORD origProtect;
	VirtualProtect(reinterpret_cast<LPVOID>(0x51DDA3), 7, PAGE_EXECUTE_READWRITE, &origProtect);
}

void Unlockcg_fov()
{
	auto cg_fov = IW4::DVAR::FindDvar("cg_fov");
	cg_fov->domain.integer.max = 180;
	cg_fov->domain.decimal.max = 180;
	cg_fov->flags = DVAR_FLAG_SAVED;
}
