#include "stdafx.h"
#include "WeaponNode.h"

WeaponNode::WeaponNode()
{

}

WeaponNode::~WeaponNode()
{

}

MenuNode* WeaponNode::Create(MenuNode* parent, std::string label, NodeType type, std::string weapon)
{
	auto node = new WeaponNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(type);
	node->SetWeapon(ConfigManager::GetWeapon(weapon));
	node->SetColor(COLOR_ENABLED);
	node->SetActiveColor(COLOR_DISABLED);
	return node;
}

void WeaponNode::SetWeapon(WeaponConfig* weapon_config)
{
	this->weapon = weapon_config;
}

void WeaponNode::SetActiveColor(const color_t& color_t)
{
	this->active_color = color_t;
}

color_t *WeaponNode::GetActiveColor()
{
	return &this->active_color;
}

MenuNode* WeaponNode::Select()
{
	switch (this->weapon->state)
	{
	case WEAPON_DEFAULT:
		this->weapon->SetState(WEAPON_STRIP);
		break;
	case WEAPON_INVALID:
		break;
	case WEAPON_STRIP:
		this->weapon->SetState(WEAPON_REPLACE);
		break;
	case WEAPON_REPLACE:
		this->weapon->SetState(WEAPON_KILL);
		break;
	case WEAPON_KILL:
		this->weapon->SetState(WEAPON_DEFAULT);
		break;
	default:
		break;
	}

	return this;
}

void WeaponNode::DrawSelf(int* x, int* y)
{
	color_t* color;
	if (this->_highlighted)
		color = this->GetHighlightColor();
	else
		color = this->weapon->state == WEAPON_DEFAULT ? this->GetColor() : this->GetActiveColor();

	std::string output;
	switch (this->_type)
	{
	case BRANCH:
		if (this->_expanded)
			output = " v";
		else
			output = " >";
		break;
	case LEAF:
		*y -= 3; // remove some space because this node is smaller
	default:
		output = "";
		break;
	}

	auto dstring = this->_label;
	switch (this->weapon->state)
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

	auto finaloutput = (dstring + output);
	auto width = IW4::R::TextWidth(finaloutput.c_str(), finaloutput.length(), font);
	auto height = IW4::R::TextHeight(font);
	IW4::R::AddCmdDrawStretchPic(static_cast<float>(*x), static_cast<float>(*y) - height + 5, width, height - 5, 1, 1, 1, 1, &COLOR_BACKGROUNDMENU, nullptr);
	IW4::R::AddCmdDrawText(finaloutput.c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);
}
