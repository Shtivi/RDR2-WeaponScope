#include "Main.h"

bool DevTools::active = false;

bool DevTools::isActive()
{
	return active;
}

void DevTools::toggle()
{
	if (active)
	{
		disable();
	}
	else
	{
		enable();
	}
}

void DevTools::enable()
{
	showSubtitle("Debug tools ON");
	ENTITY::SET_ENTITY_INVINCIBLE(player, true);
	active = true;
}

void DevTools::disable()
{
	showSubtitle("Debug tools OFF");
	ENTITY::SET_ENTITY_INVINCIBLE(player, false);
	active = false;
}


struct
{
	alignas(8) int f_0;
	alignas(8) int f_1;
	alignas(8) int f_2;
	alignas(8) int f_3;
} sVar0;

struct
{
	alignas(8) int f_0 = 0;
	alignas(8) const char* f_1;
	alignas(8) int f_2 = 0;
} vVar13;


int eventSize = 100;
int* eventData = new int[eventSize];


void DevTools::update()
{
	ENTITY::SET_ENTITY_INVINCIBLE(player, true);
	Vector3 pos = playerPos();

	Hash weaponHash;
	WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
	if (weaponHash != MISC::GET_HASH_KEY("WEAPON_UNARMED")) {
		Entity e;
		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) 
		{
			if (IsKeyJustUp(VK_KEY_K))
			{
			}

			if (IsKeyJustUp(VK_KEY_Z))
			{
			}
		}
		else
		{
		}
	}
	else
	{
		Entity targetEntity;
		if (PLAYER::GET_PLAYER_TARGET_ENTITY(PLAYER::PLAYER_ID(), &targetEntity))
		{
			
			if (IsKeyJustUp(VK_KEY_Z))
			{
			}
		}
		else
		{

		}
	}

	if (IsKeyJustUp(VK_KEY_X))
	{
	}

	if (IsKeyJustUp(VK_F1))
	{
	}

	LAW::_SET_WANTED_INTENSITY_FOR_PLAYER(PLAYER::PLAYER_ID(), 0);
	PLAYER::SET_MAX_WANTED_LEVEL(0);

		
	int curWeapHash = getPedEquipedWeapon(player);
	int weaponObj = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);



	if (IsKeyJustUp(VK_KEY_Z))
	{
		int scenarios[1];
		showSubtitle(TASK::GET_SCENARIO_POINTS_IN_AREA(pos.x, pos.y, pos.z, 5, (Any*)&scenarios, 1));
	}


	debug(PED::IS_PED_USING_SCENARIO_HASH(player, joaat("WORLD_PLAYER_PLACE_BAIT_NORMAL")));

	if (IsKeyJustUp(VK_F3))
	{

		int currentWeaponEntity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);

		WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE02"), getPedEquipedWeapon(player));
		WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE03"), getPedEquipedWeapon(player));
		WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE04"), getPedEquipedWeapon(player));
		//const char* title = MISC::VAR_STRING(10, "LITERAL_STRING", "hello");

		//sVar0.f_0 = 1000;
		//sVar0.f_1 = 0;
		//sVar0.f_2 = 0;
		//sVar0.f_3 = 0;
		//vVar13.f_1 = title;
		//vVar13.f_2 = 0;
		//UIFEED::_0x893128CDB4B81FBB ((Any)&sVar0, (Any)&vVar13, 1);

	}

	if (IsKeyJustUp(VK_KEY_K))
	{
		/*int scenarios[1];
		int count = TASK::GET_SCENARIO_POINTS_IN_AREA(pos.x, pos.y, pos.z, 2.0f, (Any*)&scenarios, 1);
		showSubtitle(count);*/

		TASK::TASK_START_SCENARIO_IN_PLACE_HASH(player, joaat("WORLD_PLAYER_PLACE_BAIT_NORMAL"), 0, true, 0, -1, false);
	}

}

