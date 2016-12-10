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
#include "PermaPunishNode.h"
#include "CustomEventManager.h"
#include "ClientNode.h"

PermaPunishNode::PermaPunishNode()
{
}

PermaPunishNode::~PermaPunishNode()
{
}

MenuNode* PermaPunishNode::Create(MenuNode* parent, std::string label, PUNISHMENT_FLAG flag)
{
	// TODO: documentation: parent must be a branch, and branch's parent should be a client node.
	auto node = new PermaPunishNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(LEAF);
	node->SetFlag(flag);
	node->SetColor(COLOR_DISABLED);
	node->SetActiveColor(COLOR_ENABLED);
	return node;
}

MenuNode* PermaPunishNode::Select()
{
	auto parent = static_cast<ClientNode*>(this->_parent->GetParent());

	if (!parent->GetClient()->IsValid) return this;

	auto id = parent->GetClientId();
	auto xuid = IW4::MISC::GetXuid(id);
	auto punishment = parent->GetPunishment();

	if (punishment == nullptr)
	{
		// There is no punishment so add one
		punishment = PunishmentManager::AddPunishment(xuid);
		parent->AttachPunishment(punishment);
	}

	PunishmentManager::TogglePunishment(xuid, this->flag);
	CustomEventManager::PushCustomEvent(C_TYPE_DEFAULT, C_EVENT_PUNISH_UPDATE, 0, id, 0);
	return this;
}

void PermaPunishNode::DrawSelf(int* x, int* y)
{
	auto parent = static_cast<ClientNode*>(this->_parent->GetParent());
	auto client = parent->GetClient();
	auto punishment = parent->GetPunishment();
	auto is_punished = PunishmentManager::IsUserPunished(punishment, this->flag);

	color_t* color;
	if (this->_highlighted)
		color = this->GetHighlightColor();
	else
		color = is_punished && client->IsValid ? &(this->active_color) : this->GetColor();

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
	if (client->IsValid)
		dstring = this->_label;
	else
		dstring = std::string("---");

	auto width = IW4::R::TextWidth(dstring.c_str(), dstring.length(), font);
	auto height = IW4::R::TextHeight(font);
	IW4::R::AddCmdDrawStretchPic(static_cast<float>(*x), static_cast<float>(*y) - height + 5, width, height - 5, 1, 1, 1, 1, &COLOR_BACKGROUNDMENU, nullptr);
	IW4::R::AddCmdDrawText(dstring.c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);
}

void PermaPunishNode::SetFlag(PUNISHMENT_FLAG flag)
{
	this->flag = flag;
}

void PermaPunishNode::SetActiveColor(color_t color)
{
	this->active_color = color;
}
