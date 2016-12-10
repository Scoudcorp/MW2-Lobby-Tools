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
#include "Memory.h"

void Memory::ToggleChopperBoxes(bool state)
{
	// TODO FIX, nops work jump change not
	/*Functions::ToggleOpcodes(JNZ, JZ, (PVOID)CHOPPER_ESP_1_a);
	Functions::ToggleOpcodes(JZ, JMP, (PVOID)CHOPPER_ESP_2_a);
	Functions::ToggleOpcodes(JZ, JMP, (PVOID)CHOPPER_ESP_3_a);*/
	WriteOpCodes(NOP, 2, reinterpret_cast<PVOID>(CHOPPER_ESP_1_a));
	WriteOpCodes(NOP, 2, reinterpret_cast<PVOID>(CHOPPER_ESP_2_a));
	WriteOpCodes(NOP, 2, reinterpret_cast<PVOID>(CHOPPER_ESP_3_a));
}

void Memory::ToggleThermalVision(bool state)
{
	ToggleOpcodes(JNZ, JZ, reinterpret_cast<PVOID>(THERMAL_a));
}

void Memory::ToggleSpread(bool state)
{
	ToggleOpcodes(JNZ, JZ, reinterpret_cast<PVOID>(SPREAD_a));
}

void Memory::ToggleRecoil(bool state)
{
	ToggleOpcodes(OJMP, JZ, reinterpret_cast<PVOID>(RECOIL_a));
}

void Memory::ToggleOpcodes(BYTE op1, BYTE op2, LPVOID address)
{
	BYTE buffer[1];
	ReadProcessMemory(GetCurrentProcess(), address, &buffer, 1, nullptr);

	if (buffer[0] == op1)
		WriteProcessMemory(GetCurrentProcess(), address, &op2, 1, nullptr);
	else if (buffer[0] == op2)
		WriteProcessMemory(GetCurrentProcess(), address, &op1, 1, nullptr);
}

void Memory::WriteOpCodes(BYTE op, int number, LPVOID address)
{
	auto *buffer = new BYTE[number];
	memset(buffer, op, number);
	WriteProcessMemory(GetCurrentProcess(), address, buffer, number, nullptr);
	delete[] buffer;
}

void Memory::WriteOpCodeBuffer(BYTE *buffer, int length, LPVOID address)
{
	DWORD origProtect;
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &origProtect);

	auto *target = static_cast<BYTE*>(address);
	for (auto i = 0; i < length; i++)
		target[i] = buffer[i];
}

void Memory::WriteBytes(unsigned int address, char * bytes, int length)
{
	WriteBytes(reinterpret_cast<LPVOID>(address), bytes, length);
}

void Memory::WriteBytes(LPVOID address, char* bytes, int length)
{
	DWORD origProtect;
	VirtualProtect(address, length, PAGE_EXECUTE_READWRITE, &origProtect);

	memcpy(address, bytes, length);
}

/**
* Rewrite a FLD instruction to a new address
* The FLD instruction is: fld dword ptr [some address]
*
* Example instruction in bytes:
* D9 05 88 EC 67 00
* <---> <--------->
*  FLD    ADDRESS
*
* The address is in reverse, so the actual address is:
* 88 EC 67 00 = 00 67 EC 88
* <--------->   <--------->
*  reversed        actual
*
* Accessing the address of a float* (&var) gives us the actual address,
* so we store it reversed.
*/
void Memory::RewriteFloatOp(DWORD source, float *destination)
{
	auto bsource = reinterpret_cast<BYTE*>(source);

	//printf("Destination is %p\n", destination);

	auto address = reinterpret_cast<int>(destination);
	BYTE bytes[4];
	bytes[0] = (address >> 24) & 0xFF;
	bytes[1] = (address >> 16) & 0xFF;
	bytes[2] = (address >> 8) & 0xFF;
	bytes[3] = address & 0xFF;

	//printf("Rewrote 0x");
	//for (auto i = 5; i > 1; i--)
	//	printf("%02X", bsource[i]);

	DWORD origProtect;
	VirtualProtect(reinterpret_cast<LPVOID>(source), 6, PAGE_EXECUTE_READWRITE, &origProtect);

	bsource[5] = bytes[0];
	bsource[4] = bytes[1];
	bsource[3] = bytes[2];
	bsource[2] = bytes[3];

	//printf(" to 0x");
	//for (auto i = 5; i > 1; i--)
	//	printf("%02X", bsource[i]);
	//printf("\n");
}

BYTE* Memory::IntToBytes(int source)
{
	BYTE bytes[4];
	bytes[0] = (source >> 24) & 0xFF;
	bytes[1] = (source >> 16) & 0xFF;
	bytes[2] = (source >> 8) & 0xFF;
	bytes[3] = source & 0xFF;

	return bytes;
}
