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

#include "CurrentGameManager.h"

struct eventqueue_item {
	unsigned int notifyListOwnerId;
	unsigned int stringValue;
	VariableValue *top;
};

class EventManager
{
public:
	static void SetCurrentGameManager(CurrentGameManager *mCurrentGame);

	static void HandleEvent(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue *top);
	static void ProcessEventQueue();

	static void PushEvent(eventqueue_item *item);
	static eventqueue_item *PopEvent();
	static void PushEvent(unsigned int notifyListOwnerId, unsigned int stringValue, VariableValue *top);

private:
	static std::queue<eventqueue_item*> eventqueue;
	static std::mutex mutex;

	static CurrentGameManager *CurrentGame;
};

