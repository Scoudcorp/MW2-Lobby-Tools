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
#include "ActionNode.h"


ActionNode::ActionNode(): function(nullptr) {
}


ActionNode::~ActionNode()
{
}

MenuNode* ActionNode::Create(MenuNode* parent, std::string label, NodeType type, void (*function)(int))
{
	ActionNode* node = new ActionNode();
	node->SetParent(parent);
	node->SetLabel(label);
	node->SetType(type);
	node->SetAction(function);
	return node;
}

MenuNode * ActionNode::Select()
{
	this->function(this->state ? 1 : 0);
	return this;
}

void ActionNode::SetAction(void(*function)(int))
{
	this->function = function;
}

void ActionNode::DrawSelf(int* x, int* y)
{
	MenuNode::DrawSelf(x, y);
}
