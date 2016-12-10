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
#include "CustomEvent.h"

CustomEvent::CustomEvent(C_TYPE type, C_EVENT event, unsigned int source_id, unsigned int target_id, unsigned int delay, void *extradata)
{
	this->type = type;
	this->event = event;
	this->source_id = source_id;
	this->target_id = target_id;
	this->delay = delay;
	this->extradata = extradata;
}

CustomEvent::~CustomEvent()
{
	if (this->extradata != nullptr)
		delete this->extradata;
}

C_TYPE CustomEvent::GetType() const
{
	return this->type;
}

C_EVENT CustomEvent::GetEvent() const
{
	return this->event;
}

unsigned int CustomEvent::GetSourceId() const
{
	return this->source_id;
}

unsigned int CustomEvent::GetTargetId() const
{
	return this->target_id;
}

unsigned int CustomEvent::GetDelay() const
{
	return this->delay;
}

void* CustomEvent::GetExtraData() const
{
	return this->extradata;
}

void CustomEvent::DecreaseDelay()
{
	this->delay -= 1;
}
