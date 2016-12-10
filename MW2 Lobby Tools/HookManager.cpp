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
#include "HookManager.h"
#include <future>

#include "ConfigManager.h"

#include "GameManager.h"
#include "CurrentGameManager.h"
#include "EventManager.h"
#include "CustomEventManager.h"

#include "PatchDvars.h"
#include "PatchJumps.h"
#include "MemoryManager.h"
#include "DrawManager.h"
#include "detours.h"
#include "ChatManager.h"
#include "PunishmentManager.h"

#define F_PROLOG __asm \
{ \
	__asm push ebp \
	__asm mov ebp, esp \
	__asm sub esp, __LOCAL_SIZE \
	__asm pushad \
}

#define F_EPILOG(X) __asm \
{ \
	__asm popad \
	__asm mov esp, ebp \
	__asm pop ebp \
	__asm jmp [X] \
}

// Original functions
typedef LRESULT(WINAPI* DispatchMessage_t)(MSG*);
DispatchMessage_t oDispatchMessage;

typedef void(*Refresh_t)(int localClientNum);
Refresh_t oRefresh;

typedef void(*Notify_fp)(unsigned int notifyListOwnerId, unsigned int stringValue, void *VariableValue_top);
Notify_fp oNotify;

typedef void(*Resume_fp)(unsigned int timeId);
Resume_fp oResume;

typedef void(*Say_fp)(gentity_t *from, gentity_t *to, int mode, const char *chatText);
Say_fp oSay;

typedef dvar_t *(*SetFromStringByNameFromSource_fp)(const char *dvarName, const char *string, DvarSetSource source);
SetFromStringByNameFromSource_fp oSetFromStringByNameFromSource;

typedef void(*setClientDvar_fp)(scr_entref_t entref);
setClientDvar_fp osetClientDvar;

typedef void(*SetPlayerData_fp)();
SetPlayerData_fp oSetPlayerData;

// Managers
MemoryManager* mMemoryManager;
DrawManager* mDrawManager;
CurrentGameManager *mCurrentGameManager;

void HookRefresh()
{
	oRefresh = reinterpret_cast<Refresh_t>(DetourFunction(reinterpret_cast<PBYTE>(0x59DE50), reinterpret_cast<PBYTE>(mRefresh)));
	printf("UI_Refresh hooked\n");
}

void HookDispatchMessageA()
{
	unsigned long DispatchMessageAddress = 0;
 
	while(!((DispatchMessageAddress = reinterpret_cast<unsigned long>(GetProcAddress(GetModuleHandleA("User32.dll"), "DispatchMessageA")))))
		Sleep(250);
 
	oDispatchMessage = reinterpret_cast<DispatchMessage_t>(DetourFunction(reinterpret_cast<PBYTE>(DispatchMessageAddress), reinterpret_cast<PBYTE>(mDispatchMessage)));
	printf("DispatchMessageA hooked\n");
}

void HookNotify()
{
	oNotify = reinterpret_cast<Notify_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x57FA20), reinterpret_cast<PBYTE>(mNotify)));
	printf("VM_Notify hooked\n");
}

void HookResume()
{
	oResume = reinterpret_cast<Resume_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x583F90), reinterpret_cast<PBYTE>(mResume)));
	printf("VM_Resume hooked\n");
}

void HookSay()
{
	oSay = reinterpret_cast<Say_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x525F90), reinterpret_cast<PBYTE>(mSay)));
	printf("G_Say hooked\n");
}

void HookSetFromStringByNameFromSource()
{
	oSetFromStringByNameFromSource = reinterpret_cast<SetFromStringByNameFromSource_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x5BE7A0), reinterpret_cast<PBYTE>(mSetFromStringByNameFromSource)));
	printf("Dvar_SetFromStringByNameFromSource hooked\n");
}

void HooksetClientDvar()
{
	osetClientDvar = reinterpret_cast<setClientDvar_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x522070), reinterpret_cast<PBYTE>(msetClientDvar)));
	printf("PlayerCmd_setClientDvar hooked\n");
}

void HookSetPlayerData()
{
	oSetPlayerData = reinterpret_cast<SetPlayerData_fp>(DetourFunction(reinterpret_cast<PBYTE>(0x5A4450), reinterpret_cast<PBYTE>(mSetPlayerData)));
	printf("SetPlayerData hooked\n");
}
	
LRESULT WINAPI mDispatchMessage(MSG* lpmsg)
{
	auto wParam = lpmsg->wParam;

	auto handled = false;

	switch(lpmsg->message)
	{
	case WM_KEYDOWN:
		switch(wParam)
		{
		case VK_F1:
			// TODO: auto open on startup
			GameManager::OpenIW4Console();
			break;
		case VK_F2:
			GameManager::OpenGameSetup();
			break;
		case VK_F3:
			GameManager::GameForceStart();
			break;
		case VK_F4:
			GameManager::ToggleForceHost();
			break;
		case VK_F5:
			GameManager::TogglePrivateMatch();
			break;
		case VK_F6:
			GameManager::ToggleMaxPlayers();
			break;

		case VK_F7:	
			IW4::UI::PopupScriptMenu(0, "popup_endgame", true);
			break;

		case VK_F8:
		{
			/*auto unset_combathigh = 9866;
			IW4::SCR::NotifyNum(2, 0, unset_combathigh, 0);*/
			IW4::CMD::ExecuteSingleCommand(0, 0, "xpartygo");
		}
			break;

		case VK_F9:
			mDrawManager->MenuToggle();
			break;
		
		case VK_UP:
			mDrawManager->CursorUp();
			handled = true;
			break;
		case VK_DOWN:
			mDrawManager->CursorDown();
			handled = true;
			break;
		case VK_LEFT:
			mDrawManager->CursorBack();
			handled = true;
			break;
		case VK_RIGHT:
			mDrawManager->CursorNext();
			handled = true;
			break;
		case VK_RETURN:
			mDrawManager->CursorSelect();
			handled = true;
			break;
		}
	case WM_CHAR:
		mDrawManager->KeyEvent(wParam);
		break;
	}

	if (!handled || !mDrawManager->IsOpen())
		return oDispatchMessage(lpmsg);
	return false;
}

void mRefresh(int localClientNum)
{
	oRefresh(localClientNum);

	mCurrentGameManager->HandleGamestate();

	/* === DO UI HERE === */

	mDrawManager->DrawMenu();

	/* ===   END UI   === */
}

void mNotify(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue *top)
{
	EventManager::HandleEvent(notifyListOwnerId, stringValue, top);
	
	return oNotify(notifyListOwnerId, stringValue, top);
}

__declspec(naked) void __cdecl mResume(unsigned int timeId)
{
	F_PROLOG;

	mCurrentGameManager->GameTick();
	CustomEventManager::ProcessCustomEventQueue();
	EventManager::ProcessEventQueue();
	
	F_EPILOG(oResume);
}

void mSay(gentity_t *from, gentity_t *to, int mode, const char *chatText)
{
	auto handled = false;

	auto muted = PunishmentManager::IsUserPunished(IW4::MISC::GetXuid(from->EntityNumber), PUNISH_MUTE);

	if (mode == 99)
		return oSay(from, to, 0, chatText);
	else
		handled = ChatManager::HandleChatMessage(from, to, mode, chatText);

	if (!handled && !muted)
		return oSay(from, to, mode, chatText);
	else if (!handled && muted)
		return oSay(EntityManager::GetGEntity(IW4::MISC::GetHostId()), from, 0, "^7You are ^1muted^7!");
}

dvar_t* mSetFromStringByNameFromSource(const char* dvarName, const char* string, DvarSetSource source)
{
	//printf("byName: (%d) %s set to %s\n", source, dvarName, string);

	if (strcmp(dvarName, "g_speed") == 0)
	{
		auto value = atoi(string);
		auto g_speed = IW4::DVAR::FindDvar("g_speed");

		if (value >= g_speed->domain.integer.min && value <= g_speed->domain.integer.max)
		{
			auto bytes = Memory::IntToBytes(value);
			auto bsource = reinterpret_cast<BYTE*>(0x51D527);
			BYTE bdest[7] = { 0xC7, 0x45, 0x5C, bytes[3], bytes[2], bytes[1], bytes[0] };
			memcpy(bsource, bdest, 7 * sizeof BYTE);
		}
	}
	else if (strcmp(dvarName, "g_gravity") == 0)
	{
		auto value = atoi(string);
		auto g_speed = IW4::DVAR::FindDvar("g_gravity");

		if (value >= g_speed->domain.decimal.min && value <= g_speed->domain.decimal.max)
		{
			auto bytes = Memory::IntToBytes(value);
			auto bsource = reinterpret_cast<BYTE*>(0x51DDA3);
			BYTE bdest[7] = { 0xC7, 0x45, 0x54, bytes[3], bytes[2], bytes[1], bytes[0] };
			memcpy(bsource, bdest, 7 * sizeof BYTE);
		}
	}

	return oSetFromStringByNameFromSource(dvarName, string, source);
}

void msetClientDvar(scr_entref_t entref)
{
	auto client = entref.entnum;
	auto dvar = IW4::SCR::GetString(0);
	auto value = IW4::SCR::GetString(1);
	//printf("(%d) %s -> %s\n", client, dvar, value);

	/* Disble the forced cg_fov on spawn */
	if (strcmp(dvar, "cg_fov") == 0 && strcmp(value, "65") == 0) return;

	return osetClientDvar(entref);
}

__declspec(naked) void mSetPlayerData()
{
	F_PROLOG;

	//printf("SetPlayerData\n");

	//int result;
	//char **string;
	//char out[0x400];

	//__asm
	//{
	//	push ebx
	//	mov ebx, [esp + 0xC]
	//	mov string, ebx
	//	pop ebx
	//}

	///*for (int i = 0; i < 0x400; i++)
	//	result = IW4::UNKN::String_Parse(string, &out[i], 0x400);*/

	//for (int i = 0; i < 8; i++)
	//{
	//	printf("string[%d] = \"%s\"\n", i, string[i]);
	//}

	//printf("String_Parse returned %d -> \"%s\".\n", result, out);

	F_EPILOG(oSetPlayerData);
}

/*
CreateDetour initializes MW2LT.
All static managers are initialized and all hooking methods are called. 
Patches various bytes in the IW4MP game.
After completion MW2LT is fully functional.
*/
DWORD WINAPI CreateDetour(LPVOID lpvoid)
{
	Logger::InitLogger(Logger::LOGGER_LEVEL_ALL);

	ConfigManager::InitWeaponList();
	ClientManager::InitClientInfos();
	ClientManager::InitSpecialClients();

	mMemoryManager = new MemoryManager;
	mDrawManager = new DrawManager;
	mCurrentGameManager = new CurrentGameManager;

	EventManager::SetCurrentGameManager(mCurrentGameManager);
	CustomEventManager::SetCurrentGameManager(mCurrentGameManager);
	GameManager::SetCurrentGameManager(mCurrentGameManager);

	mDrawManager->InitMenu();

	HookRefresh();
	HookDispatchMessageA();
	HookNotify();
	HookResume();
	HookSay();
	HookSetFromStringByNameFromSource();
	HooksetClientDvar();
	
	//HookSetPlayerData();

	PatchAllDvars();
	PatchAllJumps();

	ConfigManager::LoadConfig();

	return 0;
}
