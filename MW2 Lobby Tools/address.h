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

// Memory patches
#define CHOPPER_ESP_1_a	0x48854B
#define CHOPPER_ESP_2_a	0x488553
#define CHOPPER_ESP_3_a	0x488565
#define COLDBLOOD_FIX	0x4B74CE
#define THERMAL_a		0x4AFA7B
#define RECOIL_a		0x4B9F7B
#define SPREAD_a		0x47A16F

// User defined
#define MAX_CLIENTS		18
#define MAX_ENTITIES	2048
#define MAX_PLAYERSTATES 18

// iw4mp structs
#define CGS_a		    0x89D3A0
#define CG_a			0x8A0E50
#define REFDEF_a		0x90B5C8
#define ENTITY_a		0x9A4090
#define ENTITY_s		0x204
#define CLIENT_a		0x99785C
#define CLIENT_s		0x52C
#define CLIENT_S_s		0xA6790
#define CLIENT_S_a		0x3172010
#define GENTITY_a		0x194B950
#define GENTITY_s		0x274
#define PARTYSESSION_a	0x6B3580
#define GAMESTATE_a		0x66412D8

#define G_LOBBYDATA		0x10F91E8
#define PARTYSESSION_P	0x10F5A18

// iw4mp data
#define NUMPARAM		0x1FDE078
#define NEXTMAP			0x1B8F120
#define MAXPLAYERS_a	0x66414FC
#define JUMP_HEIGHT_a	0x67EC88
#define BG_FALLDAMAGEMINHEIGHT_a 0x67EC80
#define BG_FALLDAMAGEMAXHEIGHT_a 0x681DB4
#define G_GRAVITY_a		0x6831F0
