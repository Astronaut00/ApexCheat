#pragma once
#include "offsets.h"
#include "offset.hpp"
#include "memory.h"
#include "math.h"
#pragma warning(disable:4996)

//THIS SDK IS INNACURATE AND SHOULD BE REDONE AT SOME POINT
/* proper shit. My shit is shit.
* class BaseEntity
* 
* class Player : BaseEntity
* class Weapon : BaseEntity
*/

//find a home for this young lad, sdk.h aint it... Color doesn't really fit in...
struct Color { //Should these be ints, and then you can initialize with (float float float), representing the alpha in percentages?
	Color(float red, float green, float blue, float alpha = 255) {
		r = red;
		g = green;
		b = blue;
		a = alpha;
	}
	float r, g, b, a;
};

#pragma region Game Classes
//sloppy but it works...
static uint64_t GetEntityByIndex(int i) {
	return  pMem->Rpm<uint64_t>(pMem->base + OFFSET_ENTITYLIST + (i * 0x20));
}

enum class SignonState : int32_t; //forward declaration
class ClientState
{
public:
	//TODO: Add actual shit into here from reversing, On the road to Apex legends backtracking...
	ClientState() {
		base = pMem->Rpm<uint64_t>(pMem->base + OFFSET_CLIENTSTATE);
	}

	SignonState signOnState() {
		return pMem->Rpm<SignonState>(pMem->base + OFFSET_SIGNONSTATE); //technically in clientstate, but we have the direct dig and shit why not
	}

private:
	uint64_t base;
};

class Weapon
{
public:
	Weapon() {};
	Weapon(uint64_t address) {
		base = address;
	}
	/*TODO: Add ammo and shit and gravity, but gravity doesnt need this to be calculated*/
	OFFSET(m_flBulletSpeed, float, OFFSET_BULLET_SPEED)
	OFFSET(m_flBulletGravity, float, OFFSET_BULLET_GRAVITY)
	OFFSET(m_bInReload, bool, 0x16da)
	OFFSET(m_ammoInClip, int, 0x16c4)
	OFFSET(m_nextPrimaryAttackTime, float, 0x169c)
	

private:
	uint64_t base;
};

class BaseEntity
{
public:
	BaseEntity() {};
	BaseEntity(uint64_t address) {
		base = address;
	};

	bool operator==(const BaseEntity& entity) const {
		return entity.base == this->base;
	}

	bool operator!=(const BaseEntity& entity) const {
		return entity.base != this->base;
	}

	OFFSET(m_iSignifierName, uint64_t, OFFSET_SIGN_NAME)
	OFFSET(m_vecAbsOrigin, Vector, OFFSET_ORIGIN)
	OFFSET(m_hEntityHandle, uint64_t, OFFSET_ENTITY_HANDLE)
	OFFSET(m_vecAbsVelocity, Vector, OFFSET_ABS_VELOCITY)

	std::string GetSignifierName()
	{
		char itemName[32];
		auto name = m_iSignifierName()->fromCache();
		pMem->Rpm(name, itemName, 32);

		return std::string(itemName);
	}

	bool IsPlayer()
	{
		std::string identifier = pMem->Rpm<std::string>(m_hEntityHandle()->fromCache());
		return strcmp(identifier.c_str(), "player");
	}

	bool IsItem()
	{

	}
	

public:
	uint64_t base = NULL;
};

class BaseItem : public BaseEntity
{
public:
		BaseItem() {};
		BaseItem(uint64_t address) {
			base = address;
		};
		void DoGlow() {

			//Color color; //color based on item level
			if (pMem->Rpm<int>(base + OFFSET_GLOW_ENABLE) != 1 ||
				pMem->Rpm<int>(base + OFFSET_GLOW_THROUGH_WALLS) != 2) {

				pMem->Wpm<int>(base + OFFSET_GLOW_ENABLE, 1);
				pMem->Wpm<int>(base + OFFSET_GLOW_THROUGH_WALLS, 2);
				//pMem->Wpm<GlowMode>(base + GLOW_TYPE, { 101,101,10,50 });
				//pMem->Wpm<float>(base + GLOW_COLOR_R, color.r);
				//pMem->Wpm<float>(base + GLOW_COLOR_G, color.g);
				//pMem->Wpm<float>(base + GLOW_COLOR_B, color.b);
				pMem->Wpm<float>(base + GLOW_DISTANCE, FLT_MAX);
			}
		}
};

class BasePlayer : public BaseEntity
{
public:
	BasePlayer() {};
	BasePlayer(uint64_t address) {
		base = address;
	};
	/*This is the distance to the player believe it or not (from local eyepos)*/
	float distanceFromLocal = 0; //TODO: Make this into a function instead
	float fromLocalFov = 0; // LOL THIS IS A MEME, best way to optimize it tho

	/*cache of positions*/
	std::deque<Vector> positions;

	/*Offsets*/
	OFFSET(m_fFlags, int, 0x0098)
	OFFSET(m_iHealth, int, OFFSET_HEALTH)
	OFFSET(m_iTeamNum, int, OFFSET_TEAM)
	OFFSET(m_iName, int, OFFSET_NAME)
	OFFSET(m_flMaxspeed, float, 0x0420)

	OFFSET(m_flLastVisibleTime, float, OFFSET_VISIBLE_TIME)
	OFFSET(m_vecViewAngles, Vector, OFFSET_VIEWANGLES)
	OFFSET(m_vecCameraPos, Vector, OFFSET_CAMERAPOS)
	OFFSET(m_vecAimPunch, Vector, OFFSET_AIMPUNCH)
	OFFSET(m_boneArray, uint64_t, OFFSET_BONES)
	OFFSET(m_iBleedOutState, int, OFFSET_BLEED_OUT_STATE);
	OFFSET(m_bZooming, bool, OFFSET_ZOOMING)

	OFFSET(m_hLatestPrimaryWeapons, uint64_t, OFFSET_CURRENT_WEAPON)

private:
	/*TODO: Make glowmode an argument or somethin, put this in SDK File*/
	struct GlowMode {
		int8_t GeneralGlowMode, BorderGlowMode, BorderSize, TransparentLevel;
	};
public:

	/*Do in one Read, then parse through the data. This is inefficent*/
	bool GetEntityBonePosition(const uint32_t BoneId, Vector3* Out) {
		Vector3 Origin = m_vecAbsOrigin()->get();
		Out->x = pMem->Rpm<float>(m_boneArray()->get() + 0xCC + (BoneId * 0x30)) + Origin.x;
		Out->y = pMem->Rpm<float>(m_boneArray()->fromCache() + 0xDC + (BoneId * 0x30)) + Origin.y;
		Out->z = pMem->Rpm<float>(m_boneArray()->fromCache() + 0xEC + (BoneId * 0x30)) + Origin.z;
		return true;
	}

	bool IsKnocked() {
		return m_iBleedOutState()->get() > 0;
	}

	bool IsVisible(float* oVisTime) {
		const auto VisCheck = m_flLastVisibleTime()->get();

		const auto IsVis = VisCheck > (*oVisTime) || VisCheck < 0.f && *oVisTime > 0.f;

		*oVisTime = VisCheck;

		return IsVis;
	}

	Weapon *GetActiveWeapon() { //pointer so that fromCache works
		uint32_t weaponIndex = m_hLatestPrimaryWeapons()->get() & 0xffff;
		activeWeapon = Weapon(GetEntityByIndex(weaponIndex));
		return 	&activeWeapon;
	}

private: //as long as you make a comment people will forgive you for your shitty structuring of code
	Weapon activeWeapon;
public:

	bool IsDummy()
	{
		return (m_iTeamNum()->fromCache() == 97);
	}

	/*Determines if the player is valid and for use*/
	bool IsValid() {
		return (base != NULL && (IsPlayer() || IsDummy()) && m_iHealth()->get() > 0 &&
			m_iHealth()->fromCache() < 1000);
	}

	/*TODO: Make this more beaufiful*/
	void DoGlow(Color color) {
		if (pMem->Rpm<int>(base + OFFSET_GLOW_ENABLE) != 1 || 
			pMem->Rpm<int>(base + OFFSET_GLOW_THROUGH_WALLS) != 2) {

			pMem->Wpm<int>(base + OFFSET_GLOW_ENABLE, 1);
			pMem->Wpm<int>(base + OFFSET_GLOW_THROUGH_WALLS, 2);
			pMem->Wpm<GlowMode>(base + GLOW_TYPE, { 101,101,10,50 });
			pMem->Wpm<float>(base + GLOW_COLOR_R, color.r);
			pMem->Wpm<float>(base + GLOW_COLOR_G, color.g);
			pMem->Wpm<float>(base + GLOW_COLOR_B, color.b);
			pMem->Wpm<float>(base + GLOW_DISTANCE, FLT_MAX);
		}
	}
};

#pragma endregion
#pragma region Game Structs

//Pasted lol apexbot
enum class SignonState : int32_t {
	None,
	Challenge,
	Connected,
	StateNew,
	Prespawn,
	GettingData,
	Spawn,
	FirstSnap,
	Full,
	ChangeLevel,
};

//Pasted lol reverse myself in reclass later apexbot
// https://github.com/ValveSoftware/source-sdk-2013/blob/master/mp/src/public/globalvars_base.h
struct GlobalVars 
{
	/*0x00*/double realtime;
	/*0x08*/int32_t framecount;
	/*0x0c*/float absoluteframetime;
	/*0x10*/float curtime;
	/*0x14*/float curtime2;
	/*0x18*/float curtime3;
	/*0x1c*/float curtime4;
	/*0x20*/float frametime;
	/*0x24*/float curtime5;
	/*0x28*/float curtime6;
	/*0x2c*/float zero;
	/*0x30*/float frametime2;
	/*0x34*/int32_t maxClients;
	/*0x38*/int32_t unk38;
	/*0x3c*/int32_t unk3C;
	/*0x40*/int32_t tickcount;
	/*0x44*/float interval_per_tick;
	/*0x48*/float interpolation_amount;
	// There's more stuff after this but I don't know and I don't care
};

enum BoneId {
	head = 8,
	neck = 7,
	Left_shoulder = 11,
	Right_shoulder = 35,
	chest = 5,
	Spine = 3,
	Left_hand = 15,
	Right_hand = 39,
	Left_arm = 41,
	Right_arm = 16,
	Left_knee = 58,
	Right_knee = 63,
	Left_foot = 65,
	Right_foot = 60,
	Left_ElbowRoll = 13,
	Right_ElbowRoll = 37,

};

class AxisAlignedBox
{
public:
	Vector4 Min;
	Vector4 Max;
};

enum class WeaponID : int {
	BARE_HANDS = 14,

	HAVOC = 64,
	LSTAR = 65,

	KRABER = 54,
	MASTIFF = 55,
	DEVOTION = 48,

	SENTINEL = 1,
	CHARGE_RIFLE = 66,
	LONGBOW = 49,
	TRIPLE_TAKE = 62,

	WINGMAN = 63,
	SPITFIRE = 61,
	PROWLER = 57,
	HEMLOK = 53,
	FLATLINE = 51,

	RE45 = 47,
	P2020 = 60,
	R301 = 0,
	R99 = 59,
	ALTERNATOR = 46,
	G7_SCOUT = 52,

	MOZAMBIQUE = 56,
	PEACEKEEPER = 58,
	EVA8_AUTO = 50,
};
enum class HelmetID : int {
	HELMET_LV0,
	HELMET_LV1,
	HELMET_LV2,
	HELMET_LV3,
	HELMET_LV4,
};
enum class ArmorID : int {
	BODY_ARMOR_LV0,
	BODY_ARMOR_LV1,
	BODY_ARMOR_LV2,
	BODY_ARMOR_LV3,
	BODY_ARMOR_LV4,
};
enum class ItemID : int {
	DeathBox = 0,
	KRABER_HEIRLOOM = 1,
	MASTIFF = 2,
	MASTIFF_GOLD = 3,
	LSTAR = 7,
	LSTAR_GOLD = 8,
	HAVOC = 12,
	HAVOC_GOLD = 13,
	DEVOTION = 18,
	DEVOTION_GOLD = 19,
	TRIPLE_TAKE = 24,
	TRIPLE_TAKE_GOLD = 25,
	FLATLINE = 29,
	FLATLINE_GOLD = 30,
	HEMLOK = 34,
	HEMLOK_GOLD = 35,
	G7_SCOUT = 39,
	G7_SCOUT_GOLD = 40,
	ALTERNATOR = 45,
	ALTERNATOR_GOLD = 46,
	R99 = 51,
	R99_GOLD = 52,
	PROWLER_HEIRLOOM = 56,
	VOLT = 61,
	VOLT_GOLD = 62,
	LONGBOW = 66,
	LONGBOW_GOLD = 67,
	CHARGE_RIFLE = 71,
	CHARGE_RIFLE_GOLD = 72,
	SPITFIRE = 76,
	SPITFIRE_GOLD = 77,
	R301 = 81,
	R301_GOLD = 82,
	EVA8_AUTO = 86,
	EVA8_AUTO_GOLD = 87,
	PEACEKEEPER_HEIRLOOM = 91,
	MOZAMBIQUE = 92,
	MOZAMBIQUE_GOLD = 93,
	WINGMAN = 98,
	WINGMAN_GOLD = 99,
	P2020 = 104,
	P2020_GOLD = 105,
	RE45 = 110,
	RE45_GOLD = 111,
	SENTINEL = 116,
	SENTINEL_GOLD = 117,
	Repeater = 121,
	Repeater_GOLD = 122,

	LIGHT_ROUNDS = 126,
	ENERGY_AMMO,
	SHOTGUN_SHELLS,
	HEAVY_ROUNDS,
	SNIPER_AMMO,

	ULTIMATE_ACCELERANT,
	PHOENIX_KIT,
	MED_KIT,
	SYRINGE,
	SHIELD_BATTERY,
	SHIELD_CELL,

	HELMET_LV1,
	HELMET_LV2,
	HELMET_LV3,
	HELMET_LV4,
	BODY_ARMOR_LV1,
	BODY_ARMOR_LV2,
	BODY_ARMOR_LV3,
	BODY_ARMOR_LV4,
	EVO_SHIELD_LV0,
	EVO_SHIELD_LV1,
	EVO_SHIELD_LV2,
	EVO_SHIELD_LV3,
	EVO_SHIELD_LV4,
	KNOCKDOWN_SHIELD_LV1,
	KNOCKDOWN_SHIELD_LV2,
	KNOCKDOWN_SHIELD_LV3,
	KNOCKDOWN_SHIELD_LV4,
	BACKPACK_LV1,
	BACKPACK_LV2,
	BACKPACK_LV3,
	BACKPACK_LV4,

	THERMITE_GRENADE,
	FRAG_GRENADE,
	ARC_STAR,

	HCOG_CLASSIC,
	HCOG_BRUISER,
	HOLO,
	VARIABLE_HOLO,
	DIGITAL_THREAT,
	HCOG_RANGER,
	VARIABLE_AOG,
	SNIPER,
	VARIABLE_SNIPER,
	DIGITAL_SNIPER_THREAT,

	BARREL_STABILIZER_LV1,
	BARREL_STABILIZER_LV2,
	BARREL_STABILIZER_LV3,
	BARREL_STABILIZER_LV4,
	LIGHT_MAGAZINE_LV1,
	LIGHT_MAGAZINE_LV2,
	LIGHT_MAGAZINE_LV3,
	HEAVY_MAGAZINE_LV1,
	HEAVY_MAGAZINE_LV2,
	HEAVY_MAGAZINE_LV3,
	ENERGY_MAGAZINE_LV1,
	ENERGY_MAGAZINE_LV2,
	ENERGY_MAGAZINE_LV3,
	SNIPER_MAGAZINE_LV1,
	SNIPER_MAGAZINE_LV2,
	SNIPER_MAGAZINE_LV3,
	SHOTGUN_BOLT_LV1,
	SHOTGUN_BOLT_LV2,
	SHOTGUN_BOLT_LV3,
	STANDARD_STOCK_LV1,
	STANDARD_STOCK_LV2,
	STANDARD_STOCK_LV3,
	SNIPER_STOCK_LV1,
	SNIPER_STOCK_LV2,
	SNIPER_STOCK_LV3,

	TURBOCHARGER,
	SKULLPIERCER_RIFLING,
	HAMMERPOINT_ROUNDS,
	DOUBLE_TAP_TRIGGER,
	HOPUP_187,
	QUICKDRAW_HOLSTER,
	VAULT_KEY,
	MOBILE_RESPAWN_BEACON,
	ITEM_191,
	TREASURE_PACK,
};
#pragma endregion
