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


Prompt* p = 0;


int eventSize = 100;
int* eventData = new int[eventSize];


void DevTools::update()
{
	ENTITY::SET_ENTITY_INVINCIBLE(player, true);
	Vector3 pos = playerPos();
	int events = SCRIPTS::GET_NUMBER_OF_EVENTS(0);
	for (int i = 0; i < events; i++)
	{
		int eventType = SCRIPTS::GET_EVENT_AT_INDEX(0, i);
		if (eventType == joaat("EVENT_ENTITY_DAMAGED"))
		{
			SCRIPTS::GET_EVENT_DATA(0, i, (Any*)eventData, 9);
			int entity = eventData[0];
			int killer = eventData[1];

			int bone = 0;
			PED::GET_PED_LAST_DAMAGE_BONE(entity, &bone);

			showSubtitle(
				string("damage ")
				.append(to_string(entity))
				.append(" is animal? ")
				.append(to_string(ENTITY::GET_IS_ANIMAL(entity)))
				.append(" is dead? ")
				.append(to_string(ENTITY::IS_ENTITY_DEAD(entity)))
				.append(" killer: ")
				.append(killer == player ? "player" : to_string(killer))
				.append(" bone: ")
				.append(to_string(bone))
			);

			bool criticalOrgan = bone >= 14410 && bone <= 14413;

			PED::_SET_PED_WOUND_EFFECT(entity, 2, bone, 0, 0, 0, 0, 0, -1, 1);


			//if (!criticalOrgan)
			//{

			//	WAIT(2000);
			//PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(entity, false);
			//	WAIT(2000);
			//	PED::_REMOVE_PED_WOUND_EFFECT(entity, 0);
			//}

		} 
		else if (eventType == joaat("EVENT_SHOT_FIRED_BULLET_IMPACT"))
		{
			//SCRIPTS::GET_EVENT_DATA(0, i, (Any*)eventData, 1);
			//int entity = eventData[0];
			//Ped killer = PED::GET_PED_SOURCE_OF_DEATH(entity);

			//showSubtitle(
			//	string("bullet hit ")
			//	.append(to_string(entity))
			//	.append(" is animal? ")
			//	.append(to_string(ENTITY::GET_IS_ANIMAL(entity)))
			//	.append(" is dead? ")
			//	.append(to_string(ENTITY::IS_ENTITY_DEAD(entity)))
			//	.append(" killer: ")
			//	.append(killer == player ? "player" : to_string(killer))
			//);
		}
		else if (eventType == joaat("EVENT_TRIGGERED_ANIMAL_WRITHE"))
		{
			//SCRIPTS::GET_EVENT_DATA(0, i, (Any*)eventData, 2);

			//int animal = eventData[0];
			//int killer = eventData[1];

			//showSubtitle(
			//	string("animal writhe: ")
			//	.append(to_string(animal))
			//	.append(" killer: ")
			//	.append(killer == player ? "player" : to_string(killer))
			//);
		}
		else if (eventType == joaat("EVENT_PED_CREATED"))
		{
			SCRIPTS::GET_EVENT_DATA(0, i, (Any*)eventData, 1);

			int ped = eventData[0];

			if (distance(ped, player) < 20)
			{

				if (ENTITY::GET_IS_ANIMAL(ped))
				{
					int modelSize = PED::_GET_PED_SIZE_FROM_MODEL(ENTITY::GET_ENTITY_MODEL(ped));
					float scale = (float)rndInt(80, 120) / 100;
					PED::_SET_PED_SCALE(ped, scale);
					float height = PED::_GET_PED_HEIGHT(ped);
					float weightFactor = modelSize < 2 ? 1 : modelSize >= 4 ? 45 : 25;

					float weight = weightFactor * modelSize * scale;

					showSubtitle(
						string("ped created: ")
						.append(to_string(ped))
						.append(" distance: ")
						.append(to_string(distance(player, ped)))
						.append(" height: ")
						.append(to_string(PED::_GET_PED_HEIGHT(ped)))
						.append(" weight: ")
						.append(to_string(weight))
						.append(" size: ")
						.append(to_string(modelSize))
					);
				}

			}
		
		}
	}


	Hash weaponHash;
	WEAPON::GET_CURRENT_PED_WEAPON(player, &weaponHash, 0, 0, 0);
	if (weaponHash != MISC::GET_HASH_KEY("WEAPON_UNARMED")) {
		Entity e;
		if (PLAYER::GET_ENTITY_PLAYER_IS_FREE_AIMING_AT(PLAYER::PLAYER_ID(), &e) /*&& distanceBetweenEntities(player, e) < 20*/) 
		{
			//debug(e);

			//Vector3 ray = raycastCrosshair(50, RaycastIntersectionOptions::Peds1).hitPos;

			//int bestBone = 0;
			//float bestDist = 9999;
			//for (int i = 1; i < 20; i++)
			//{
			//	Vector3 coords = ENTITY::GET_WORLD_POSITION_OF_ENTITY_BONE(e, i);
			//	if (distance(coords, ray) < bestDist)
			//	{
			//		bestDist = distance(coords, ray);
			//		bestBone = i;
			//	}
			//}

			//debug(bestBone);

			if (IsKeyJustUp(VK_KEY_K))
			{
				PED::_SET_PED_WOUND_EFFECT(e, 2, 14412, 0, 0, 0, 0, 1000, -1, 1);
				WAIT(5000);
				PED::_REMOVE_PED_WOUND_EFFECT(e, 0);
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
		//WEAPON::SET_PED_AMMO(player, getPedEquipedWeapon(player), 100);
	}

	if (IsKeyJustUp(VK_F1))
	{
		//WEAPON::SET_PED_INFINITE_AMMO(player, true, getPedEquipedWeapon(player));
		Ped p = createPed("a_c_buck_01", playerPos() + getForwardVector(player) * 5);
		PED::SET_BLOCKING_OF_NON_TEMPORARY_EVENTS(p, 1);

		//Ped hunter = createPed("a_m_m_bivroughtravellers_01", entityPos(p) + getForwardVector(player) * 20);
		//DECORATOR::DECOR_SET_BOOL(hunter, "HONOR_OVERRIDE", 1);
		//giveWeaponToPed(hunter, WeaponHash::WeaponBow, 100, true);
		//WAIT(100);
		//WEAPON::SET_CURRENT_PED_WEAPON(hunter, WeaponHash::WeaponBow, true, 0, false, false);
		//PED::SET_PED_ACCURACY(hunter, 100);

		//int seq;
		//TASK::OPEN_SEQUENCE_TASK(&seq);
		////TASK::TASK_SEEK_CLEAR_LOS_TO_ENTITY(0, p, 5, 1, 10000);
		//TASK::TASK_SET_CROUCH_MOVEMENT(0, true, 0, 0); // crouch
		//TASK::TASK_GO_TO_ENTITY(0, p, -1, 12, 1, 0, 0);
		////TASK::TASK_AIM_GUN_AT_ENTITY(0, p, 4000, 0, 0);
		//TASK::_TASK_THROW_PROJECTILE_2(0, p, 1, 1);
		//TASK::CLOSE_SEQUENCE_TASK(seq);
		//TASK::TASK_PERFORM_SEQUENCE(hunter, seq);

	}

	LAW::_SET_WANTED_INTENSITY_FOR_PLAYER(PLAYER::PLAYER_ID(), 0);
	PLAYER::SET_MAX_WANTED_LEVEL(0);


		
	int curWeapHash = getPedEquipedWeapon(player);
	int weaponObj = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);


	if (IsKeyJustUp(VK_KEY_Z))
	{
		//WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE04"), WeaponHash::SniperrifleRollingblock);

		
		int currentWeaponEntity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);
		int comp = joaat("COMPONENT_RIFLE_SCOPE04");
		WEAPON::_GIVE_WEAPON_COMPONENT_TO_ENTITY(currentWeaponEntity, comp, -1, 0);
		
		
		
		WEAPON::_GIVE_WEAPON_COMPONENT_TO_ENTITY(player, comp, getPedEquipedWeapon(player), 1);
	}



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


	//if (TASK::IS_PED_RUNNING_TASK_ITEM_INTERACTION(player))
	//{
	//	int entity = TASK::_0x05A0100EA714DB68(player, TASK::_GET_ITEM_INTERACTION_FROM_PED(player));
	//	int entityModel = TASK::_0x804425C4BBD00883(player);

	//}
	//else
	//{
	//	int currentWeaponEntity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);

	//	debug(string("no inspect ")
	//		.append(to_string(currentWeaponEntity)).append(" ")
	//		.append(to_string(getPedEquipedWeapon(player)))
	//	);

	//}

	if (IsKeyJustUp(VK_KEY_K))
	{
		//if (!p)
		//{
		//	p = new Prompt("Scope", joaat("INPUT_CONTEXT_RT"), PromptMode::Standard);
		//	int currentWeaponEntity = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 0);

		//	p->setTargetEntity(currentWeaponEntity);
		//	p -> show();
		//}
	}

}

