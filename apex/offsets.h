#pragma once

//TODO: DICTH THIS ENTIRE FUCKING THING, ITS CLUNKY AND STUPID + I DONT WANT IT
#define NUM_ENT_ENTRIES			(1 << 12)
#define ENT_ENTRY_MASK			(NUM_ENT_ENTRIES - 1)

#define TOFFSET(d1) d1 //((d1)+offset_ran_var)

extern unsigned long long offset_ran_var;

#define ENTITY_SIZE_DEF				0x42E0		    //biggest entity offset //0x42E0

#define OFFSET_ENTITYLIST		    0x1923578  //0x19224d8 		//0x18da338		//cl_entitylist
#define OFFSET_LOCAL_ENT			0x1cd2df8  		//0x1c898f8		//LocalPlayer d

#define OFFSET_TEAM					0x448			//m_iTeamNum
#define OFFSET_HEALTH				0x438			//m_iHealth
#define OFFSET_SHIELD				0x170			//m_shieldHealth
#define OFFSET_NAME					0x589			//m_iName
#define OFFSET_SIGN_NAME			0x580			//m_iSignifierName
#define OFFSET_ABS_VELOCITY         0x140			//m_vecAbsVelocity
#define OFFSET_ZOOMING      		0x1bf1			//m_bZooming
#define OFFSET_SHIELD				0x0170			//m_shieldHealth
#define OFFSET_MAX_SHIELD			0x0174			//m_shieldHealth +0x4
#define OFFSET_VISIBLE_TIME         0x1ACC          //m_visibletime  (+0x653) E8 ? ? ? ? FF C6 48 83 C7 30

#define OFFSET_LIFE_STATE			0x0798			//m_lifeState
#define OFFSET_BLEED_OUT_STATE		0x26d8 			//m_bleedoutState

#define OFFSET_ITEM_ID				0x1698			//m_customScriptInt

#define OFFSET_BULLET_SPEED			0x1ED0			//Inside the index of the gun entity
#define OFFSET_BULLET_GRAVITY		0x1ED8			//

#define OFFSET_CURRENT_WEAPON		0x1a6c 			//m_latestPrimaryWeapons
#define OFFSET_ORIGIN				0x14c			//m_vecAbsOrigin
#define OFFSET_BONES				0xF38			//m_nForceBone + 0x50 - 0x8 -> 48 8B 97 ?? ?? ?? ?? 48 8D 04 5B 48 C1
#define OFFSET_AIMPUNCH				0x2440			//m_vecPunchWeapon_Angle
#define OFFSET_CAMERAPOS			0x1EE0			//  7A 38 75 36 F3 41 0F 10 00 0F 2E 81 ?? ?? ?? ??
#define OFFSET_VIEWANGLES			0x2528			//m_ammoPoolCapacity - 0x14  -> 0F 57 C0 8B 86 ?? ?? ?? ?? F3 segundo uso de un 0x24AX
#define OFFSET_BREATH_ANGLES		(OFFSET_VIEWANGLES - 0x10)
#define OFFSET_OBSERVER_MODE		0x339c 			//m_iObserverMode
#define OFFSET_OBSERVING_TARGET		0x33a8 			//m_hObserverTarget
#define OFFSET_ENTITY_HANDLE		0x589

#define OFFSET_HELMET_TYPE			0x4488  			//m_helmetType


#define OFFSET_GLOW_ENABLE 0x3C8 //7 = enabled, 2 = disabled
#define OFFSET_GLOW_THROUGH_WALLS 0x3d0 //2 = enabled, 5 = disabled
#define GLOW_TYPE 0xD75					//0x2C0 //0x2C4 //OK Script_Highlight_GetState + 4 / m_highlightFunctionBits + 4?
#define GLOW_COLOR_R 0x1D0				//OK Script_CopyHighlightState mov tcx nº7 / m_highlightParams + 24 (0x18)
#define GLOW_COLOR_G 0x1D4				//OK Script_CopyHighlightState mov tcx nº7 / m_highlightParams + 24 (0x18)
#define GLOW_COLOR_B 0x1D8				//OK Script_CopyHighlightState mov tcx nº7 / m_highlightParams + 24 (0x18)
#define GLOW_DISTANCE 0x3B4				//OK Script_Highlight_SetFarFadeDist / m_highlightServerFadeEndTimes + 52(0x34)


//#define GLOW_CONTEXT				0x3C8 //Script_Highlight_SetCurrentContext
//#define GLOW_LIFE_TIME				0x3A4 //Script_Highlight_SetLifeTime + 4
//#define GLOW_DISTANCE				0x3B4 //Script_Highlight_SetFarFadeDist
//#define GLOW_TYPE					0x2C4 //Script_Highlight_GetState + 4
//#define GLOW_COLOR					0x1D0 //Script_CopyHighlightState 15th mov
//#define GLOW_VISIBLE_TYPE			0x3D0 //Script_Highlight_SetVisibilityType 5th mov
//#define GLOW_FADE					0x388 //Script_Highlight_GetCurrentInsideOpacity 3rd result of 3 offsets consecutive or first + 8

#define OFFSET_MATRIX				0x1b3bd0  //0x1D00840
#define OFFSET_RENDER				0x41f5e98  //0x41e4a08		
#define OFFSET_CLIENTSTATE			0x12C0740 //0x12af8d0
#define OFFSET_SIGNONSTATE			0x12C07D8 //0x12bf738 

#define OFFSET_IN_ATTACK 0x041f7630 
#define OFFSET_IN_JUMP 0x041f76a8 
#define OFFSET_IN_RELOAD 0x041e70d0
