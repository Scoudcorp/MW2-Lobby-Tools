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
#include <queue>
#include <mutex>

#include "CustomEvent.h"
#include "CurrentGameManager.h"

class CustomEventManager
{
public:
	static void SetCurrentGameManager(CurrentGameManager *CurrentGame);

	static void HandleEvent();
	static void ProcessCustomEventQueue();

	static void PushCustomEvent(CustomEvent *event);
	static void PushCustomEvent(C_TYPE type, C_EVENT event, unsigned int source_id, unsigned int target_id, unsigned int delay);
	static void PushCustomEvent(C_TYPE type, C_EVENT event, unsigned int source_id, unsigned int target_id, unsigned int delay, void *extradata);
	static CustomEvent *PopCustomEvent();

	static void AdminMessageAll(unsigned int id, std::string message);

	static void HandlePunishEvent(CustomEvent *event);
	
private:
	static std::queue<CustomEvent*> customevents;
	static std::mutex mutex;
	static CurrentGameManager *CurrentGame;
};

