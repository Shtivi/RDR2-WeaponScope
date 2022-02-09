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

int camera = 0;

bool Initialize()
{
	player = PLAYER::PLAYER_PED_ID();

	ScriptSettings::load("CombatExperience.ini", new SettingsMap{
		{"AlternativeCamera", "1"},
		{"MeleeWeaponsGore", "1"},
		{"CameraShakeOnShot", "1"},
		{"DisableReticleInScope", "1"}
	});

	return true;
}

void tickAltCamera()
{
	//&& PED::IS_PED_ON_FOOT(player)
	//&& !WEAPON::_IS_WEAPON_SNIPER(!getPedEquipedWeapon(player)) 
	//&& !WEAPON::_IS_WEAPON_BINOCULARS(!getPedEquipedWeapon(player)) 

	if (PLAYER::IS_PLAYER_FREE_AIMING(0) && 
		getPedEquipedWeapon(player) != WeaponHash::Unarmed && (
			WEAPON::IS_WEAPON_A_GUN(getPedEquipedWeapon(player)) ||
			WEAPON::IS_WEAPON_BOW(getPedEquipedWeapon(player)))
	) {
		if (camera == 1)
		{
			CAM::_ANIMATE_GAMEPLAY_CAM_ZOOM(1, 1);
		}
		else if (camera == 2)
		{
			CAM::_0x90DA5BA5C2635416();

			if (CAM::GET_FINAL_RENDERED_CAM_FOV() > 26 && CAM::GET_FINAL_RENDERED_CAM_FOV() < 35)
			{
				//PAD::_SET_CONTROL_NORMAL(0, joaat("INPUT_IRON_SIGHT"), 1);
			}
		}
		else if (camera == 3)
		{
			CAM::_DISABLE_FIRST_PERSON_CAM_THIS_FRAME_2();
		}

		if (PAD::_IS_USING_KEYBOARD(0))
		{
			if (PAD::IS_CONTROL_PRESSED(0, joaat("INPUT_SNIPER_ZOOM_OUT_ONLY")))
			{
				camera = !camera;
			}
		}
		else
		{
			float controlValue = PAD::GET_CONTROL_NORMAL(0, joaat("INPUT_AIM"));

			if (camera == 0 && controlValue >= 0.7)
			{
				camera = 1;
			}
			else if (camera == 1)
			{
				if (controlValue < 0.5)
				{
					camera = 0;
				}

			}

			if (camera == 0 && controlValue >= 0.3)
			{
				camera = 1;
			}
			else if (camera == 1)
			{
				if (controlValue < 0.2)
				{
					camera = 0;
				}
				else if (controlValue >= 0.95 && PED::IS_PED_ON_FOOT(player))
				{
					camera = 2;
				}
			}
			else if (camera == 2 && controlValue < 0.6)
			{
				camera = 1;
			}
		}
	}
	else
	{
		camera = 0;
	}
}

void tickMeleeGore()
{
	if (getPedEquipedWeapon(player) == WeaponHash::MeleeMachete || getPedEquipedWeapon(player) == WeaponHash::MeleeCleaver)
	{
		Ped target = PED::GET_MELEE_TARGET_FOR_PED(player);

		if (target && ENTITY::IS_ENTITY_A_PED(target) && PED::IS_PED_HUMAN(target) && (!ENTITY::IS_ENTITY_DEAD(target) || PED::GET_PED_CONFIG_FLAG(target, 11, 0)))
		{
			int boneId = 0;
			PED::GET_PED_LAST_DAMAGE_BONE(target, &boneId);
			if (boneId == 21030 || boneId == 55120 || boneId == 53675 || boneId == 54187 || boneId == 14283)
			{
				if (PED::GET_PED_CONFIG_FLAG(target, 11, 0))
				{
					WAIT(700);
				}
				else
				{
					WAIT(80);
				}
				Vector3 exp = PED::GET_PED_BONE_COORDS(target, boneId, 0, 0, 0);
				Vector3 pos2 = exp + getUpVector(target) * 0.15;
				MISC::SHOOT_SINGLE_BULLET_BETWEEN_COORDS(pos2.x, pos2.y, pos2.z, exp.x, exp.y, exp.z, 1000, true, WeaponHash::ShotgunPump, player, false, true, -1, false);
				WAIT(80);
			}
		}
	}
}

void tickDisableReticleInScope()
{
	if (!DevTools::isActive() && PLAYER::_0x04D7F33640662FA2(PLAYER::PLAYER_ID()) && WEAPON::IS_WEAPON_A_GUN(getPedEquipedWeapon(player)))
	{
		HUD::HIDE_HUD_AND_RADAR_THIS_FRAME();
	}
}

void tickCameraShake()
{
	if (WEAPON::IS_WEAPON_A_GUN(getPedEquipedWeapon(player)) && PED::IS_PED_SHOOTING(player) && !CAM::IS_GAMEPLAY_CAM_SHAKING())
	{
		WeaponHash weap = (WeaponHash)getPedEquipedWeapon(player);
		float intensity = 0.05;

		if (WEAPON::IS_WEAPON_SHOTGUN(weap))
		{
			intensity = 0.08;
		}

		CAM::SHAKE_GAMEPLAY_CAM("SMALL_EXPLOSION_SHAKE", intensity);
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
			if (ScriptSettings::getBool("AlternativeCamera"))
			{
				tickAltCamera();
			}

			if (ScriptSettings::getBool("MeleeWeaponsGore"))
			{
				tickMeleeGore();
			}

			if (ScriptSettings::getBool("CameraShakeOnShot"))
			{
				tickCameraShake();
			}

			if (ScriptSettings::getBool("DisableReticleInScope"))
			{
				tickDisableReticleInScope();
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