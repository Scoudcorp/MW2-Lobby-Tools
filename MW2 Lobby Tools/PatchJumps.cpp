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
#include "PatchJumps.h"
#include "Memory.h"


void PatchAllJumps()
{
	PatchTeamBalance();
	PatchSteamConnection();
}

void PatchTeamBalance()
{
	Memory::WriteBytes(reinterpret_cast<LPVOID>(0x4D73C0), "\xEB\x12", 2); // Some basic check for players in lobby
	Memory::WriteBytes(reinterpret_cast<LPVOID>(0x4D73DF), "\xEB\x14", 2); // Party combining logic
}

void PatchSteamConnection()
{
	// PartyHost_Frame, patch steamLobbyID matchup checks
	Memory::WriteBytes(0x4DA1E8, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	Memory::WriteBytes(0x4DA1F5, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);

	// OnLobbyCreated, patch disconnect when steam says you not allowed
	Memory::WriteBytes(0x628A18, "\x90\x90\x90\x90\x90", 5);

	// OnLobbyEntered, patch disconnect when steam returns anything else than "Success"
	Memory::WriteBytes(0x628AC0, "\x75", 1);

	// Steam_Frame
	Memory::WriteBytes(0x4DB721, "\xEB", 1); //\x74

	// Some steam check function (right before runcallbacks)
	Memory::WriteBytes(0x628F5F, "\x90\x90\x90\x90\x90", 5);

	// Some steam auth request, kicks players with no auth
	Memory::WriteBytes(0x627796, "\xEB", 1);

	// Some steam kick request, ignore
	Memory::WriteBytes(0x6279B8, "\x90\x90\x90\x90\x90", 5);

	// Steam login check, force login jump (TODO: test)
	Memory::WriteBytes(0x6243E3, "\xEB", 1);

	// HandleClientHandshake ignores invalid lobby id
	Memory::WriteBytes(0x4DA20D, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	Memory::WriteBytes(0x4DA21B, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);

	// HandleClientHandshake ignores users because our party's lobby id doesn't match
	Memory::WriteBytes(0x4DB755, "\x90\x90\x0\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 18);

	// AnonymousAddRequest ignores invalid lobby id
	Memory::WriteBytes(0x4DA78C, "\xEB", 1);

	// AnonymousAddRequest ingores user because our party's lobby id doesn't match
	Memory::WriteBytes(0x4DA7C4, "\xEB", 1);
	Memory::WriteBytes(0x4DA7CC, "\xEB", 1);
	
	// Some migration logic, we always want to keep hosting, dont migrate
	Memory::WriteBytes(0x5B74E2, "\xEB", 1);

	// PartyHost_StartMatch always start, even on errors
	Memory::WriteBytes(0x4D74E0, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	Memory::WriteBytes(0x4D74F5, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);
	Memory::WriteBytes(0x4D750A, "\x90\x90\x90\x90\x90\x90\x90\x90", 8);

	// Some steamlobbyid check, remove it
	Memory::WriteBytes(0x4DC436, "\x90\x90\x90\x90\x90", 5);
	Memory::WriteBytes(0x4DC43D, "\x90\x90\x90\x90\x90", 5);
	Memory::WriteBytes(0x4DC446, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 14);


	/*
	You would need to patch the game every frame with a valid lobby ID (64bit) so players can join you, 
	but you wont be able to do this with my bypass .dll since I have disabled other players joining altogether. 
	If you can get around that, you would need to patch 0x10FA9D0 with the aforementioned steam ID, 
	as well as the pointers at 0x665473C + 0x90 and 0x665473C + 0x98"
	*/
}
