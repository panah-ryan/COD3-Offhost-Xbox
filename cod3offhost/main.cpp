#include "stdafx.h"
#include "Detour.h"
#include "Globals.h"
#include "Menu.h"
#include "Drawing.h"
#include "Engine.h"
#include "MenuFunc.h"

using namespace::std;

extern bool MenuOpen;
extern int refDefWidth;
extern int refDefHeight;

DWORD dwTitleID = NULL;

entity_s** entityArray = (entity_s**)0x82B99EC4;
entity_s* ourEnt = nullptr;
clientActive_t* clientActive = (clientActive_t*)0x82BEEBF0;

bool GetStructs()
{
	if (*(int*)0x82BA9EC4 && entityArray[0] != nullptr)
	{
		for (int i = 0; i < *(int*)0x82BA9EC4; i++)
		{
			entity_s* ent = entityArray[i];
			if (ent && Entity_IsLocalPlayer(ent))
			{
				ourEnt = ent;
				break;
			}
		}

		if (ourEnt != nullptr)
			return true;
	}
	return false;
}

bool IsEnemy(entity_s* ent)
{
	return ourEnt->getTeam() == 0 || ourEnt->getTeam() != ent->getTeam();
}

AimTarget aimTargets[18];
void DrawClient(entity_s* ent, Color color)
{
	Vector2 feetLoc, headLoc;
	if (WorldToScreen(ent->pos, feetLoc) && WorldToScreen(GetTagPos(ent, "Bip01 Helmet"), headLoc))
	{
		float drawX = feetLoc.x, drawY = feetLoc.y;
		float fheight = feetLoc.y - (headLoc.y - 5.0f);
		float fwidth = fheight / 1.95f;

		if (shaderType == S_2D)
			Draw2DBox(feetLoc, fwidth, fheight, color, false);
		else if (shaderType == S_CORNER)
			DrawCornerBox(feetLoc, fwidth, fheight, color);
		else if (shaderType == S_FILLED)
			Draw2DBox(feetLoc, fwidth, fheight, color, true);

		if (drawNames)
		{
			if(ent->getPeer())
				DrawTextBackground(ent->getPeer()->gamertag, feetLoc.x, headLoc.y - 20.0f, 0.5f);
		}

		if (drawDistance)
		{
			char buffer[64];
			sprintf_s(buffer, "%.2f", GetDistance(ent->pos, GetOurEntity()->pos));
			DrawTextBackground(buffer, feetLoc.x, feetLoc.y, 0.5f);
		}
	}
}

void ESP()
{
	for (int i = 0; i < *(int*)0x82BA9EC4; i++)
	{
		entity_s* ent = entityArray[i];
		if (ent && ent->type == ET_PLAYER)
		{
			if (Entity_IsLocalPlayer(ent) || ent->isDead() || !ent->getPeer())
				continue;

			bool shouldDraw = espSelectType == E_ALL || (!IsEnemy(ent) && espSelectType == E_FR) || (IsEnemy(ent) && espSelectType == E_EN);
			Color clientColor;
			if (!IsEnemy(ent))
				clientColor = alliesColor;
			else if (IsTagVisible(ent, "Bip01 Helmet"))
				clientColor = enemiesVisColor;
			else
				clientColor = enemiesMainColor;

			if (shouldDraw)
				DrawClient(ent, clientColor);
		}
	}
}

Detour<void> CG_Draw2DDetour;
void CG_Draw2D(int r3, int r4)
{
	if (GetStructs())
	{
		//printf("isDead %i\n", GetOurEntity()->isDead());

		if(!GetOurEntity()->isDead())
			ESP();
	}

	CG_Draw2DDetour.CallOriginal(r3, r4);
}

Detour<DWORD> XamInputGetStateDetour;
DWORD XamInputGetStateHook(DWORD dwUserIndex, DWORD r4, PXINPUT_STATE pState)
{
	DWORD dwResult = XamInputGetStateDetour.CallOriginal(dwUserIndex, r4, pState);

	if (dwUserIndex != 0 || dwResult != 0)
		return dwResult;

	if (MenuOpen)
	{
		// remove all menu input from the game
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_UP;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_DOWN;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_LEFT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_DPAD_RIGHT;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_X;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_B;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_A;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_LEFT_SHOULDER;
		pState->Gamepad.wButtons &= ~XINPUT_GAMEPAD_RIGHT_SHOULDER;
	}

	return dwResult;
}

MenuClass* Playertab;
Detour<void> RE_EndFrameDetour;
extern vector<MenuClass*> menuTabs;
extern int currentTabIndex;
void RE_EndFrame()
{
	RE_EndFrameDetour.CallOriginal();

	static DWORD Tick;
	XINPUT_STATE State;
	if (FAILED(XInputGetState(0, &State)))
		goto End;

	if (State.Gamepad.bLeftTrigger >= 30 &&
		State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN &&
		(GetTickCount() - Tick) > 500)
	{
		Tick = GetTickCount();
		MenuOpen ^= true;
	}

	if (MenuOpen)
	{
		if (GetStructs())
		{
			int menuIndex = 0;
			for (int i = 0; i < *(int*)0x82BA9EC4; i++)
			{
				entity_s* ent = entityArray[i];
				if (ent && ent->type == ET_PLAYER && menuIndex < 24)
				{
					MenuClassItem* menuItem = &Playertab->menuItems->operator[](menuIndex);
					mpeer_s* peer = ent->getPeer();
					strncpy(menuItem->itemText, (peer && peer->gamertag[0]) ? peer->gamertag : "Empty...", 16);
					menuIndex++;
				}
			}
		}

		DisplayMenu(menuTabs.at(currentTabIndex));
	}
	if (drawServerInfo)
	{
		char buffer[100];
		char cvarBuffer[128];

		Cvar_GetBuffer("mapname", cvarBuffer, 128);
		_snprintf(buffer, 100, "Map: %s", cvarBuffer);
		DrawInfoBox(buffer, 0);

		Cvar_GetBuffer("mp_gametype", cvarBuffer, 128);
		_snprintf(buffer, 100, "Gametype: %s", cvarBuffer);
		DrawInfoBox(buffer, 1);

		_snprintf(buffer, 100, "FPS: %i", CalculateFPS());
		DrawInfoBox(buffer, 2);

		static int cpuTemp = 0;
		static int gpuTemp = 0;
		static DWORD tempTick;

		if ((GetTickCount() - tempTick) > 10000)
		{
			char m_SMCMessage[16] = { 0 };
			char m_SMCReturn[16] = { 0 };

			m_SMCMessage[0] = 7;
			HalSendSMCMessage(m_SMCMessage, m_SMCReturn);
			cpuTemp = m_SMCReturn[2];
			gpuTemp = m_SMCReturn[4];
			tempTick = GetTickCount();
		}

		_snprintf(buffer, 100, "CPU: %i °C", cpuTemp);
		DrawInfoBox(buffer, 3);

		_snprintf(buffer, 100, "GPU: %i °C", gpuTemp);
		DrawInfoBox(buffer, 4);

		if (GetStructs())
		{
			_snprintf(buffer, 100, "Players: %i / 24", GetPlayerCount());
			DrawInfoBox(buffer, 5);

			if (ourEnt)
			{
				_snprintf(buffer, 100, "X: %.2f, %.2f, %.2f", ourEnt->pos.x, ourEnt->pos.y, ourEnt->pos.z);
				DrawInfoBox(buffer, 6);
			}
		}
	}
End:
	return;
}

entity_s* GetBestTarget()
{
	float closetDistance = FLT_MAX;
	entity_s* closestClient = nullptr;

	for (int i = 0; i < *(int*)0x82BA9EC4; i++)
	{
		entity_s* ent = entityArray[i];

		if (!ent || !ent->type == ET_PLAYER)
			continue;

		if (Entity_IsLocalPlayer(ent) || ent->isDead() || !ent->getPeer() || !IsEnemy(ent))
			continue;

		float distance = GetDistance(ent->pos, GetOurEntity()->pos);
		if (distance > aimDistance)
			continue;

		if (distance < closetDistance)
		{
			closetDistance = distance;
			closestClient = ent;
		}

		if (!IsTagVisible(ent, "bip01 head"))
			continue;
	}

	return closestClient;
}

void CalcAngle(Vector3 src, Vector3 dst, Vector3 *angles)
{
	Vector3 delta = src - dst;
	double hyp = delta.Length2D();
	angles->y = atan(delta.y / delta.x) * 57.295779513082f;
	angles->x = atan(delta.z / hyp) * 57.295779513082f;
	angles->z = 0.0f;

	if (delta.x >= 0.0f)
		angles->y += 180.0f;
}

Detour<void> CL_WritePacketDetour;
void CL_WritePacket()
{
	if (GetStructs() && !ourEnt->isDead())
	{
		XINPUT_STATE State;
		if (FAILED(XInputGetState(0, &State)))
			goto End;

		static Vector3 vOldAngles, vCurrAngles;
		vOldAngles = vCurrAngles;
		vCurrAngles = clientActive->viewAngles;

		if (aimTypeValue)
		{
			if (aimTypeValue == A_ASNAP && State.Gamepad.bLeftTrigger < 30)
				goto End;

			entity_s* target = GetBestTarget();
			if (target != nullptr)
			{
				Vector3 bonePos = GetTagPos(target, "bip01 head");
				if (bonePos.IsZero())
					goto End;
				
				Vector3 eyePos = Vector3(0.0f, 0.0f, 0.0f);
				Sentient_GetEyePosition(ourEnt, &eyePos);

				Vector3 angles;
				CalcAngle(eyePos, bonePos, &angles);
				angles.Normalize();
				angles.ClampAngle();

				if (aimTypeValue)
				{
					if (aimTypeValue == A_SNAP || aimTypeValue == A_SNAP)
					{
						if (!angles.IsZero())
						{
							clientActive->viewAngles.x = angles.x;
							clientActive->viewAngles.y = angles.y;
						}
					}
				}
			}
		}
	}
End:
	CL_WritePacketDetour.CallOriginal();
}

BOOL terminateProcess;
BOOL shutdownTitleStuff;

BOOL haveWeBootedGame;
extern Color mainMenuColor;
MenuClass* Maintab;
MenuClass* GunPosMenu;
MenuClass* Aimtab;
MenuClass* AntiMenu;
MenuClass* Esptab;
MenuClass* Misctab;
MenuClass* Settingstab;
MenuClass* MainMenuColor;
MenuClass* GamebackgroundColor;
MenuClass* EnemiesMainColor;
MenuClass* EnemiesWallColor;
MenuClass* EnemiesVisColor;
MenuClass* AlliesColor;
MenuClass* FriendsColor;
MenuClass* GroundWeapColor;
MenuClass* ThrowableColor;
MenuClass* BulletImpactColor;
MenuClass* TracerColor;
MenuClass* TrailColor;
BOOL InitMenu()
{
	Maintab = new MenuClass;
	GunPosMenu = new MenuClass;
	Aimtab = new MenuClass;
	AntiMenu = new MenuClass;
	Esptab = new MenuClass;
	Playertab = new MenuClass;
	Misctab = new MenuClass;
	Settingstab = new MenuClass;
	MainMenuColor = new MenuClass;
	GamebackgroundColor = new MenuClass;
	EnemiesMainColor = new MenuClass;
	EnemiesWallColor = new MenuClass;
	EnemiesVisColor = new MenuClass;
	AlliesColor = new MenuClass;
	FriendsColor = new MenuClass;
	GroundWeapColor = new MenuClass;
	ThrowableColor = new MenuClass;
	BulletImpactColor = new MenuClass;
	TracerColor = new MenuClass;
	TrailColor = new MenuClass;

	Maintab->SetMenuTitle("Main");
	Maintab->AddStaticItem("No Recoil", &mainNoRecoil, NULL, NULL, "Remove recoil when firing weapons.");
	Maintab->AddStaticItem("No Sway", &mainNoSway, NULL, NULL, "Removes weapon swaying.");
	Maintab->AddStaticItem("No Flinch", &mainNoFlinch, NULL, NULL, "Removes flinch when getting hit.");
	Maintab->AddStaticItem("Small Crosshair", &mainSmallCross, NULL, NULL, "Makes your crosshair tight.");
	Maintab->AddStaticItem("Rapid Fire", &mainRapidFire, NULL, NULL, "Just hold down the trigger to shoot real quick.");
	Maintab->AddStaticItem("Jitter Fire", &mainJitter, NULL, NULL, "Makes burst fire guns full auto.");
	Maintab->AddStaticItem("Fast Reload", &mainFastReload, NULL, NULL, "Auto cancels reload animation for quicker reloads.");
	Maintab->AddStaticItem("Auto Sprint", &mainAutoSprint, NULL, NULL, "Sprints automatically for you.");
	Maintab->AddStaticFloatNumericItem("Field of View", &mainFOV, 65.0f, 120.0f, 1.0f, NULL, "Change your field of view.");
	Maintab->AddStaticItem("End Game", NULL, doEndGame, NULL, "Allows you to end game off host.");
	Maintab->AddStaticItem("Leave Game", NULL, doLeaveGame, NULL, "Disconnect yourself from the current game.");
	Maintab->AddStaticItem("Change Teams", NULL, doSwapTeams, NULL, "Swap to the other team if possible.");
	Maintab->AddStaticIntNumericItem("Change Class", &mainSwapClass, 1, 5, 1, doSwapClasses, "Change your current class (Have to respawn).", false);
	Maintab->AddStaticItem("Third Person", &mainThirdPerson, NULL, NULL, "Change your view to be in third person or first person.");
	Maintab->AddStaticItem("Laser", &mainLaser, NULL, NULL, "Gives your weapons a laser pointer.");
	Maintab->AddStaticStringNumericItem("Camo", &camoSkinValue, camoSkins, 8, NULL, "Changes camo for all weapons. (Not every weapon can change to selected camo.)", false);
	Maintab->AddSubMenu("Gun Position", GunPosMenu, "Modify the position of your guns.");
	Maintab->AddStaticItem("Save Position", NULL, doSavePosition, NULL, "Save your current position for teleporting back to later.");
	Maintab->AddStaticItem("Teleport To Saved Position", NULL, doTelePosition, "Teleport to your saved location.");
	menuTabs.push_back(Maintab);

	GunPosMenu->SetMenuTitle("Gun Position");
	GunPosMenu->AddStaticFloatNumericItem("X Value", &gunXValue, -20.0f, 20.0f, 1.0f, NULL, "Sets the X position of your gun.");
	GunPosMenu->AddStaticFloatNumericItem("Y Value", &gunYValue, -20.0f, 20.0f, 1.0f, NULL, "Sets the Y position of your gun.");
	GunPosMenu->AddStaticFloatNumericItem("Z Value", &gunZValue, -20.0f, 20.0f, 1.0f, NULL, "Sets the Z position of your gun.");

	Aimtab->SetMenuTitle("Aimbot");
	Aimtab->AddStaticStringNumericItem("Aimbot Type", (int*)&aimTypeValue, aimTypeList, 4, NULL, "Change how the aimbot works.");
	Aimtab->AddStaticStringNumericItem("Bone Target", &boneTargetValue, bones, 20, NULL, "Bone we should target.");
	Aimtab->AddStaticItem("Target Best Bone", &aimTargetBestBone, NULL, NULL, "Aimbot will try to target the best possible bone.");
	Aimtab->AddStaticItem("Auto Wall", &aimShouldAutoWall, NULL, NULL, "Aimbot can target players through penetrable walls.");
	Aimtab->AddStaticItem("Closest to Crosshair", &aimClosestCross, NULL, NULL, "Aimbot will target closest to cross hair (Should only be used when legit aimbotting.)");
	Aimtab->AddStaticFloatNumericItem("Auto Wall Damage", &aimWallDamage, 0.01f, 0.9f, 0.01f, NULL, "Amount of damage we check for when auto walling.");
	Aimtab->AddStaticFloatNumericItem("Distance", &aimDistance, 10.0f, 200.0f, 10.0f, NULL, "How far out can we look for targets.");
	Aimtab->AddStaticFloatNumericItem("FOV Range", &aimFOVValue, 0.01f, 1.0f, 0.01f, NULL, "How far away from crosshair can we look for targets. (Max allows targeting behind us.)");
	Aimtab->AddStaticFloatNumericItem("Smooth Snap", &aimSmoothSpeedValue, 0.0f, 20.0f, 1.0f, NULL, "Sets how fast and smooth we snap to targets. (Doesn't affect silent aim.)");
	Aimtab->AddStaticFloatNumericItem("Prediction", &aimPrediction, 0.000f, 0.100f, 0.001f, NULL, "How much we try to predict the players movements.", true);
	Aimtab->AddStaticItem("No Spread", &aimNoSpread, NULL, NULL, "Trys to correct bullet direction.");
	Aimtab->AddStaticItem("Auto Shoot", &aimAutoShoot, NULL, NULL, "Automatically fire your weapon when someone is targetable. (Required for silent aim)");
	Aimtab->AddStaticItem("Invert Teams", &invertTeams, NULL, NULL, "Target players on your team. (Team Switchers)");
	Aimtab->AddSubMenu("Anti Aim Options", AntiMenu, "Toggle options for HvH.");
	menuTabs.push_back(Aimtab);

	AntiMenu->SetMenuTitle("Anti Aim Options");
	AntiMenu->AddStaticItem("Spinbot", &antiSpinBot, NULL, NULL, "Toggle spinbot on or off.");
	AntiMenu->AddStaticStringNumericItem("X Value", (int*)&spinXValue, spinXList, 4, NULL, "Change which direction to be looking.");
	AntiMenu->AddStaticStringNumericItem("Y Value", (int*)&spinYValue, spinYList, 7, NULL, "Change which direction you will be spinning.");
	AntiMenu->AddStaticStringNumericItem("Crouch", &crouchEventType, crouchEventList, 3, NULL, "Change when to auto crouch.");
	AntiMenu->AddStaticItem("Fake Lag", &antiFakeLag, NULL, NULL, "Give yourself lag.");
	AntiMenu->AddStaticIntNumericItem("Fake Lag Amount", &antiFakeLagAmount, 1, 20, 1, NULL, "Amount of frames we don't send data for (To much can cause you to lag out.)");

	Esptab->SetMenuTitle("ESP");
	Esptab->AddStaticStringNumericItem("Shader Type", (int*)&shaderType, ShaderList, 4, NULL, "Change the type of box to draw around players.");
	Esptab->AddStaticStringNumericItem("Team Select", (int*)&espSelectType, ESPSelectList, 3, NULL, "Change which team to draw on screen.");
	Esptab->AddStaticItem("Names", &drawNames, NULL, NULL, "Draw the names of players.");
	Esptab->AddStaticItem("Distance", &drawDistance, NULL, NULL, "Draws a box saying how far away they are from you.");
	//Esptab->AddStaticItem("Aim Debug Info", &drawAimDebug, NULL, NULL, "Draws info about what the aimbot is calculating.");
	menuTabs.push_back(Esptab);

	Playertab->SetMenuTitle("Players");
	for (int i = 0; i < 24; i++)
		Playertab->AddStaticItem("Empty...", NULL, NULL, NULL);
	menuTabs.push_back(Playertab);

	Misctab->SetMenuTitle("Misc");
	Misctab->AddStaticItem("Custom Dvar", NULL, doCustomDvar, NULL, "Execute a custom dvar with the keyboard.");
	Misctab->AddStaticItem("Low Gravity", &hostLowGravity, doLowGravity, NULL, "Sets the game gravity to be low.");
	Misctab->AddStaticItem("Infinite Ammo", &hostInfAmmo, doInfiniteAmmo, NULL, "Sets infinite ammo for all players.");
	Misctab->AddStaticStringNumericItem("Timescale", &hostTimescale, hostTimescaleList, 3, doTimescale, "Changes the timescale for the game.");
	menuTabs.push_back(Misctab);

	Settingstab->SetMenuTitle("Settings");
	Settingstab->AddStaticItem("Server Info", &drawServerInfo, NULL, NULL, "Displays info about the current game and server on right side of screen.");
	Settingstab->AddColorMenu("Menu Color", MainMenuColor, &mainMenuColor);
	Settingstab->AddColorMenu("Background Color", GamebackgroundColor, &gameBackGroundColor);
	Settingstab->AddColorMenu("Enemies Main Color", EnemiesMainColor, &enemiesMainColor);
	Settingstab->AddColorMenu("Enemies Wall Color", EnemiesWallColor, &enemiesWallColor);
	Settingstab->AddColorMenu("Enemies Vis Color", EnemiesVisColor, &enemiesVisColor);
	Settingstab->AddColorMenu("Allies Color", AlliesColor, &alliesColor);
	Settingstab->AddColorMenu("Friends Color", FriendsColor, &friendsColor);
	Settingstab->AddColorMenu("Ground Weapons Color", GroundWeapColor, &groundWeapColor);
	Settingstab->AddColorMenu("Throwables Color", ThrowableColor, &throwableColor);
	Settingstab->AddColorMenu("Bullet Impact Color", BulletImpactColor, &bulletImpactColor);
	Settingstab->AddColorMenu("Tracer Color", TracerColor, &tracerColor);
	Settingstab->AddColorMenu("Trail Color", TrailColor, &trailColor);
	menuTabs.push_back(Settingstab);

	MainMenuColor->SetMenuTitle("Menu Color");
	MainMenuColor->AddStaticColorItem("Red", COLOR_RED);
	MainMenuColor->AddStaticColorItem("Green", COLOR_GREEN);
	MainMenuColor->AddStaticColorItem("Blue", COLOR_BLUE);
	MainMenuColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	MainMenuColor->AddStaticItem("Rainbow", &mainColorRainbow, doMainColorRainbow, NULL, "Makes this color constantly changing colors.");

	GamebackgroundColor->SetMenuTitle("Background Color");
	GamebackgroundColor->AddStaticColorItem("Red", COLOR_RED);
	GamebackgroundColor->AddStaticColorItem("Green", COLOR_GREEN);
	GamebackgroundColor->AddStaticColorItem("Blue", COLOR_BLUE);
	GamebackgroundColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	GamebackgroundColor->AddStaticItem("Rainbow", &backgroundColorRainbow, doBackgroundColorRainbow, NULL, "Makes the games background color constantly changing colors.");
	GamebackgroundColor->AddStaticItem("Enable Background Color", &customBackground, NULL, NULL, "Enables or disables the custom background color.");

	EnemiesMainColor->SetMenuTitle("Enemies Main Color");
	EnemiesMainColor->AddStaticColorItem("Red", COLOR_RED);
	EnemiesMainColor->AddStaticColorItem("Green", COLOR_GREEN);
	EnemiesMainColor->AddStaticColorItem("Blue", COLOR_BLUE);
	EnemiesMainColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	EnemiesMainColor->AddStaticItem("Rainbow", &enemiesMainColorRainbow, doEnemiesMainColorRainbow, NULL, "Makes this color constantly changing colors.");

	EnemiesWallColor->SetMenuTitle("Enemies Wall Color");
	EnemiesWallColor->AddStaticColorItem("Red", COLOR_RED);
	EnemiesWallColor->AddStaticColorItem("Green", COLOR_GREEN);
	EnemiesWallColor->AddStaticColorItem("Blue", COLOR_BLUE);
	EnemiesWallColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	EnemiesWallColor->AddStaticItem("Rainbow", &enemiesWallColorRainbow, doEnemiesWallColorRainbow, NULL, "Makes this color constantly changing colors.");

	EnemiesVisColor->SetMenuTitle("Enemies Visible Color");
	EnemiesVisColor->AddStaticColorItem("Red", COLOR_RED);
	EnemiesVisColor->AddStaticColorItem("Green", COLOR_GREEN);
	EnemiesVisColor->AddStaticColorItem("Blue", COLOR_BLUE);
	EnemiesVisColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	EnemiesVisColor->AddStaticItem("Rainbow", &enemiesVisColorRainbow, doEnemiesVisColorRainbow, NULL, "Makes this color constantly changing colors.");

	AlliesColor->SetMenuTitle("Allies Color");
	AlliesColor->AddStaticColorItem("Red", COLOR_RED);
	AlliesColor->AddStaticColorItem("Green", COLOR_GREEN);
	AlliesColor->AddStaticColorItem("Blue", COLOR_BLUE);
	AlliesColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	AlliesColor->AddStaticItem("Rainbow", &alliesColorRainbow, doAlliesColorRainbow, NULL, "Makes this color constantly changing colors.");

	FriendsColor->SetMenuTitle("Friends Color");
	FriendsColor->AddStaticColorItem("Red", COLOR_RED);
	FriendsColor->AddStaticColorItem("Green", COLOR_GREEN);
	FriendsColor->AddStaticColorItem("Blue", COLOR_BLUE);
	FriendsColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	FriendsColor->AddStaticItem("Rainbow", &friendsColorRainbow, doFriendsColorRainbow, NULL, "Makes this color constantly changing colors.");

	GroundWeapColor->SetMenuTitle("Ground Weapons Color");
	GroundWeapColor->AddStaticColorItem("Red", COLOR_RED);
	GroundWeapColor->AddStaticColorItem("Green", COLOR_GREEN);
	GroundWeapColor->AddStaticColorItem("Blue", COLOR_BLUE);
	GroundWeapColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	GroundWeapColor->AddStaticItem("Rainbow", &groundWeapColorRainbow, doGroundWeapColorRainbow, NULL, "Makes this color constantly changing colors.");

	ThrowableColor->SetMenuTitle("Throwables Color");
	ThrowableColor->AddStaticColorItem("Red", COLOR_RED);
	ThrowableColor->AddStaticColorItem("Green", COLOR_GREEN);
	ThrowableColor->AddStaticColorItem("Blue", COLOR_BLUE);
	ThrowableColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	ThrowableColor->AddStaticItem("Rainbow", &throwableColorRainbow, doThrowableColorRainbow, NULL, "Makes this color constantly changing colors.");

	BulletImpactColor->SetMenuTitle("Bullet Impact Color");
	BulletImpactColor->AddStaticColorItem("Red", COLOR_RED);
	BulletImpactColor->AddStaticColorItem("Green", COLOR_GREEN);
	BulletImpactColor->AddStaticColorItem("Blue", COLOR_BLUE);
	BulletImpactColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	BulletImpactColor->AddStaticItem("Rainbow", &bulletImpactColorRainbow, doBulletImpactColorRainbow, NULL, "Makes this color constantly changing colors.");

	TracerColor->SetMenuTitle("Tracer Color");
	TracerColor->AddStaticColorItem("Red", COLOR_RED);
	TracerColor->AddStaticColorItem("Green", COLOR_GREEN);
	TracerColor->AddStaticColorItem("Blue", COLOR_BLUE);
	TracerColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	TracerColor->AddStaticItem("Rainbow", &tracerColorRainbow, doTracerColorRainbow, NULL, "Makes this color constantly changing colors.");

	TrailColor->SetMenuTitle("Trail Color");
	TrailColor->AddStaticColorItem("Red", COLOR_RED);
	TrailColor->AddStaticColorItem("Green", COLOR_GREEN);
	TrailColor->AddStaticColorItem("Blue", COLOR_BLUE);
	TrailColor->AddStaticColorItem("Alpha", COLOR_ALPHA);
	TrailColor->AddStaticItem("Rainbow", &trailColorRainbow, doTrailColorRainbow, NULL, "Makes this color constantly changing colors.");

	currentTabIndex = 0;

	XamInputGetStateDetour.SetupDetour(0x82000000, MODULE_XAM, 0x191, XamInputGetStateHook);
	RE_EndFrameDetour.SetupDetour(0x823F75E8, RE_EndFrame);
	CG_Draw2DDetour.SetupDetour(0x824CD2F8, CG_Draw2D);
	CL_WritePacketDetour.SetupDetour(0x824EA688, CL_WritePacket);
	
	haveWeBootedGame = TRUE;
	return TRUE;
}

BOOL bIgnoreHttpRequests = FALSE;
void UninitMenu()
{
	if (Maintab == NULL)
		return;

	XamInputGetStateDetour.TakeDownDetour();
	RE_EndFrameDetour.TakeDownDetour();
	CG_Draw2DDetour.TakeDownDetour();
	CL_WritePacketDetour.TakeDownDetour();
	
	delete Maintab;
	delete GunPosMenu;
	delete Aimtab;
	delete AntiMenu;
	delete Esptab;
	delete Playertab;
	delete Misctab;
	delete Settingstab;
	delete MainMenuColor;
	delete GamebackgroundColor;
	delete EnemiesMainColor;
	delete EnemiesWallColor;
	delete EnemiesVisColor;
	delete AlliesColor;
	delete FriendsColor;
	delete GroundWeapColor;
	delete ThrowableColor;
	delete BulletImpactColor;
	delete TracerColor;
	delete TrailColor;

	Maintab = NULL;

	haveWeBootedGame = FALSE;
	shutdownTitleStuff = FALSE;
	bIgnoreHttpRequests = TRUE;
}

HANDLE traceThreadHandle;
BOOL constantLoopRunning;
DWORD WINAPI MainThread(LPVOID)
{
	while (!terminateProcess)
	{
		dwTitleID = XamGetCurrentTitleId();
		if (!shutdownTitleStuff && dwTitleID == 0x415607E1)
		{
			if (!haveWeBootedGame && MmIsAddressValid((PVOID)0x821F2BF8) && *(int*)0x821F2BF8 == 0xEDAD0032)
			{
				if (!InitMenu())
					Sleep(1000);
			}
		}
		else
			UninitMenu();
		Sleep(33);
	}

	UninitMenu();
	constantLoopRunning = FALSE;
	return TRUE;
}

BOOL APIENTRY DllMain( HANDLE hModule, DWORD dwReason, LPVOID lpReserved )
{
	constantLoopRunning = FALSE;
	terminateProcess = FALSE;
	shutdownTitleStuff = FALSE;

	if (dwReason == DLL_PROCESS_DETACH)
	{
		terminateProcess = TRUE;
		shutdownTitleStuff = TRUE;
		while (constantLoopRunning)
			Sleep(33);
		Sleep(500);
		return TRUE;
	}
	if (dwReason == DLL_PROCESS_ATTACH)
		ExCreateThread( 0, 0, 0, 0, MainThread, 0, EX_CREATE_FLAG_CORE5 | EX_CREATE_FLAG_SYSTEM );
	return TRUE;
}