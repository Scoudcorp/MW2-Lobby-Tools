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
#include "CustomEventManager.h"
#include "EntityManager.h"
#include "ClientManager.h"
#include "WeaponManager.h"
#include "Punishment.h"

std::queue<CustomEvent*> CustomEventManager::customevents;
std::mutex CustomEventManager::mutex;
CurrentGameManager *CustomEventManager::CurrentGame;

void CustomEventManager::SetCurrentGameManager(CurrentGameManager * CurrentGame)
{
	CustomEventManager::CurrentGame = CurrentGame;
}

/**
* Custom events are handled here
*/
void CustomEventManager::HandleEvent()
{
}

/**
* Handle all events in the custom event queue.
*/
void CustomEventManager::ProcessCustomEventQueue()
{
	CustomEvent *event;

	int size = CustomEventManager::customevents.size();
	for (auto i = 0; i < size; i++)
	{
		event = CustomEventManager::PopCustomEvent();

		if (!event)
		{
			printf("WARNING: PopCustomEvent returned null\n");
			return;
		}

		if (event->GetType() == C_TYPE_DELAYED && event->GetDelay() > 0)
		{
			event->DecreaseDelay();
			CustomEventManager::PushCustomEvent(event);
			continue;
		}

		switch (event->GetEvent())
		{
		case C_EVENT_NULL:
			break;
		case C_EVENT_PUNISH:
			HandlePunishEvent(event);
			break;
		case C_EVENT_PUNISH_UPDATE:
			CurrentGame->ClientPunish(event->GetTargetId());
			break;
		case C_EVENT_SPAWN:
			CurrentGame->ClientWeaponChange(event->GetTargetId());
			break;
		case C_EVENT_KILL:
			break;
		case C_EVENT_DEATH:
			break;
		case C_EVENT_JOIN:
			break;
		case C_EVENT_LEAVE:
			break;
		case C_EVENT_BEGIN:
			break;
		case C_EVENT_END:
			break;
		case C_EVENT_BOT_JOIN_GAME:
			IW4::SCR::AddString("autoassign");
			IW4::SCR::AddString("team_marinesopfor");
			IW4::SCR::NotifyNum(event->GetTargetId(), 0, *reinterpret_cast<short*>(0x1B6AAE0), 2);
			CustomEventManager::PushCustomEvent(C_TYPE_DELAYED, C_EVENT_BOT_JOIN_TEAM, INVALID_ID, event->GetTargetId(), 100);
			break;
		case C_EVENT_BOT_JOIN_TEAM:
			IW4::SCR::AddString("class2");
			IW4::SCR::AddString("changeclass");
			IW4::SCR::NotifyNum(event->GetTargetId(), 0, *reinterpret_cast<short*>(0x1B6AAE0), 2); //TODO: find SL_ConvertFromString()
			break;
		case C_EVENT_TELEPORT:
		{
			auto ent1 = EntityManager::GetGEntity(event->GetSourceId());
			auto ent2 = EntityManager::GetGEntity(event->GetTargetId());

			if (ent1 == nullptr || ent2 == nullptr || ent1->playerstate_s == nullptr || ent2->playerstate_s == nullptr) break;

			ent2->playerstate_s->SetOrigin(ent1->Origin);
		}
			break;
		default:
			printf("WARNING: Invalid CustomEvent received\n");
		}

		delete event;
	}
}


/**
* Pushes an item to the queue, locks the queue.
*/
void CustomEventManager::PushCustomEvent(CustomEvent *event)
{
	CustomEventManager::mutex.lock();
	customevents.push(event);
	CustomEventManager::mutex.unlock();
}

void CustomEventManager::PushCustomEvent(C_TYPE type, C_EVENT event, unsigned int source_id, unsigned int target_id, unsigned int delay)
{
	CustomEventManager::PushCustomEvent(new CustomEvent(type, event, source_id, target_id, delay, nullptr));
}

void CustomEventManager::PushCustomEvent(C_TYPE type, C_EVENT event, unsigned source_id, unsigned target_id, unsigned delay, void* extradata)
{
	CustomEventManager::PushCustomEvent(new CustomEvent(type, event, source_id, target_id, delay, extradata));
}

/**
* Pops an event from the queue, locks the queue. Make sure the queue has data or this fails.
*/
CustomEvent *CustomEventManager::PopCustomEvent()
{
	if (CustomEventManager::customevents.empty())
	{
		printf("WARNING: trying to PopCustomEvent on empty queue\n");
		return nullptr;
	}

	CustomEvent *event;

	CustomEventManager::mutex.lock();
	event = CustomEventManager::customevents.front();
	CustomEventManager::customevents.pop();
	CustomEventManager::mutex.unlock();

	return event;
}

void CustomEventManager::AdminMessageAll(unsigned id, std::string message)
{
	IW4::MISC::SendAllClientsMessageBold("^2Admin^7: " + std::string(ClientManager::GetClientById(id)->szName) + "^7 " + message);
}

void CustomEventManager::HandlePunishEvent(CustomEvent *event)
{
	auto targetid = event->GetTargetId();
	auto ent = EntityManager::GetGEntity(targetid);

	if (!CurrentGame->ClientAlive(targetid)) return;

	auto flag = static_cast<punishment_wrapper*>(event->GetExtraData())->flag;
	
	switch (flag)
	{
	case PUNISH_EMPTY:
		break;
	case PUNISH_ROLLERCOASTER:
		// TODO: implement
		break;
	case PUNISH_KILL:
		IW4::UNKN::player_die(ent, ent, ent, 100, 0, 0, nullptr, 0, 0);
		CustomEventManager::AdminMessageAll(targetid, "was ^1killed^7 by the server!");
		break;
	case PUNISH_PLANT:
		// TODO: implement
		break;
	case PUNISH_SLOW:
		// TODO: implement
		break;
	case PUNISH_INVERSE:
		// TODO: implement
		break;
	case PUNISH_STRIP_WEAPON:
		WeaponManager::StripWeapons(targetid);
		CustomEventManager::AdminMessageAll(targetid, "had his weapons ^1stripped^7!");
		break;
	case PUNISH_STRIP_PERK:
		// TODO: implement
		break;
	case PUNISH_STRIP_ALL:
		// TODO: implement
		break;
	case PUNISH_BLIND:
		// TODO: implement
		break;
	case PUNISH_AMMO_MAG:
		// TODO: implement
		break;
	case PUNISH_AMMO_ALL:
		// TODO: implement
		break;
	case PUNISH_CRAP:
		// TODO: implement
		break;
	case PUNISH_MUTE:
		// TODO: implement
		break;
	case PUNISH_ALL:
		// TODO: implement
		break;
	default:
		// TODO: implement
		break;
	}
}
