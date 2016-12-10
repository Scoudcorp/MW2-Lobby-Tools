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
#include "DrawManager.h"
#include "EventNode.h"
#include "ClientNode.h"
#include "PermaPunishNode.h"
#include "PunishNode.h"
#include "WeaponNode.h"
#include "AttachmentNode.h"
#include "WeaponManager.h"

DrawManager::DrawManager(void)
{
	this->menu = new MenuNode;
	this->menu->SetLabel("Settings");
	this->menu->SetType(BRANCH);
	this->menu->SetParent(nullptr);
	this->menu->Expand();
	this->menu->SetVisible(false);
	this->currentNode = menu;
}

DrawManager::DrawManager(const DrawManager &): menu(nullptr), currentNode(nullptr)
{
}

DrawManager::~DrawManager(void)
{
}

void DrawManager::InitMenu()
{
	auto saveload = this->menu->AddChild("Save/Load", BRANCH);
		saveload->AddCustomChild(EventNode::Create(saveload, "Save Config", LEAF, CONFIG_SAVE));
		saveload->AddCustomChild(EventNode::Create(saveload, "Load Config", LEAF, CONFIG_LOAD));
	auto actions = this->menu->AddChild("Actions", BRANCH);
		auto globalactions = actions->AddChild("Global", BRANCH);
			globalactions->AddCustomChild(EventNode::Create(globalactions, "Toggle FullBright", LEAF, TOGGLE_FULLBRIGHT));
			globalactions->AddCustomChild(EventNode::Create(globalactions, "Toggle FoV", LEAF, TOGGLE_FOV));
			globalactions->AddCustomChild(EventNode::Create(globalactions, "Toggle Infinite Ammo", LEAF, TOGGLE_INFINITE));
		auto funactions = actions->AddChild("Fun", BRANCH);
			auto weaponmodes = funactions->AddChild("Weaponmodes", BRANCH);
				weaponmodes->AddCustomChild(EventNode::Create(weaponmodes, "Default", LEAF, WEAPONMODE_DEFAULT));
				weaponmodes->AddCustomChild(EventNode::Create(weaponmodes, "AC-130 Mode", LEAF, WEAPONMODE_AC130));
				weaponmodes->AddCustomChild(EventNode::Create(weaponmodes, "Sniper Mode ", LEAF, WEAPONMODE_SNIPER));
	auto weapons = this->menu->AddChild("Weapons", BRANCH);
		auto launchers = weapons->AddChild("Launchers/Misc.", BRANCH);
			for (auto weapon : ConfigManager::launchers)
			{
				launchers->AddCustomChild(WeaponNode::Create(launchers, WeaponManager::TranslateWeaponName(weapon), LEAF, weapon));
			}
		auto handguns = weapons->AddChild("Handguns", BRANCH);
			for (auto weapon : ConfigManager::handguns)
			{
				auto wep = handguns->AddCustomChild(WeaponNode::Create(handguns, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto machinepistols = weapons->AddChild("Machine Pistols", BRANCH);
			for (auto weapon : ConfigManager::machinepistols)
			{
				auto wep = machinepistols->AddCustomChild(WeaponNode::Create(machinepistols, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto submachineguns = weapons->AddChild("Sub Machine Guns", BRANCH);
			for (auto weapon : ConfigManager::submachineguns)
			{
				auto wep = submachineguns->AddCustomChild(WeaponNode::Create(submachineguns, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto assaultrifles = weapons->AddChild("Assault Rifles", BRANCH);
			for (auto weapon : ConfigManager::assaultrifles)
			{
				auto wep = assaultrifles->AddCustomChild(WeaponNode::Create(assaultrifles, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto sniperrifles = weapons->AddChild("Sniper Rifles", BRANCH);
			for (auto weapon : ConfigManager::sniperrifles)
			{
				auto wep = sniperrifles->AddCustomChild(WeaponNode::Create(sniperrifles, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto shotguns = weapons->AddChild("Shotguns", BRANCH);
			for (auto weapon : ConfigManager::shotguns)
			{
				auto wep = shotguns->AddCustomChild(WeaponNode::Create(shotguns, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
		auto lightmachineguns = weapons->AddChild("Light Machine guns", BRANCH);
			for (auto weapon : ConfigManager::lightmachineguns)
			{
				auto wep = lightmachineguns->AddCustomChild(WeaponNode::Create(lightmachineguns, WeaponManager::TranslateWeaponName(weapon), BRANCH, weapon));
					for (auto attachment : ConfigManager::attachments)
					{
						wep->AddCustomChild(AttachmentNode::Create(wep, WeaponManager::TranslateAttachmentName(attachment), weapon, attachment));
					}
			}
	auto clients = this->menu->AddChild("Clients", BRANCH);
	for (auto i = 0; i < MAX_CLIENTS; i++)
	{
		auto child = clients->AddCustomChild(ClientNode::Create(clients, BRANCH, ClientManager::GetClientById(i)));
			child->AddCustomChild(EventNode::Create(child, "Kick", LEAF, KICK));
			child->AddCustomChild(EventNode::Create(child, "Ban", LEAF, BAN));
			child->AddCustomChild(EventNode::Create(child, "Fetch", LEAF, TELEPORT_FETCH));
			child->AddCustomChild(EventNode::Create(child, "Goto", LEAF, TELEPORT_GOTO));
			auto punishments = child->AddChild("Punishments", BRANCH);
				punishments->AddCustomChild(PunishNode::Create(punishments, "Coaster", PUNISH_ROLLERCOASTER));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Kill", PUNISH_KILL));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Plant", PUNISH_PLANT));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Slow", PUNISH_SLOW));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Inverse", PUNISH_INVERSE));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Strip Perks", PUNISH_STRIP_PERK));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Strip Weapons", PUNISH_STRIP_WEAPON));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Strip All", PUNISH_STRIP_ALL));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Blind", PUNISH_BLIND));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Low Ammo", PUNISH_AMMO_MAG));
				punishments->AddCustomChild(PunishNode::Create(punishments, "No Ammo", PUNISH_AMMO_ALL));
				punishments->AddCustomChild(PunishNode::Create(punishments, "Crap Weapons", PUNISH_CRAP));
			auto permapunishments = child->AddChild("Perma Punishments", BRANCH);
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Mute", PUNISH_MUTE));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Coaster", PUNISH_ROLLERCOASTER));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Kill", PUNISH_KILL));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Plant", PUNISH_PLANT));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Slow", PUNISH_SLOW));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Inverse", PUNISH_INVERSE));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Strip Perks", PUNISH_STRIP_PERK));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Strip Weapons", PUNISH_STRIP_WEAPON));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Strip All", PUNISH_STRIP_ALL));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Blind", PUNISH_BLIND));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Low Ammo", PUNISH_AMMO_MAG));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma No Ammo", PUNISH_AMMO_ALL));
				permapunishments->AddCustomChild(PermaPunishNode::Create(permapunishments, "Perma Crap Weapons", PUNISH_CRAP));
	}

	this->currentNode = this->menu->GotoChildren()->Highlight();
}

void DrawManager::DrawMenu() const
{
	if (!this->MenuOpen) return;

	auto x = 5, y = 40;
	this->menu->Draw(&x, &y);
}

void DrawManager::CursorUp()
{
	if (!this->MenuOpen) return;

	IW4::SND::PlayLocalSoundAliasByName(0, "match_countdown_tick", 0);

	this->currentNode->SetHighlighted(false);
	this->currentNode = this->currentNode->Previous();
}

void DrawManager::CursorDown()
{
	if (!this->MenuOpen) return;
	
	IW4::SND::PlayLocalSoundAliasByName(0, "match_countdown_tick", 0);

	this->currentNode->SetHighlighted(false);
	this->currentNode = this->currentNode->Next();
}

void DrawManager::CursorBack()
{
	if (!this->MenuOpen) return;

	IW4::SND::PlayLocalSoundAliasByName(0, "match_countdown_tick", 0);

	this->currentNode->SetHighlighted(false);
	this->currentNode = this->currentNode->GotoParent();
}

void DrawManager::CursorNext()
{
	if (!this->MenuOpen) return;

	IW4::SND::PlayLocalSoundAliasByName(0, "match_countdown_tick", 0);

	this->currentNode->SetHighlighted(false);
	this->currentNode = this->currentNode->GotoChildren();
}

void DrawManager::CursorSelect() const
{
	if (!this->MenuOpen) return;

	IW4::SND::PlayLocalSoundAliasByName(0, "MP_hit_alert", 0);

	this->currentNode->Select();
}

bool DrawManager::IsOpen() const
{
	return this->MenuOpen;
}

void DrawManager::MenuToggle()
{
	this->MenuOpen = !this->MenuOpen;
}

bool DrawManager::KeyEvent(WPARAM wparam) const
{
	auto handled = false;

	auto c = static_cast<char>(wparam);
	auto isAscii = (c >= 32 && c <= 126);
	auto isSpecial = (c == '\b');

	if (isAscii || isSpecial)
	{
		handled = true;
		this->currentNode->OnKey(static_cast<char>(wparam));
	}
	
	return handled;
}
