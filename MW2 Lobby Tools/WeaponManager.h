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

#define AMMO_MAX 999

class WeaponManager
{
public:
	static void GiveWeapon(int id, int weaponid, int camo, int ammo, bool akimbo, bool switchto);
	static void GiveWeapon(int id, char* weapon, int camo, int ammo, bool akimbo, bool switchto);
	static void RemoveWeapon(int id, char* weapon);
	static void StripWeapons(int id);
	static void SelectWeapon(int id, int weaponid);
	static void SelectWeapon(int id, char* weapon);
	static void SwitchWeapon(int id, int times);

	static std::string TranslateWeaponName(std::string weapon);
	static std::string TranslateAttachmentName(std::string attachment);
};

