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

#include "Client.h"
#include "TeleportManager.h"
#include <ctime>

class CurrentGameManager
{
public:
	CurrentGameManager();
	~CurrentGameManager();

	void HandleGamestate();
	void GameTick();
	void Advertise() const;

	void StartGame();
	void EndGame();

	void ClientConnect(int id) const;
	void ClientDisconnect(int id) const;

	void ClientSpawn(int id) const;

	void ClientKill(int id) const;
	void ClientDeath(int id) const;

	bool ClientAlive(int id) const;

	void ClientWeaponChange(int id) const;

	void ClientDvars(int id) const;
	void ClientLoadout(int id) const;
	void ClientPunish(int id) const;

private:
	bool gameactive = false;
	Client **clients = new Client*[MAX_CLIENTS];
	TeleporterManager *tpmanager = new TeleporterManager();
	std::clock_t last_time = std::clock();
	unsigned long interval_time = 120;

	dvar_t *cl_ingame = IW4::DVAR::FindDvar("cl_ingame");
};
