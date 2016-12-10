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
#include "MenuNode.h"
#include "Punishment.h"

class ClientNode : public MenuNode
{
public:
	ClientNode();
	~ClientNode();

	static MenuNode* Create(MenuNode* parent, NodeType type, clientinfo_t* client);

	MenuNode* Select() override;
	MenuNode *Highlight() override;
	void OnEvent(MenuEvent event) override;
	void DrawSelf(int *x, int *y) override;

	void SetClient(clientinfo_t* client);
	unsigned int GetClientId() const;
	bool ValidatePunishment(long long xuid) const;
	void AttachPunishment(Punishment *punishment);

	Punishment *GetPunishment() const;
	clientinfo_t *GetClient() const;

private:
	clientinfo_t *_client;
	Punishment *_punishment = nullptr;
};

