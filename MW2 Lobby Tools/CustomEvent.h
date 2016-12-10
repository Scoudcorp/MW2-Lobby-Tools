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

#define INVALID_ID 0xFF

enum C_EVENT
{
	C_EVENT_NULL,
	C_EVENT_PUNISH,
	C_EVENT_PUNISH_UPDATE,
	C_EVENT_SPAWN,
	C_EVENT_KILL,
	C_EVENT_DEATH,
	C_EVENT_JOIN,
	C_EVENT_LEAVE,
	C_EVENT_BEGIN,
	C_EVENT_END,
	C_EVENT_BOT_JOIN_GAME,
	C_EVENT_BOT_JOIN_TEAM,
	C_EVENT_TELEPORT
};

enum C_TYPE
{
	C_TYPE_DEFAULT,
	C_TYPE_DELAYED
};

class CustomEvent
{
public:
	CustomEvent(C_TYPE type, C_EVENT event, unsigned int source_id, unsigned int target_id, unsigned int delay, void *extradata);
	~CustomEvent();

	C_TYPE GetType() const;
	C_EVENT GetEvent() const;
	unsigned int GetSourceId() const;
	unsigned int GetTargetId() const;
	unsigned int GetDelay() const;
	void *GetExtraData() const;

	void DecreaseDelay();

private:
	C_TYPE type;
	C_EVENT event;
	unsigned int source_id = INVALID_ID;
	unsigned int target_id = INVALID_ID;
	unsigned int delay = 0;
	void *extradata = nullptr;
};

