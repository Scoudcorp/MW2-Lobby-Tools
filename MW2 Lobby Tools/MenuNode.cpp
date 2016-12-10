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

#include "StdAfx.h"
#include "MenuNode.h"
#include "GameManager.h"

void* MenuNode::font = IW4::R::RegisterFont(FONT_SMALL, 0);
int MenuNode::yoffset = IW4::R::TextHeight(font) - 5;

MenuNode::MenuNode(void): _parent(nullptr) {
}

/**
 * Deconstructor
 */
MenuNode::~MenuNode(void)
{
	this->Finalize();
}

/**
 * Call cleanup on all children
 */
void MenuNode::Finalize()
{
	for(auto child = this->_children.begin(); child != this->_children.end(); ++child)
	{
		(*child)->Finalize();
		delete (*child);
	}
}

/**
 * Expand this menu item, exposing children
 */ 
void MenuNode::Expand()
{
	this->_expanded = true;
}

/**
 * Collapse this menu item, hiding children
 */ 
void MenuNode::Collapse()
{
	this->_expanded = false;
}

void MenuNode::SetParent(MenuNode * node)
{
	this->_parent = node;
}

/**
 * Set the label text of this menu item
 */ 
void MenuNode::SetLabel(std::string label)
{
	this->_label = label;
}

void MenuNode::SetType(NodeType type)
{
	this->_type = type;
}

/**
 * Set the text color of this menu item
 */ 
void MenuNode::SetColor(color_t color)
{
	this->_color = color;
}

/*

*/
void MenuNode::SetIndex(int index)
{
	this->_index = index;
}

/**
 * Set the highlighted state
 */ 
void MenuNode::SetHighlighted(bool state)
{
	this->_highlighted = state;
}

void MenuNode::SetVisible(bool state)
{
	this->_visible = state;
}

/**
 * This function handles events
 */
 void MenuNode::OnEvent(MenuEvent mevent)
 {
	switch (mevent)
	{
	case CONFIG_SAVE:
		ConfigManager::SaveConfig();
		break;
	case CONFIG_LOAD:
		ConfigManager::LoadConfig();
		break;
	case RESTART:
		IW4::SV::MapRestart(0);
		break;
	case OPEN_CONSOLE:
		GameManager::OpenIW4Console();
		break;
	case OPEN_GAMESETUP:
		GameManager::OpenGameSetup();
		break;
	case GAME_START:
		GameManager::GameForceStart();
		break;
	case FORCE_HOST:
		GameManager::ToggleForceHost();
		break;
	case PRIVATE_MATCH:
		GameManager::TogglePrivateMatch();
		break;
	case MAX_PLAYERS:
		GameManager::ToggleMaxPlayers();
		break;
	case TOGGLE_FULLBRIGHT:
		GameManager::ToggleFullBright();
		break;
	case TOGGLE_INFINITE:
		GameManager::ToggleInfiniteAmmo();
		break;
	case WEAPONMODE_DEFAULT:
	case WEAPONMODE_AC130:
	case WEAPONMODE_SNIPER:
		GameManager::SetWeaponMode(mevent);
		break;
	default:
		break;
	}
 }

 void MenuNode::OnKey(char key)
 {
 }


/**
 * Draw this node
 */ 
void MenuNode::Draw(int* x, int* y)
{	
	if (this->_visible)
	{
		this->DrawSelf(x, y);

		// Next row drawing
		*y += yoffset;
	}

	// Draw children if this is expanded or is a root
	if(this->_expanded || !this->_parent)
	{
		for(auto child = this->_children.begin(); child != this->_children.end(); ++child)
		{
			*x += 15;
			(*child)->Draw(x, y);
			*x -= 15;
		}
	}
}

/**
 * Draws this node, virtual
 */
void MenuNode::DrawSelf(int *x, int *y)
{
	color_t* color;

	if (this->_highlighted)
		color = this->GetHighlightColor();
	else
		color = this->GetColor();

	std::string output;

	switch (this->_type)
	{
	case BRANCH:
		if (this->_expanded)
			output = (this->_label + " v");
		else
			output = (this->_label + " >");
		break;
	case LEAF:
	default:
		output = (this->_label);
		break;
	}

	auto width = IW4::R::TextWidth(output.c_str(), output.length(), font);
	auto height = IW4::R::TextHeight(font);
	IW4::R::AddCmdDrawStretchPic(static_cast<float>(*x), static_cast<float>(*y) - height + 5, static_cast<float>(width), static_cast<float>(height) - 5, 1, 1, 1, 1, &COLOR_BACKGROUNDMENU, nullptr);
	IW4::R::AddCmdDrawText(output.c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);

}

/**
 * Add a new child to the _children vector
 */
MenuNode* MenuNode::AddChild(std::string label, NodeType type)
{
	auto node = new MenuNode;
	node->SetParent(this);
	node->SetLabel(label);
	node->SetType(type);
	node->SetIndex(this->GetNumChildren());
	this->_children.push_back(node);
	return node;
}

MenuNode* MenuNode::AddCustomChild(MenuNode* node)
{
	node->SetIndex(this->GetNumChildren());
	this->_children.push_back(node);
	return node;
}

/**
 * Returns whether this node is expanded
 */ 
bool MenuNode::IsExpanded() const
{
	return this->_expanded;
}

/**
 * Returns the number of children of this node
 */ 
int MenuNode::GetNumChildren() const
{
	return this->_children.size();
}

/**
 * Returns the label text
 */ 
std::string MenuNode::GetLabel() const
{
	return this->_label;
}

/**
 * Returns the text color
 */ 
color_t* MenuNode::GetColor()
{
	return &(this->_color);
}

color_t * MenuNode::GetHighlightColor()
{
	return &(this->_highlightcolor);
}

/**
 * Get child at index
 */
MenuNode* MenuNode::GetChild(int index)
{
	// No children, go to next in line
	if (this->_children.empty())
	{
		return this->Highlight();
	}

	// Has children, but index out of bounds
	if (index < 0)
	{
		return this->_children.back()->Highlight();
	}
	else if (static_cast<unsigned int>(index) >= this->_children.size())
	{
		return this->_children.front()->Highlight();
	}

	// correct index, return child at.
	return this->_children.at(index)->Highlight();
}

MenuNode* MenuNode::GetParent() const
{
	return this->_parent;
}

/**
 * Get the next node
 */
MenuNode* MenuNode::Next()
{
	if (!this->_parent) return this->Highlight();
	return this->_parent->GetChild(this->_index + 1);
}

/**
 * Get the previous node
 */
MenuNode* MenuNode::Previous()
{
	if (!this->_parent) return this->Highlight();
	return this->_parent->GetChild(this->_index - 1);
}


/**
 * Select this node, return reference to itself
 * This method should implement some action
 */
MenuNode* MenuNode::Select()
{
	return this;
}

/**
 * Highlight this node, return reference to itself
 */
MenuNode* MenuNode::Highlight()
{
	this->_highlighted = true;
	return this;
}

/**
 * Highlight the first child, returns itself when no children
 */
MenuNode* MenuNode::GotoChildren()
{
	if (this->GetNumChildren() > 0)
	{
		this->Expand();
		return this->_children.at(0)->Highlight();
	}
	else
	{
		return this->Highlight();
	}
}

/**
 * Highlight the parent, returns itself when no parent
 */
MenuNode* MenuNode::GotoParent()
{
	if (this->_parent != nullptr && this->_parent->GetParent() != nullptr)
	{
		this->_parent->Collapse();
		return this->_parent->Highlight();
	}
	else
	{
		return this->Highlight();
	}
}