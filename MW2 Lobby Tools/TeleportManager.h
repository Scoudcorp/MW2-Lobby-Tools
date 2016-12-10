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
#include "EntityManager.h"

class Teleporter
{
public:
	Teleporter()
	{
		this->flag = nullptr;
		this->radius = 100;
		this->active = false;
	}

	explicit Teleporter(gentity_t *flag)
	{
		this->flag = flag;
		this->radius = 100;
		this->active = true;
	}

	explicit Teleporter(vec3_t location) 
	{
		vec3_t angle = { 0, 0, 0 };
		this->flag = EntityManager::SpawnEntity(SCRIPT_MODEL, "prop_flag_neutral", location, angle);
		this->radius = 100;
		this->active = true;
	}

	~Teleporter() { IW4::G::FreeEntity(flag); }

	bool InSphere(vec3_t target) const;
	void Teleport(gentity_t *ent) const;

	gentity_t *flag;
	float radius;
	bool active;
};

class TeleporterPair
{
public:
	TeleporterPair(Teleporter *a, Teleporter *b) { this->a = a; this->b = b; this->oneway = true; };
	
	~TeleporterPair() { delete this->a; delete this->b; }

	Teleporter *a;
	Teleporter *b;
	bool oneway;
};

class TeleporterManager
{
public:
	TeleporterManager() {};
	~TeleporterManager() {};

	void SetupTeleporterPair(vec3_t a, vec3_t b);
	void RemoveAllTeleporters();
	void CheckAllTeleporters();


private:
	std::vector<TeleporterPair*> teleporters;
};

