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
#include "WeaponConfig.h"


WeaponConfig::WeaponConfig()
{
	this->state = WEAPON_DEFAULT;
	this->SetAllAttachments(WEAPON_DEFAULT);
}

WeaponConfig::WeaponConfig(WEAPON_STATE state)
{
	this->state = state;
	this->SetAllAttachments(WEAPON_DEFAULT);
}

WeaponConfig::~WeaponConfig()
{
}

void WeaponConfig::SetState(WEAPON_STATE state)
{
	this->state = state;
	this->SetAllAttachments(state);
}

void WeaponConfig::SetAllAttachments(WEAPON_STATE state)
{
	this->attachments["acog"] = state;
	this->attachments["akimbo"] = state;
	this->attachments["eotech"] = state;
	this->attachments["fmj"] = state;
	this->attachments["gl"] = state;
	this->attachments["grip"] = state;
	this->attachments["heartbeat"] = state;
	this->attachments["reflex"] = state;
	this->attachments["rof"] = state;
	this->attachments["shotgun"] = state;
	this->attachments["silencer"] = state;
	this->attachments["tactical"] = state;
	this->attachments["thermal"] = state;
	this->attachments["xmags"] = state;
}

void WeaponConfig::SetAttachmentState(std::string attachment, WEAPON_STATE state)
{
	this->attachments[attachment] = state;
}
