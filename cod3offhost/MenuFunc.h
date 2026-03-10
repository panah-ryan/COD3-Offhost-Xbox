#pragma once
#include <algorithm>

bool drawServerInfo = false;
bool customBackground = false;

bool aimbotFriendList[18];
bool aimbotPriorityList[18];

char* ShaderList[] = { "Off", "2D", "Corner", "Filled" };
enum SHADERTYPE : int
{
	S_OFF,
	S_2D,
	S_CORNER,
	S_FILLED,
};
SHADERTYPE shaderType = S_OFF;

char* ESPSelectList[] = { "All", "Enemies", "Allies" };
enum ESPSELECT : int
{
	E_ALL,
	E_EN,
	E_FR
};
ESPSELECT espSelectType = E_ALL;

bool drawBones = false;
bool drawNames = false;
bool drawWeaponNames = false;
bool drawDistance = false;
bool drawSnapLines = false;
bool drawAimDebug = false;

bool antiSpinBot = false;

char* spinXList[] = { "Off", "Up", "Down", "Jitter" };
enum SPINXTYPE
{
	X_OFF,
	X_UP,
	X_DOWN,
	X_JITTER,
};
SPINXTYPE spinXValue = X_OFF;

char* spinYList[] = { "Off", "Fast", "Slow", "Face Away", "Jitter", "Side", "Static Jitter" };
enum SPINYTYPE
{
	Y_OFF,
	Y_FAST,
	Y_SLOW,
	Y_FACEAWAY,
	Y_JITTER,
	Y_SIDE,
	Y_STATICJITTER,
};
SPINYTYPE spinYValue = Y_OFF;

char* crouchEventList[] = { "Off", "Damage", "Always" };
int crouchEventType = 0;
bool antiFakeLag = false;
int antiFakeLagAmount = 1;

char* aimTypeList[] = { "Off", "Aim Snap", "Snap", "Silent" };
enum AIMTYPESELECT
{
	A_OFF,
	A_ASNAP,
	A_SNAP,
	A_SIL
};
AIMTYPESELECT aimTypeValue = A_OFF;
bool aimShouldAutoWall = false;

char* bones[] =
{
	"j_helmet", "j_mainroot", "j_neck", "j_head", "j_hip_le",
	"j_hip_ri", "j_knee_le", "j_knee_ri", "j_spineupper", "j_ankle_le",
	"j_ankle_ri", "j_shoulder_le", "j_shoulder_ri", "j_elbow_le",
	"j_elbow_ri", "j_jaw", "j_thumb_le_1",
	"j_thumb_ri_1", "j_pinky_le_1", "j_pinky_ri_1",
};
int boneTargetValue = 0;

bool aimTargetBestBone = false;
bool aimClosestCross = false;
float aimWallDamage = 0.1f;
float aimDistance = 10.0f;
float aimFOVValue = 0.01f;
float aimSmoothSpeedValue = 0.0f;
float aimPrediction = 0.0f;
bool aimNoSpread = false;
bool aimAutoShoot = false;

bool mainNoRecoil = false;
bool mainNoSway = false;
bool mainNoFlinch = false;
bool mainSmallCross = false;
bool mainRapidFire = false;
bool mainFastReload = false;
float mainFOV = 65.0f;
bool mainThirdPerson = false;
bool mainLaser = false;
bool mainAutoSprint = false;
bool mainJitter = false;

bool invertTeams = false;

char* camoSkins[] =
{
	"Default", "None", "Desert", "Woodland", "Digital", "Red Tiger", "Blue Tiger", "Golden"
};
int camoSkinValue = 0;

float gunXValue = 0.0f;
float gunYValue = 0.0f;
float gunZValue = 0.0f;

vector<Color*> rainbowColorList;
extern Color mainMenuColor;
bool mainColorRainbow = false;
void doMainColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (mainColorRainbow)
	{
		mainMenuColor.Color_.R = 255;
		mainMenuColor.Color_.G = 0;
		mainMenuColor.Color_.B = 0;
		rainbowColorList.push_back(&mainMenuColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &mainMenuColor);
		rainbowColorList.erase(it);
	}
}

Color gameBackGroundColor = Color(255, 0, 0, 255);
bool backgroundColorRainbow = false;
void doBackgroundColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (backgroundColorRainbow)
	{
		gameBackGroundColor.Color_.R = 255;
		gameBackGroundColor.Color_.G = 0;
		gameBackGroundColor.Color_.B = 0;
		rainbowColorList.push_back(&gameBackGroundColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &gameBackGroundColor);
		rainbowColorList.erase(it);
	}
}

Color alliesColor = Color(0, 0, 255, 255);
bool alliesColorRainbow = false;
void doAlliesColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (alliesColorRainbow)
	{
		alliesColor.Color_.R = 255;
		alliesColor.Color_.G = 0;
		alliesColor.Color_.B = 0;
		rainbowColorList.push_back(&alliesColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &alliesColor);
		rainbowColorList.erase(it);
	}
}

Color friendsColor = Color(255, 255, 255, 255);
bool friendsColorRainbow = false;
void doFriendsColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (friendsColorRainbow)
	{
		friendsColor.Color_.R = 255;
		friendsColor.Color_.G = 0;
		friendsColor.Color_.B = 0;
		rainbowColorList.push_back(&friendsColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &friendsColor);
		rainbowColorList.erase(it);
	}
}

Color enemiesMainColor = Color(255, 0, 0, 255);
bool enemiesMainColorRainbow = false;
void doEnemiesMainColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (enemiesMainColorRainbow)
	{
		enemiesMainColor.Color_.R = 255;
		enemiesMainColor.Color_.G = 0;
		enemiesMainColor.Color_.B = 0;
		rainbowColorList.push_back(&enemiesMainColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &enemiesMainColor);
		rainbowColorList.erase(it);
	}
}

Color enemiesWallColor = Color(255, 255, 0, 255);
bool enemiesWallColorRainbow = false;
void doEnemiesWallColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (enemiesWallColorRainbow)
	{
		enemiesWallColor.Color_.R = 255;
		enemiesWallColor.Color_.G = 0;
		enemiesWallColor.Color_.B = 0;
		rainbowColorList.push_back(&enemiesWallColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &enemiesWallColor);
		rainbowColorList.erase(it);
	}
}

Color enemiesVisColor = Color(0, 255, 0, 255);
bool enemiesVisColorRainbow = false;
void doEnemiesVisColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (enemiesVisColorRainbow)
	{
		enemiesVisColor.Color_.R = 255;
		enemiesVisColor.Color_.G = 0;
		enemiesVisColor.Color_.B = 0;
		rainbowColorList.push_back(&enemiesVisColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &enemiesVisColor);
		rainbowColorList.erase(it);
	}
}

Color groundWeapColor = Color(255, 255, 255, 255);
bool groundWeapColorRainbow = false;
void doGroundWeapColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (groundWeapColorRainbow)
	{
		groundWeapColor.Color_.R = 255;
		groundWeapColor.Color_.G = 0;
		groundWeapColor.Color_.B = 0;
		rainbowColorList.push_back(&groundWeapColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &groundWeapColor);
		rainbowColorList.erase(it);
	}
}

Color throwableColor = Color(255, 255, 255, 255);
bool throwableColorRainbow = false;
void doThrowableColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (throwableColorRainbow)
	{
		throwableColor.Color_.R = 255;
		throwableColor.Color_.G = 0;
		throwableColor.Color_.B = 0;
		rainbowColorList.push_back(&throwableColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &throwableColor);
		rainbowColorList.erase(it);
	}
}

Color bulletImpactColor = Color(255, 255, 255, 255);
bool bulletImpactColorRainbow = false;
void doBulletImpactColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (bulletImpactColorRainbow)
	{
		bulletImpactColor.Color_.R = 255;
		bulletImpactColor.Color_.G = 0;
		bulletImpactColor.Color_.B = 0;
		rainbowColorList.push_back(&bulletImpactColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &bulletImpactColor);
		rainbowColorList.erase(it);
	}
}

Color tracerColor = Color(255, 255, 255, 255);
bool tracerColorRainbow = false;
void doTracerColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (tracerColorRainbow)
	{
		tracerColor.Color_.R = 255;
		tracerColor.Color_.G = 0;
		tracerColor.Color_.B = 0;
		rainbowColorList.push_back(&tracerColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &tracerColor);
		rainbowColorList.erase(it);
	}
}

Color triggerColor = Color(255, 0, 0, 64);
bool triggerColorRainbow = false;
void doTriggerColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (triggerColorRainbow)
	{
		triggerColor.Color_.R = 255;
		triggerColor.Color_.G = 0;
		triggerColor.Color_.B = 0;
		rainbowColorList.push_back(&triggerColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &triggerColor);
		rainbowColorList.erase(it);
	}
}

Color trailColor = Color(255, 0, 0, 64);
bool trailColorRainbow = false;
void doTrailColorRainbow(int selectedIndex, vector<int> *selectedMenuIndexes, char itemText[32], void *functionParameters)
{
	if (trailColorRainbow)
	{
		trailColor.Color_.R = 255;
		trailColor.Color_.G = 0;
		trailColor.Color_.B = 0;
		rainbowColorList.push_back(&trailColor);
	}
	else
	{
		vector<Color*>::iterator it = find(rainbowColorList.begin(), rainbowColorList.end(), &trailColor);
		rainbowColorList.erase(it);
	}
}

bool hostLowGravity = false;
void doLowGravity(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	Cbuf_ExecuteText(va("g_gravity %i", hostLowGravity ? 200 : 800));
}

bool hostInfAmmo = false;
void doInfiniteAmmo(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	*(DWORD*)0x824533A4 = hostInfAmmo ? 0x60000000 : 0x7D2B512E;
}

char* hostTimescaleList[] = { "Normal", "Slow", "Fast" };
int hostTimescale = 0;
void doTimescale(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	char* timescaleValue = (char*)*(int*)functionParamters;
	float timescaleFloat = 1.0f;

	if (strcmp(timescaleValue, "Slow") == 0)
		timescaleFloat = 0.3f;
	else if (strcmp(timescaleValue, "Fast") == 0)
		timescaleFloat = 2.0f;

	Cbuf_ExecuteText(va("timescale %.1f", timescaleFloat));
}

void doDvarThread()
{
	int error;
	char* dvar = XamShowKeyboardUI(L"Custom Cvar", L"Enter a cvar", &error);
	if (error == 0 && dvar[0])
		Cbuf_ExecuteText(dvar);
}

void doCustomDvar(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	ExCreateThread(0, 0, 0, 0, (LPTHREAD_START_ROUTINE)doDvarThread, 0, 0);
}

void doEndGame(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	((void(*)(bool, bool))0x823C0268)(true, true);
}

void doLeaveGame(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	((void(*)(int, char*, char*))0x824F6AB0)(*(int*)0x82AFFCE8, "%s", "cp \"agga test\"");
}

const char* GetTeamName(int team)
{
	if (team == 1)
		return "axis";
	else if (team == 2)
		return "allies";
	else if (team == 3)
		return "spectator";
	return "null";
}

void doSwapTeams(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	
}

int mainSwapClass = 1;
void doSwapClasses(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	
}

Vector3 savePosition;
void doSavePosition(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	//savePosition = Centity_s[cgArray_s->ClientNumber].Origin;
}

void doTelePosition(int selectedIndex, vector<int>* selectedMenuIndexes, char itemText[32], void* functionParamters)
{
	
}