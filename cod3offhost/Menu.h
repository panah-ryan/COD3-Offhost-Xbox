#pragma once
#include "stdafx.h"
#include "Drawing.h"

using namespace std;

class MenuClass;

// Our callback function, when called by the menu it will give the selected index and text for you to use if you need it
typedef void(__cdecl *MenuClassCallback)(int selectedIndex, vector<int> *selectedMenuIndexes, char *itemText, void *functionParameters);

typedef enum _MenuClassType {
	MENU_TYPE_STATIC,
	MENU_TYPE_STATIC_NUMERIC,
	MENU_TYPE_STATIC_SUBMENU
} MenuClassType;

typedef enum _NumericType {
	NUMERIC_INT,
	NUMERIC_FLOAT,
	NUMERIC_STRING,
	NUMERIC_COLOR
} NumericType;

typedef enum _ColorType
{
	COLOR_RED,
	COLOR_GREEN,
	COLOR_BLUE,
	COLOR_ALPHA
} ColorType;

typedef union _NumericValue {
	float* fValue;
	int* iValue;
	byte* bValue;
} NumericValue;

typedef union _NumericValues
{
	float* fValues;
	int* iValues;
	char** cValues;
} NumericValues;

union PlayerValues
{
	int* intValues;
	float* floatValues;
	char** charValues;
	bool* boolValues;
};

union NumValue
{
	int iValue;
	float fValue;
};

typedef struct _MenuClassItem {
	MenuClassType menuItemType;
	char itemText[64];
	MenuClassCallback functionCallback;
	MenuClass *subMenu;
	void *functionParameters;
	bool itemToggleable;
	bool *itemToggled;
	bool useNumeric;
	NumericType numericType;
	NumericValue numericValue;
	NumericValues numericValues;
	bool decimal3;
	int currentNumIndex;
	NumValue numericMin;
	NumValue numericMax;
	NumValue numericInc;
	PlayerValues playerVals;
	char* description;
	bool autoExecute;
} MenuClassItem;

class MenuClass {

public:
	char menuTitle[32];
	vector<MenuClassItem> *menuItems;
	MenuClass *rootMenu;
	MenuClass *currentMenu;
	stack<MenuClass*> *menuStack;
	vector<int> *menuIndexStack;
	int selectedMenuItem;
	bool playerSubMenu;
	int playerIndex;
	Color* color;

	MenuClass();
	~MenuClass();

	// Set the menu title
	void SetMenuTitle(char *Text, ...);

	// Add a submenu, returns the index
	void AddSubMenu(char *displayText, MenuClass *subMenu, char* description = "");

	// Add a submenu for player index
	void AddPlayerMenu(char *displayText, MenuClass *subMenu);

	void AddColorMenu(char* displayText, MenuClass *subMenu, Color* color);

	// Add a static item to the menu
	void AddStaticItem(char *displayText, bool* itemToggleable, MenuClassCallback functionCallback, void *functionParameters, char* description = "");

	// Add a static item to the menu
	void AddStaticPlayerItem(char *displayText, bool* itemToggleable, MenuClassCallback functionCallback, void *functionParameters, char* description = "");

	// Add a static item with a int numeric selector
	void AddStaticIntNumericItem(char *displayText, int* NumericValue, int min, int max, int inc, MenuClassCallback functionCallback, char* description = "", bool autoExecute = true);

	// Add a static item with a float numeric selector
	void AddStaticFloatNumericItem(char *displayText, float* NumericValuePtr, float min, float max, float inc, MenuClassCallback functionCallback, char* description = "", bool decimal3 = false, bool autoExecute = true);

	// Add a static item with a char* numeric selector
	void AddStaticStringNumericItem(char* displayText, int* startingIndex, char** values, int arraySize, MenuClassCallback functionCallback, char* description = "", bool autoExecute = true);

	void AddStaticColorItem(char* displayText, ColorType type, char* description = "");
};

void DisplayMenu(MenuClass* currentTab);