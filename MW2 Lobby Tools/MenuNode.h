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

#pragma once

#include <vector>

enum NodeType {
	UNDEFINED,
	BRANCH,
	LEAF
};

enum MenuEvent {
	NONE,
	CONFIG_SAVE,
	CONFIG_LOAD,
	KICK,
	BAN,
	KILL,
	GOD,
	SWITCH,
	TELEPORT_FETCH,
	TELEPORT_GOTO,
	RESTART,
	OPEN_CONSOLE,
	OPEN_GAMESETUP,
	GAME_START,
	FORCE_HOST,
	PRIVATE_MATCH,
	MAX_PLAYERS,
	TOGGLE_FULLBRIGHT,
	TOGGLE_FOV,
	TOGGLE_INFINITE,
	WEAPONMODE_DEFAULT,
	WEAPONMODE_AC130,
	WEAPONMODE_SNIPER
};

class MenuNode
{
public:
	MenuNode(void);
	virtual ~MenuNode(void);

	static void* font;
	static int yoffset;

	void Finalize();
	void Expand();
	void Collapse();
	void SetParent(MenuNode* node);
	void SetLabel(std::string);
	void SetType(NodeType type);
	void SetColor(color_t);
	void SetIndex(int index);
	void SetHighlighted(bool state);
	void SetVisible(bool state);
	virtual void OnEvent(MenuEvent event);
	virtual void OnKey(char key);
	void Draw(int* x, int* y);
	virtual void DrawSelf(int *x, int *y);

	bool IsExpanded() const;

	int GetNumChildren() const;
	std::string GetLabel() const;
	color_t* GetColor();
	color_t* GetHighlightColor();

	MenuNode* GetChild(int index);
	MenuNode* GetParent() const;
	MenuNode* Next();
	MenuNode* Previous();
	virtual MenuNode* Select();
	virtual MenuNode* Highlight();
	MenuNode* GotoParent();
	MenuNode* GotoChildren();
	MenuNode* AddChild(std::string label, NodeType type);
	MenuNode* AddCustomChild(MenuNode* node);

protected:
	NodeType _type = BRANCH;
	MenuNode* _parent;
	std::vector<MenuNode*> _children;
	int _index = 0;
	std::string _label = std::string("undefined");
	color_t _color;
	color_t _highlightcolor = COLOR_HIGHLIGHTED;
	bool _expanded = false;
	bool _highlighted = false;
	bool _visible = true;
};