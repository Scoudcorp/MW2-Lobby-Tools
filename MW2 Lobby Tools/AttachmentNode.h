#pragma once
#include "WeaponNode.h"

class AttachmentNode : public WeaponNode
{
public:
	AttachmentNode();
	~AttachmentNode();

	static MenuNode* Create(MenuNode* parent, std::string label, std::string weapon, std::string attachment);
	void SetAttachment(std::string attachment);

	MenuNode* Select() override;
	void DrawSelf(int *x, int *y) override;

private:
	color_t active_color;
	std::string attachment;
};

