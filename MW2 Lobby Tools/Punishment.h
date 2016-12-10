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

enum PUNISHMENT_FLAG
{
	PUNISH_EMPTY = 0, /* Empty Punishments */
	PUNISH_ROLLERCOASTER= 1, /* Launches the player on spawn */
	PUNISH_KILL = 2, /* Kill on spawn */
	PUNISH_PLANT = 4, /* Plant (stuck) on spawn */
	PUNISH_SLOW = 8, /* Slow movement on spawn */
	PUNISH_INVERSE = 16, /* Inverse movement on spawn */
	PUNISH_STRIP_WEAPON = 32, /* Strip weapons on spawn */
	PUNISH_STRIP_PERK = 64, /* Strip perks on spawn */
	PUNISH_STRIP_ALL = 128, /* Strip everything on spawn */
	PUNISH_BLIND = 256, /* Blind (black screen) on spawn */
	PUNISH_AMMO_MAG = 512, /* 1 bullet per mag on spawn */
	PUNISH_AMMO_ALL = 1024, /* No ammo on spawn */
	PUNISH_CRAP = 2048, /* Crap weapons on spawn */
	PUNISH_MUTE = 4096, /* Can't talk (can still execute commands) */
	PUNISH_ALL = ~0, /* All punishments (rip) */
};

struct punishment_wrapper
{
	explicit punishment_wrapper(PUNISHMENT_FLAG flag) { this->flag = flag; }

	PUNISHMENT_FLAG flag;
};

class Punishment
{
public:
	Punishment(long long xuid);
	~Punishment();

	long long xuid = 0;
	unsigned long flags = PUNISH_EMPTY;
};

