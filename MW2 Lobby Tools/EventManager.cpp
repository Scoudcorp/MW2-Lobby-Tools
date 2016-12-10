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
#include "EventManager.h"
#include "CurrentGameManager.h"
#include "EntityManager.h"
#include "GameManager.h"

#define FILTER_DEATH

std::queue<eventqueue_item*> EventManager::eventqueue;
std::mutex EventManager::mutex;
CurrentGameManager *EventManager::CurrentGame;

void EventManager::SetCurrentGameManager(CurrentGameManager * CurrentGame)
{
	EventManager::CurrentGame = CurrentGame;
}

/**
 * Events from VM_Notify enter here, if the event is something we want to process
 * push it on our event queue.
 */
void EventManager::HandleEvent(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue *top)
{
	switch (stringValue)
	{
	case 13370001:
		printf("Received player (%d) weapon changed!\n", IW4::SCR::GetSelf(notifyListOwnerId));
		break;
	default:
		break;
	}

	std::string game_event(IW4::SL::ConvertToString(stringValue));

	bool pushevent = false;
	
	if (
		game_event == "match_start_timer_beginning" || /* Match has started (timer is shown) */ 
		game_event == "exitLevel_called" || /* The game has ended */
		game_event == "killed_player" || /* A player has been killed */
		game_event == "killed_enemy" || /* A player has killen an enemy */
		game_event == "spawned_player" || /* A player is spawning */
		game_event == "giveLoadout" || /* A player has received their spawn loadout */
		game_event == "changed_kit" || /* A player has changed their loadout */
		game_event == "weapon_change" || /* A player finishes weapon changing */
		game_event == "joined_team" || /* A player joined spectators (first action that happens in game */
		game_event == "weapon_fired" /* A weapon has fired */
	)
	{
		pushevent = true;
	}
#ifdef _DEBUG
	#ifdef FILTER_DEATH
		//if (game_event != "death") printf("event: %s\n", game_event.c_str());
	#else
		//printf("%s: %s\n", pushevent ? "HANDLED_EVENT" : "UNHANDLED_EVENT", game_event.c_str());
	#endif
#endif

	if (pushevent)
	{
		auto v = new VariableValue;
		v->entity = top->entity;
		v->integer = top->integer;
		v->number = top->number;
		v->string = top->string;
		v->type = top->type;
		v->vector = top->vector;

		EventManager::PushEvent(notifyListOwnerId, stringValue, v);
	}
}

/**
 * Handle all events in the event queue.
 */
void EventManager::ProcessEventQueue()
{
	eventqueue_item *item;

	while (!EventManager::eventqueue.empty())
	{
		item = EventManager::PopEvent();

		if (!item)
		{
			printf("WARNING: PopEvent returned null\n");
			return;
		}

		std::string game_event(IW4::SL::ConvertToString(item->stringValue));


		if (game_event == "match_start_timer_beginning")
		{
			//CurrentGame->StartGame();
		}
		else if (game_event == "joined_team")
		{
			auto id = IW4::SCR::GetSelf(item->notifyListOwnerId);
			CurrentGame->ClientConnect(id);
		}
		else if (game_event == "exitLevel_called")
		{
			//CurrentGame->EndGame();
		}
		else if (game_event == "killed_player")
		{
			auto id = IW4::SCR::GetSelf(item->notifyListOwnerId);
			CurrentGame->ClientDeath(id);
		}
		else if (game_event == "killed_enemy")
		{
			auto id = IW4::SCR::GetSelf(item->notifyListOwnerId);
			CurrentGame->ClientKill(id);
		}
		else if (game_event == "spawned_player")
		{
			auto id = IW4::SCR::GetSelf(item->notifyListOwnerId);
			CurrentGame->ClientSpawn(id);
		}
		else if (game_event == "weapon_change")
		{
			auto id = IW4::SCR::GetSelf(item->notifyListOwnerId);
			CurrentGame->ClientWeaponChange(id);
		}
		else if (game_event == "weapon_fired")
		{
			int id = IW4::SCR::GetSelf(item->notifyListOwnerId);

			if (GameManager::InfiniteAmmo) // && ClientManager::IsSpecialClient(id)
			{
				
				auto gentity = EntityManager::GetGEntity(id);

				auto playerstate = reinterpret_cast<int*>(IW4::SV::GameClientNum(id));
				auto weaponid = IW4::BG::GetViewmodelWeaponIndex(playerstate);
				IW4::MISC::Add_Ammo(gentity, weaponid, 0, 1, 1);
			}
		}

		delete item->top;
		delete item;
	}
}

/**
 * Pushes an item to the queue, locks the queue.
 */
void EventManager::PushEvent(eventqueue_item *item)
{
	EventManager::mutex.lock();
	eventqueue.push(item);
	EventManager::mutex.unlock();
}

/**
 * Pops an event from the queue, locks the queue. Make sure the queue has data or this fails.
 */
eventqueue_item *EventManager::PopEvent()
{
	if (EventManager::eventqueue.empty())
	{
		printf("WARNING: trying to PopEvent on empty queue\n");
		return nullptr;
	}

	eventqueue_item *item;

	EventManager::mutex.lock();
	item = EventManager::eventqueue.front();
	EventManager::eventqueue.pop();
	EventManager::mutex.unlock();

	return item;
}

/**
 * Create and push a new event
 */
void EventManager::PushEvent(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue * top)
{
	EventManager::PushEvent(new eventqueue_item{ notifyListOwnerId , stringValue, top });
}
