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
#include "InputNode.h"


InputNode::InputNode()
{
}

InputNode::~InputNode()
{
}

InputNode * InputNode::Create(MenuNode * parent, std::string label, NodeType type)
{
	auto node = new InputNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(type);
	node->SetColor(COLOR_DEFAULT);
	return node;
}

MenuNode * InputNode::Select()
{
	if (this->receiving)
	{
		// TODO: send data
	}
	else
	{
		// TODO: do something?
	}

	this->receiving = !this->receiving;

	return this;
}

void InputNode::DrawSelf(int *x, int *y)
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
	IW4::CL::DrawTextPhysical((this->_label + ": " + this->_buffer).c_str(), 0x7FFFFFFF, font, static_cast<float>(*x), static_cast<float>(*y), 1, 1, color, 0);
	//IW4::R::AddCmdDrawText((this->_label + ": " + this->_buffer).c_str(), 0x7FFFFFFF, font, *x, *y, 1.0f, 1.0f, 0.0f, color, 0);
}

void InputNode::OnKey(char c)
{
	if (!receiving) return;

	switch (c)
	{
	case '\b':
		this->Pop();
	default:
		this->Push(c);
	}
}

void InputNode::Push(char c)
{
	if (this->_buffer.size() < MAX_BUFFER_SIZE)
		this->_buffer.push_back(c);
}

void InputNode::Pop()
{
	if (!this->_buffer.empty())
		this->_buffer.pop_back();
}
