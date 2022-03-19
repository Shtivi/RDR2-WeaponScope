/*
/*
	THIS FILE IS A PART OF RDR 2 SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2019
*/

#include "Main.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

Ped player;
Prompt* toggleScopePrompt = NULL;

bool Initialize()
{
	player = PLAYER::PLAYER_PED_ID();

	ScriptSettings::load("WeaponScope.ini", new SettingsMap{
		{"DisableReticleInScope", "1"},
		{"EnableScopeToggle", "1"},
		{"ScopeTogglePromptLabel", "Toggle Scope"},

	});

	toggleScopePrompt = new Prompt(ScriptSettings::get("ScopeTogglePromptLabel").c_str(), joaat("INPUT_FRONTEND_RS"), PromptMode::SemiHold);
	toggleScopePrompt->hide();

	return true;
}

int getWeaponScopeComponentType(Entity weaponObject)
{
	int scopeShort = joaat("COMPONENT_RIFLE_SCOPE02");
	int scopeMedium = joaat("COMPONENT_RIFLE_SCOPE03");
	int scopeLarge = joaat("COMPONENT_RIFLE_SCOPE04");

	return 
		WEAPON::HAS_WEAPON_GOT_WEAPON_COMPONENT(weaponObject, scopeShort) ? scopeShort : 
		WEAPON::HAS_WEAPON_GOT_WEAPON_COMPONENT(weaponObject, scopeMedium) ? scopeMedium : 
		WEAPON::HAS_WEAPON_GOT_WEAPON_COMPONENT(weaponObject, scopeLarge) ? scopeLarge : NULL;
}

void tickDisableReticleInScope()
{
	if (!DevTools::isActive() && PLAYER::_0x04D7F33640662FA2(PLAYER::PLAYER_ID()) && WEAPON::IS_WEAPON_A_GUN(getPedEquipedWeapon(player)))
	{
		HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
}

map<int, int> cachedPlayerScopeByWeaponHash;

void tickScopeToggle()
{
	Ped horse = getPlayerSaddleHorse();
	toggleScopePrompt->setTargetEntity(horse);

	//Hash currentWeaponHash = getPedEquipedWeapon(player, 0) != WeaponHash::Unarmed ? getPedEquipedWeapon(player, 0) : getPedEquipedWeapon(player, 1);
	Hash currentWeaponHash = getPedEquipedWeapon(player, 1);

	if (distance(player, horse) < 3 &&
		PED::_0x56E58D4D118FB45E(getPlayerSaddleHorse(), (Any)"Agitation") < 0.3 &&
		!ENTITY::IS_ENTITY_DEAD(horse) && 
		!TASK::IS_PED_IN_WRITHE(horse) &&
		(WEAPON::IS_WEAPON_REPEATER(currentWeaponHash) || WEAPON::IS_WEAPON_RIFLE(currentWeaponHash) || WEAPON::_IS_WEAPON_SNIPER(currentWeaponHash)))
	{
		int weaponObj = WEAPON::GET_CURRENT_PED_WEAPON_ENTITY_INDEX(player, 1);

		toggleScopePrompt->show();
		if (toggleScopePrompt->isActivatedByPlayer())
		{
			int scopeComponentType = getWeaponScopeComponentType(weaponObj);

			
			if (scopeComponentType)
			{
				cachedPlayerScopeByWeaponHash[currentWeaponHash] = scopeComponentType;
				WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE02"), currentWeaponHash);
				WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE03"), currentWeaponHash);
				WEAPON::_REMOVE_WEAPON_COMPONENT_FROM_PED(player, joaat("COMPONENT_RIFLE_SCOPE04"), currentWeaponHash);
			}
			else
			{
				int scopeComponent = 
					cachedPlayerScopeByWeaponHash.find(currentWeaponHash) != cachedPlayerScopeByWeaponHash.end() ? 
					cachedPlayerScopeByWeaponHash[currentWeaponHash] : 
					joaat("COMPONENT_RIFLE_SCOPE02");

				WEAPON::_GIVE_WEAPON_COMPONENT_TO_ENTITY(player, scopeComponent, currentWeaponHash, 1);
				WEAPON::_GIVE_WEAPON_COMPONENT_TO_ENTITY(weaponObj, scopeComponent, -1, 0);
			}

			TASK::TASK_ANIMAL_INTERACTION(player, horse, joaat("Interaction_DrawRifleFromUnderSaddle_BothHands"), 0, true);
		}
	}
	else
	{
		toggleScopePrompt->hide();
	}
}


void main()
{
	WAIT(500);


	if (!Initialize())
	{
		return;
	}

	while (true)
	{
		player = PLAYER::PLAYER_PED_ID();

		try
		{
			if (ScriptSettings::getBool("DisableReticleInScope"))
			{
				tickDisableReticleInScope();
			}

			if (ScriptSettings::getBool("EnableScopeToggle"))
			{
				tickScopeToggle();
			}
		}
		catch (...)
		{
			log("Something wrong happened");
			std::exception_ptr ex = std::current_exception();
			try
			{
				if (ex)
				{
					rethrow_exception(ex);
				}
				else
				{
					log("No exception captured.");
				}
			}
			catch (const exception& e)
			{
				const char* logMsg =
					string("Fatal: ")
					.append(e.what())
					.append(", check the logs for more info.").c_str();

				log(logMsg);
				showSubtitle(logMsg);
			}
		}

		if (DevTools::isActive())
		{
			DevTools::update();
		}

		if (false && IsKeyJustUp(VK_F2))
		{
			DevTools::toggle();
		}

		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}

void debug(const char* text)
{
	drawText((char*)text, 0, 0, 255, 255, 255, 255, false, 0.7, 0.7, "$title");
}

void debug(string text) {
	debug(text.c_str());
}

void debug(Vector3 pos) {
	stringstream str;
	str << pos.x << ", " << pos.y << ", " << pos.z;
	debug(str.str());
}

void debug(float f) {
	debug(to_string(f));
}

void debug(int n) {
	debug(to_string(n));
}

void debug(bool b) {
	debug(to_string(b));
}

void debug(Hash hash) {
	debug(to_string((int)hash));
}

void logPlayerPos()
{
	Vector3 playerPos = entityPos(player);
	float ground;
	MISC::GET_GROUND_Z_FOR_3D_COORD(playerPos.x, playerPos.y, playerPos.z, &ground, false);
	std::stringstream output;
	output << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << playerPos.z << "\n"
		<< playerPos.x << ", " << playerPos.y << ", " << ground << "\n"
		<< "heading: " << ENTITY::GET_ENTITY_HEADING(player);

	log(output.str().c_str());
	showSubtitle(to_string(getGroundPos(playerPos)));
}