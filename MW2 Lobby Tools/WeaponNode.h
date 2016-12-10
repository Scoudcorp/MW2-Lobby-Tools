#pragma once
#include "MenuNode.h"

class WeaponNode : public MenuNode
{
public:
	WeaponNode();
	~WeaponNode();

	static MenuNode* Create(MenuNode* parent, std::string label, NodeType type, std::string weapon);
	void SetWeapon(WeaponConfig* weapon_config);
	void SetActiveColor(const color_t& color_t);	

	color_t *GetActiveColor();

	MenuNode* Select() override;
	void DrawSelf(int *x, int *y) override;

protected:
	WeaponConfig *weapon;
	color_t active_color;
};

