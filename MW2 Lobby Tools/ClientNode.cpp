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
#include "ClientNode.h"
#include "CustomEventManager.h"
#include "GameManager.h"
#include "PunishmentManager.h"
#include "EntityManager.h"

ClientNode::ClientNode(): _client(nullptr)
{
}

ClientNode::~ClientNode()
{
}

MenuNode* ClientNode::Create(MenuNode* parent, NodeType type, clientinfo_t* client)
{
	auto node = new ClientNode();
	node->SetParent(parent);
	node->SetType(type);
	node->SetClient(client);
	node->SetColor(COLOR_DEFAULT);
	return node;
}

MenuNode* ClientNode::Select()
{
	IW4::MISC::SendAllClientsChatMessage(this->_client->clientNum, "I am host send me friend invite");
	return this;
}

MenuNode* ClientNode::Highlight()
{
	this->_highlighted = true;

	auto xuid = IW4::MISC::GetXuid(this->_client->clientNum);
	if (!ValidatePunishment(xuid))
	{
		auto punishment = PunishmentManager::GetPunishment(xuid);
		this->AttachPunishment(punishment);
	}

	return this;
}

void ClientNode::SetClient(clientinfo_t* client)
{
	this->_client = client;
}

void ClientNode::OnEvent(MenuEvent mevent)
{
	switch (mevent)
	{
	case KICK:
		ClientManager::KickId(this->_client->clientNum);
		break;
	case BAN:
		ClientManager::BanId(this->_client->clientNum);
		break;
	case KILL:
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH, INVALID_ID, this->_client->clientNum, 0, static_cast<void*>(new punishment_wrapper(PUNISH_KILL)));
		break;
	case GOD:
		break;
	case TELEPORT_FETCH:
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_TELEPORT, IW4::MISC::GetHostId(), this->_client->clientNum, 0);
		break;
	case TELEPORT_GOTO:
		CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_TELEPORT, this->_client->clientNum, IW4::MISC::GetHostId(), 0);
		break;
	default:
		break;
	}
}

void ClientNode::DrawSelf(int* x, int* y)
{
	if (this->_client->IsValid)
	{
		// update punishment if needed
		auto xuid = IW4::MISC::GetXuid(this->_client->clientNum);
		if (!this->ValidatePunishment(xuid))
		{
			this->AttachPunishment(PunishmentManager::AddPunishment(xuid));
		}
	}

	color_t* color;

	if (this->_highlighted)
		color = this->GetHighlightColor();
	else
		color = this->GetColor();

	switch (this->_type)
	{
	case BRANCH:
		break;
	case LEAF:
		*y -= 3; // remove some space because this node is smaller
		break;
	default:
		break;
	}

	std::string dstring;
	if (this->_client->IsValid)
		dstring = std::string(std::to_string(this->_client->clientNum) + " - " + std::string(this->_client->szName));
	else
		dstring = std::string("---");

	auto width = IW4::R::TextWidth(dstring.c_str(), dstring.length(), font);
	auto height = IW4::R::TextHeight(font);
	IW4::R::AddCmdDrawStretchPic(static_cast<float>(*x), static_cast<float>(*y) - height + 5, width, height - 5, 1, 1, 1, 1, &COLOR_BACKGROUNDMENU, nullptr);
	IW4::R::AddCmdDrawText(dstring.c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);
}

unsigned ClientNode::GetClientId() const
{
	return this->_client->clientNum;
}

bool ClientNode::ValidatePunishment(long long xuid) const
{
	if (this->_punishment != nullptr)
		return this->_punishment->xuid == xuid;
	else
		return false;
}

void ClientNode::AttachPunishment(Punishment *punishment)
{
	if (punishment != nullptr)
		this->_punishment = punishment;
}

Punishment* ClientNode::GetPunishment() const
{
	return this->_punishment;
}

clientinfo_t* ClientNode::GetClient() const
{
	return this->_client;
}
