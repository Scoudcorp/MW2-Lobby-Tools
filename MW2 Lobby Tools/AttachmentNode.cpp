#include "stdafx.h"
#include "AttachmentNode.h"


AttachmentNode::AttachmentNode()
{
}

AttachmentNode::~AttachmentNode()
{
}

MenuNode* AttachmentNode::Create(MenuNode* parent, std::string label, std::string weapon, std::string attachment)
{
	auto node = new AttachmentNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(LEAF);
	node->SetWeapon(ConfigManager::GetWeapon(weapon));
	node->SetAttachment(attachment);
	node->SetColor(COLOR_ENABLED);
	node->SetActiveColor(COLOR_DISABLED);
	return node;
}

void AttachmentNode::SetAttachment(std::string attachment)
{
	this->attachment = attachment;
}

MenuNode* AttachmentNode::Select()
{
	auto state = this->weapon->attachments[attachment];

	switch (state)
	{
	case WEAPON_DEFAULT:
		this->weapon->SetAttachmentState(this->attachment, WEAPON_STRIP);
		break;
	case WEAPON_INVALID:
		break;
	case WEAPON_STRIP:
		this->weapon->SetAttachmentState(this->attachment, WEAPON_REPLACE);
		break;
	case WEAPON_REPLACE:
		this->weapon->SetAttachmentState(this->attachment, WEAPON_KILL);
		break;
	case WEAPON_KILL:
		this->weapon->SetAttachmentState(this->attachment, WEAPON_DEFAULT);
		this->weapon->state = WEAPON_DEFAULT;
		break;
	default:
		break;
	}

	return this;
}

void AttachmentNode::DrawSelf(int* x, int* y)
{
	auto state = this->weapon->attachments[this->attachment];

	color_t* color;
	if (this->_highlighted)
		color = this->GetHighlightColor();
	else
		color = state == WEAPON_DEFAULT ? this->GetColor() : this->GetActiveColor();

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

	auto dstring = this->_label;
	switch (state)
	{
	case WEAPON_DEFAULT:
		break;
	case WEAPON_INVALID:
		dstring += " [INVALID]";
		break;
	case WEAPON_STRIP:
		dstring += " [STRIP]";
		break;
	case WEAPON_REPLACE:
		dstring += " [REPLACE]";
		break;
	case WEAPON_KILL:
		dstring += " [KILL]";
		break;
	default:
		dstring += " [UNKNOWN]";
		break;
	}

	auto width = IW4::R::TextWidth(dstring.c_str(), dstring.length(), font);
	auto height = IW4::R::TextHeight(font);
	IW4::R::AddCmdDrawStretchPic(static_cast<float>(*x), static_cast<float>(*y) - height + 5, width, height - 5, 1, 1, 1, 1, &COLOR_BACKGROUNDMENU, nullptr);
	IW4::R::AddCmdDrawText(dstring.c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);
}
