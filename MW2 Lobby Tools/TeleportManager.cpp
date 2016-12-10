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
#include "TeleportManager.h"
#include "EntityManager.h"

bool Teleporter::InSphere(vec3_t target) const
{
	auto goal = this->flag->Origin;
	
	auto dx = (target[0] - goal[0]);
	dx = dx * dx;

	auto dy = (target[1] - goal[1]);
	dy = dy * dy;

	auto dz = (target[2] - goal[2]);
	dz = dz * dz;

	auto left = (dx + dy + dz);
	auto right = this->radius * this->radius;
	auto dif = left - right;

	return (left < right);
}

void Teleporter::Teleport(gentity_t *ent) const
{
	ent->playerstate_s->Origin[0] = this->flag->Origin[0];
	ent->playerstate_s->Origin[1] = this->flag->Origin[1];
	ent->playerstate_s->Origin[2] = this->flag->Origin[2];
}

void TeleporterManager::SetupTeleporterPair(vec3_t a, vec3_t b)
{
	auto from = new Teleporter(a);
	auto to = new Teleporter(b);
	auto pair = new TeleporterPair(from, to);
	this->teleporters.push_back(pair);
}

void TeleporterManager::RemoveAllTeleporters()
{
	this->teleporters.clear();
}

void TeleporterManager::CheckAllTeleporters()
{
	for (auto pair : this->teleporters)
	{
		for (auto i = 0; i < MAX_CLIENTS; i++)
		{
			auto ent = EntityManager::GetGEntity(i);
			if (ent == nullptr || ent->playerstate_s == nullptr) continue;

			auto in_area = pair->a->InSphere(ent->Origin);
			if (in_area)
			{
				printf("%d is in area of teleporter\n", ent->EntityNumber);
				pair->b->Teleport(ent);
			}
		}
	}
}
