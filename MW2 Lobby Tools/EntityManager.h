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
#include "stdafx.h"

/* Modelclasses

"info_notnull"
"trigger_radius"
"script_model"
"script_origin"
"script_vehicle_collmap"

*/

enum modelclass
{
	INFO_NOTNULL,
	TRIGGER_RADIUS,
	SCRIPT_MODEL,
	SCRIPT_ORIGIN,
	SCRIPT_VEHICLE_COLLMAP,
	SCRIPT_BRUSHMODEL
};

class EntityManager
{
public:
	enum godmode {
		MORTAL,
		GOD,
		KNOCKBACK,
		DEMI
	};

	static gentity_t *GetGEntity(int id);
	static gentity_t *PlayerStateToGEntity(playerstate_t *ps);

	static gentity_t *SpawnEntity(modelclass c, std::string model, vec3_t origin, vec3_t angle);

	static void SetGodMode(int id, godmode mode);

private:
	static std::string modelclasses[];
};

