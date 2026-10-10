#include "MeAuriga.h"


MePort_Sig mePort[17] = {
	{ NC, NC }, {   5,   4 }, {   3,   2 }, {   7,   6 }, {   9,   8 }, 
	{ 16, 17 }, { A10, A15 }, {  A9, A14 }, {  A8, A13 }, {  A7, A12 }, 
	//             LIGHT2        LIGHT1        TEMP          SOUND
	{ A6,A11 }, {  NC,  A2 }, {  NC,  A3 }, {  NC,  A0 }, {  NC,  A1 },
	{ NC, NC }, { NC, NC },
};

Encoder_port_type encoder_Port[6] = {
	{ NC,     NC,     NC,     NC,     NC},
	//ENA A   ENA B   PWMA    DIR A2  DIR A1
	{ 19,     42,     11,     49,     48},
	//ENB A   ENB B   PWMB    DIR B1  DIR B2
	{ 18,     43,     10,     47,     46},
	{ NC,     NC,     NC,     NC,     NC},
	{ NC,     NC,     NC,     NC,     NC},
};
