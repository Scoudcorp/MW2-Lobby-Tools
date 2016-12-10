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
#include "MemoryToggleNode.h"


MemoryToggleNode::MemoryToggleNode(): function(nullptr) {
}

MemoryToggleNode::~MemoryToggleNode()
{
}

MenuNode* MemoryToggleNode::Create(MenuNode* parent, std::string label, NodeType type, void(*function)(bool))
{
	MemoryToggleNode* node = new MemoryToggleNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(type);
	node->SetAction(function);
	node->SetColor(COLOR_DEFAULT);
	return node;
}

MenuNode* MemoryToggleNode::Select()
{
	this->Toggle();
	return this;
}

void MemoryToggleNode::Toggle()
{
	this->state = !this->state;
	this->function(this->state);
}

void MemoryToggleNode::SetAction(void (*function)(bool))
{
	this->function = function;
}

void MemoryToggleNode::DrawSelf(int *x, int *y)
{
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

	IW4::R::AddCmdDrawText((this->_label + (this->state ? " [X]" : " [O]")).c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1.0f, 1.0f, 0.0f, color, 0);
}
