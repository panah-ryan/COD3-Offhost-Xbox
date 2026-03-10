#include "Menu.h"

int currentTabIndex = 0;
vector<MenuClass*> menuTabs;
float menuX = 640 / 2;
float menuY = 480 / 2;
float optionHeight = 15.3f;
int refDefWidth;
int refDefHeight;
extern float aimFOVValue;
extern int matWhite;

Color mainMenuColor = Color(235, 235, 50, 255); //Yellow

bool MenuOpen = false;
#define MaxOptions 20

// Constructor
MenuClass::MenuClass() {
	//inputHandler = new Input;
	menuItems = new vector<MenuClassItem>;
	menuStack = new stack<MenuClass*>;
	menuIndexStack = new vector<int>;
	rootMenu = this;
	currentMenu = rootMenu;
	playerSubMenu = false;
	playerIndex = -1;
	color = NULL;
}

// Destructor
MenuClass::~MenuClass() {
	delete menuItems;
	delete menuStack;
	delete menuIndexStack;
}

// Set the menu title
void MenuClass::SetMenuTitle(char *Text, ...) {

	va_list pArgList;
	va_start(pArgList, Text);

	// Clear out the title memory
	memset(menuTitle, 0, 32);

	// Copy the display text in, safely
	vsnprintf(menuTitle, 31, Text, pArgList);

	va_end(pArgList);
}

// Add a submenu
void MenuClass::AddSubMenu(char *displayText, MenuClass *subMenu, char* description) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_SUBMENU;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// No function callback
	menuItem.functionCallback = NULL;

	// Sub menu
	menuItem.subMenu = subMenu;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = 0;
	menuItem.description = description;

	// Add our menu item
	menuItems->push_back(menuItem);
}

// Add a submenu
void MenuClass::AddPlayerMenu(char *displayText, MenuClass *subMenu) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_SUBMENU;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// No function callback
	menuItem.functionCallback = NULL;

	// Sub menu
	menuItem.subMenu = subMenu;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = 0;
	menuItem.description = "";
	menuItem.subMenu->playerSubMenu = true;

	// Add our menu item
	menuItems->push_back(menuItem);
}

void MenuClass::AddColorMenu(char* displayText, MenuClass *subMenu, Color* color)
{
	MenuClassItem menuItem;
	menuItem.menuItemType = MENU_TYPE_STATIC_SUBMENU;
	strncpy(menuItem.itemText, displayText, 63);
	menuItem.functionCallback = NULL;
	menuItem.subMenu = subMenu;
	menuItem.functionParameters = NULL;
	menuItem.itemToggleable = false;
	menuItem.itemToggled = 0;
	menuItem.description = "";
	menuItem.subMenu->color = color;
	menuItems->push_back(menuItem);
}

// Add a static item to the menu
void MenuClass::AddStaticItem(char *displayText, bool* itemToggleable, MenuClassCallback functionCallback, void *functionParameters, char* description) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = functionCallback;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = functionParameters;

	// Menu item toggling
	menuItem.itemToggleable = (int)itemToggleable != 0;
	menuItem.itemToggled = itemToggleable;

	// Menu numeric
	menuItem.useNumeric = false;
	menuItem.numericMin.iValue = 0;
	menuItem.numericMax.iValue = 0;
	menuItem.description = description;

	// Add our menu item
	menuItems->push_back(menuItem);
}

// Add a static item to the menu
void MenuClass::AddStaticPlayerItem(char *displayText, bool* itemToggleable, MenuClassCallback functionCallback, void *functionParameters, char* description) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = functionCallback;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = functionParameters;

	// Menu item toggling
	menuItem.itemToggleable = true;
	//menuItem.itemToggled = itemToggleable;

	// Menu numeric
	menuItem.useNumeric = false;
	menuItem.numericMin.iValue = 0;
	menuItem.numericMax.iValue = 0;
	menuItem.description = description;
	menuItem.playerVals.boolValues = itemToggleable;

	// Add our menu item
	menuItems->push_back(menuItem);
}

// Add a static item with a int numeric selector
void MenuClass::AddStaticIntNumericItem(char *displayText, int* value, int min, int max, int inc, MenuClassCallback functionCallback, char* description, bool autoExecute) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_NUMERIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = functionCallback;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = NULL;

	// Menu numeric
	menuItem.numericType = NUMERIC_INT;
	menuItem.useNumeric = true;
	menuItem.numericValue.iValue = value;
	menuItem.numericMin.iValue = min;
	menuItem.numericMax.iValue = max;
	menuItem.numericInc.iValue = inc;
	menuItem.description = description;
	menuItem.autoExecute = autoExecute;

	// Add our menu item
	menuItems->push_back(menuItem);
}

// Add a static item with a float numeric selector
void MenuClass::AddStaticFloatNumericItem(char *displayText, float* value, float min, float max, float inc, MenuClassCallback functionCallback, char* description, bool decimal3, bool autoExecute) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_NUMERIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = functionCallback;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = NULL;

	// Menu numeric
	menuItem.useNumeric = true;
	menuItem.numericMin.fValue = min;
	menuItem.numericMax.fValue = max;
	menuItem.numericType = NUMERIC_FLOAT;
	menuItem.numericValue.fValue = value;
	menuItem.numericInc.fValue = inc;
	menuItem.description = description;
	menuItem.autoExecute = autoExecute;
	menuItem.decimal3 = decimal3;

	// Add our menu item
	menuItems->push_back(menuItem);
}

// Add a static item with a char* numeric selector
void MenuClass::AddStaticStringNumericItem(char *displayText, int* startingIndex, char** values, int arraySize, MenuClassCallback functionCallback, char* description, bool autoExecute) {

	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_NUMERIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = functionCallback;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = NULL;

	// Menu numeric
	menuItem.useNumeric = true;
	menuItem.numericValues.cValues = values;
	menuItem.numericMin.iValue = 0;
	menuItem.numericMax.iValue = arraySize;
	menuItem.numericType = NUMERIC_STRING;
	menuItem.numericValue.iValue = startingIndex;
	menuItem.currentNumIndex = *startingIndex;
	menuItem.description = description;
	menuItem.autoExecute = autoExecute;

	// Add our menu item
	menuItems->push_back(menuItem);
}

void MenuClass::AddStaticColorItem(char* displayText, ColorType type, char* description)
{
	MenuClassItem menuItem;

	// Set menu type
	menuItem.menuItemType = MENU_TYPE_STATIC_NUMERIC;

	// Copy menu item text
	strncpy(menuItem.itemText, displayText, 63);

	// Function callback
	menuItem.functionCallback = NULL;

	// Sub menu
	menuItem.subMenu = NULL;

	// Function params
	menuItem.functionParameters = NULL;

	// Menu item toggling
	menuItem.itemToggleable = false;
	menuItem.itemToggled = NULL;

	// Menu numeric
	menuItem.useNumeric = true;
	switch (type)
	{
	case COLOR_RED:
		menuItem.numericValue.bValue = &this->color->Color_.R;
		break;
	case COLOR_GREEN:
		menuItem.numericValue.bValue = &this->color->Color_.G;
		break;
	case COLOR_BLUE:
		menuItem.numericValue.bValue = &this->color->Color_.B;
		break;
	case COLOR_ALPHA:
		menuItem.numericValue.bValue = &this->color->Color_.A;
		break;
	}
	menuItem.numericMin.iValue = 0;
	menuItem.numericMax.iValue = 255;
	menuItem.numericType = NUMERIC_COLOR;
	menuItem.description = description;

	// Add our menu item
	menuItems->push_back(menuItem);
}

XINPUT_STATE State;

BOOL IsAutoScrollActive(int* holdCounter, int xinput)
{
	BOOL isControlPressed = State.Gamepad.wButtons & xinput;
	if (isControlPressed)
		(*holdCounter)++;
	else
	{
		*holdCounter = 0;
		return false;
	}
	if (*holdCounter <= 30)
	{
		if (*holdCounter < 10)
			return false;
		switch (*holdCounter)
		{
		case 10:
		case 15:
		case 20:
		case 24:
		case 27:
		case 29:
			return isControlPressed;
		default:
			return false;
		}
	}
	return isControlPressed;
}

int descLines;
extern Fonts fonts;
int OptionIndex[2] = { 0, 0 };
bool bRBButtonPressed = false;
bool bLBButtonPressed = false;
bool bXButtonPressed = false;
bool bButtonPressed = false;
bool bDpadUp = false;
bool bDpadDown = false;
bool bDpadLeft = false;
bool bDpadRight = false;
DWORD dwScrollTick;
#define MOVE_TIME 100 // 0.1 second(s)
void DisplayMenu(MenuClass* currentTab)
{
	MenuClass* tab = currentTab;

	if (!tab->currentMenu)
	{
		tab->currentMenu = tab->rootMenu;
		return;
	}

	static int HoldUpCounter = 0, HoldDownCounter = 0, HoldLeftCounter = 0, HoldRightCounter = 0;

	XInputGetState(0, &State);

	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && !bRBButtonPressed)
	{
		int currentIndex = currentTabIndex;
		currentIndex++;
		if (currentIndex >= menuTabs.size())
			currentIndex = 0;
		tab = menuTabs.at(currentIndex);
		if (OptionIndex[0] > tab->currentMenu->menuItems->size() || OptionIndex[1] > tab->currentMenu->menuItems->size()
			|| OptionIndex[0] < 0 || OptionIndex[1] < 0)
		{
			tab->selectedMenuItem = 0;
			OptionIndex[0] = 0;
			OptionIndex[1] = 0;
		}
		currentTabIndex = currentIndex;
		bRBButtonPressed = true;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER))
		bRBButtonPressed = false;

	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER && !bLBButtonPressed)
	{
		int currentIndex = currentTabIndex;
		currentIndex--;
		if (currentIndex < 0)
			currentIndex = menuTabs.size() - 1;
		tab = menuTabs.at(currentIndex);
		if (OptionIndex[0] > tab->currentMenu->menuItems->size() || OptionIndex[1] > tab->currentMenu->menuItems->size()
			|| OptionIndex[0] < 0 || OptionIndex[1] < 0)
		{
			tab->selectedMenuItem = 0;
			OptionIndex[0] = 0;
			OptionIndex[1] = 0;
		}
		currentTabIndex = currentIndex;
		bLBButtonPressed = true;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER))
		bLBButtonPressed = false;

	// Handle X input
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_X && !bXButtonPressed) {
		// Get our current selected menu item
		MenuClassItem *menuItem = &tab->currentMenu->menuItems->operator[](tab->selectedMenuItem);

		// Handle our a pressed event
		switch (menuItem->menuItemType) {


		case MENU_TYPE_STATIC:
		{
			// Toggle our menu item if its toggleable
			if (menuItem->itemToggleable && !tab->currentMenu->playerSubMenu)
				menuItem->itemToggled[0] = !menuItem->itemToggled[0];
			else if (menuItem->itemToggleable && tab->currentMenu->playerSubMenu)
				menuItem->playerVals.boolValues[tab->currentMenu->playerIndex] = !menuItem->playerVals.boolValues[tab->currentMenu->playerIndex];

			// Checking to see if we added a callback
			if (menuItem->functionCallback)
				menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, (menuItem->functionParameters) ? menuItem->functionParameters : &menuItem->itemToggleable);
			break;
		}
		case MENU_TYPE_STATIC_SUBMENU:
		{
			// Checking if submenu is null
			if (menuItem->subMenu) {
				// Checking if its null
				if (tab->menuStack) {
					// Save our current place
					tab->menuStack->push(tab->currentMenu);
				}

				tab->currentMenu = menuItem->subMenu;

				// Set our sub menu index so we can read it
				tab->menuIndexStack->push_back(tab->selectedMenuItem);
				if (menuItem->subMenu->playerSubMenu)
					menuItem->subMenu->playerIndex = tab->selectedMenuItem;

				// change the menu title to what we selected
				strncpy(tab->currentMenu->menuTitle, menuItem->itemText, 32);
				OptionIndex[0] = 0;
				OptionIndex[1] = 0;

				// reset our item number
				tab->selectedMenuItem = 0;
			}

			break;
		}
		case MENU_TYPE_STATIC_NUMERIC:
		{
			if(menuItem->numericType == NUMERIC_FLOAT)
			{
				if (menuItem->functionCallback)
					menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.fValue);
			}
			else if (menuItem->numericType == NUMERIC_INT)
			{
				if (menuItem->functionCallback)
					menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.iValue);
			}
			else if (menuItem->numericType == NUMERIC_STRING)
			{
				if (menuItem->functionCallback)
					menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, &menuItem->numericValues.cValues[menuItem->currentNumIndex]);
			}
			break;
		}
		default:
		{
			break;
		}
		}

		bXButtonPressed = true;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_X)) {
		bXButtonPressed = false;
	}

	// Handle b input
	if (State.Gamepad.wButtons & XINPUT_GAMEPAD_B && !bButtonPressed) {

		// Checking if its null
		if (tab->menuStack) {

			// Check if we are at the root menu and make sure the menu stack is not empty
			if (tab->currentMenu != tab->rootMenu && !tab->menuStack->empty()) {

				// Pop it off the stack and set it to the current menu
				tab->currentMenu = tab->menuStack->top();
				tab->selectedMenuItem = 0;
				OptionIndex[0] = 0;
				OptionIndex[1] = 0;

				tab->menuStack->pop();

				tab->menuIndexStack->pop_back();
			}
			else if (tab->menuStack->empty() && tab->currentMenu == tab->rootMenu)
				MenuOpen = false;
		}

		bButtonPressed = true;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_B)) {
		bButtonPressed = false;
	}

	// Handle Dpad Up input
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP && !bDpadUp) || IsAutoScrollActive(&HoldUpCounter, XINPUT_GAMEPAD_DPAD_UP)) {

		if ((GetTickCount() - dwScrollTick) >= MOVE_TIME)
		{
			int Index = tab->currentMenu->menuItems->size() <= MaxOptions ? tab->selectedMenuItem : OptionIndex[1];
			// Subtract from our item
			tab->selectedMenuItem--;

			// make sure it does not go negative
			if (tab->selectedMenuItem < 0) {
				tab->selectedMenuItem = tab->currentMenu->menuItems->size() - 1;
				if (tab->currentMenu->menuItems->size() > MaxOptions)
				{
					OptionIndex[0] = tab->selectedMenuItem - (MaxOptions - 1);
					OptionIndex[1] = MaxOptions - 1;
				}
			}
			else if (tab->currentMenu->menuItems->size() > MaxOptions) {
				if (OptionIndex[1] == 0)
					OptionIndex[0]--;
				else
					OptionIndex[1]--;
			}

			dwScrollTick = GetTickCount();

			bDpadUp = TRUE;
		}
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)) {
		bDpadUp = FALSE;
	}

	// Handle Dpad Down input
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN && !bDpadDown) || IsAutoScrollActive(&HoldDownCounter, XINPUT_GAMEPAD_DPAD_DOWN)) {

		if ((GetTickCount() - dwScrollTick) >= MOVE_TIME)
		{
			int Index = tab->currentMenu->menuItems->size() <= MaxOptions ? tab->selectedMenuItem : OptionIndex[1];

			tab->selectedMenuItem = (tab->selectedMenuItem + 1) % tab->currentMenu->menuItems->size();
			if (tab->currentMenu->menuItems->size() > MaxOptions)
			{
				if (tab->selectedMenuItem == 0)
				{
					OptionIndex[0] = 0;
					OptionIndex[1] = 0;
				}
				else
				{
					if (OptionIndex[1] >= (MaxOptions - 1))
						OptionIndex[0]++;
					else
						OptionIndex[1]++;
				}
			}

			dwScrollTick = GetTickCount();

			bDpadDown = TRUE;
		}
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)) {
		bDpadDown = FALSE;
	}

	// Handle Dpad Left input
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT && !bDpadLeft) || IsAutoScrollActive(&HoldLeftCounter, XINPUT_GAMEPAD_DPAD_LEFT)) {

		// Get our current selected menu item
		MenuClassItem *menuItem = &tab->currentMenu->menuItems->operator[](tab->selectedMenuItem);

		// Handle our a pressed event
		switch (menuItem->menuItemType) {

		case MENU_TYPE_STATIC_NUMERIC:
		{
			if (menuItem->useNumeric)
			{

				if (menuItem->numericType == NUMERIC_FLOAT)
				{
					*menuItem->numericValue.fValue -= menuItem->numericInc.fValue;
					if (*menuItem->numericValue.fValue < menuItem->numericMin.fValue)
						*menuItem->numericValue.fValue = menuItem->numericMax.fValue;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.fValue);
				}

				if (menuItem->numericType == NUMERIC_INT)
				{
					*menuItem->numericValue.iValue -= menuItem->numericInc.iValue;
					if (*menuItem->numericValue.iValue < menuItem->numericMin.iValue)
						*menuItem->numericValue.iValue = menuItem->numericMax.iValue;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.iValue);
				}

				if (menuItem->numericType == NUMERIC_STRING)
				{
					menuItem->currentNumIndex--;
					if (menuItem->currentNumIndex < menuItem->numericMin.iValue)
						menuItem->currentNumIndex = (menuItem->numericMax.iValue - 1);
					*menuItem->numericValue.iValue = menuItem->currentNumIndex;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, &menuItem->numericValues.cValues[menuItem->currentNumIndex]);
				}

				if (menuItem->numericType == NUMERIC_COLOR)
				{
					*menuItem->numericValue.bValue -= 1;
					if (*menuItem->numericValue.bValue <= -1)
						*menuItem->numericValue.bValue = 255;
				}
			}
			break;
		}
		}

		bDpadLeft = TRUE;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT)) {
		bDpadLeft = FALSE;
	}

	// Handle Dpad Right input
	if ((State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT && !bDpadRight) || IsAutoScrollActive(&HoldRightCounter, XINPUT_GAMEPAD_DPAD_RIGHT)) {

		// Get our current selected menu item
		MenuClassItem *menuItem = &tab->currentMenu->menuItems->operator[](tab->selectedMenuItem);

		// Handle our a pressed event
		switch (menuItem->menuItemType) {

		case MENU_TYPE_STATIC_NUMERIC:
		{
			if (menuItem->useNumeric)
			{
				if (menuItem->numericType == NUMERIC_FLOAT)
				{
					*menuItem->numericValue.fValue += menuItem->numericInc.fValue;
					if (*menuItem->numericValue.fValue > menuItem->numericMax.fValue)
						*menuItem->numericValue.fValue = menuItem->numericMin.fValue;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.fValue);
				}

				if (menuItem->numericType == NUMERIC_INT)
				{
					*menuItem->numericValue.iValue += menuItem->numericInc.iValue;
					if (*menuItem->numericValue.iValue > menuItem->numericMax.iValue)
						*menuItem->numericValue.iValue = menuItem->numericMin.iValue;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, menuItem->numericValue.iValue);
				}

				if (menuItem->numericType == NUMERIC_STRING)
				{
					menuItem->currentNumIndex++;
					if (menuItem->currentNumIndex >= menuItem->numericMax.iValue)
						menuItem->currentNumIndex = menuItem->numericMin.iValue;
					*menuItem->numericValue.iValue = menuItem->currentNumIndex;
					if (menuItem->functionCallback && menuItem->autoExecute)
						menuItem->functionCallback(tab->selectedMenuItem, tab->menuIndexStack, menuItem->itemText, &menuItem->numericValues.cValues[menuItem->currentNumIndex]);
				}

				if (menuItem->numericType == NUMERIC_COLOR)
				{
					*menuItem->numericValue.bValue += 1;
					if (*menuItem->numericValue.bValue >= 256)
						*menuItem->numericValue.fValue = 0;
				}
			}
			break;
		}
		}

		bDpadRight = TRUE;
	}
	else if (!(State.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT)) {
		bDpadRight = FALSE;
	}

	float width = 212.5f;
	//Draw Header
	DrawOutlinedRect(menuX - (width / 2), menuY - 159.0f, width, 20, 1, Color(0, 0, 0, 150), mainMenuColor);
	DrawString(tab->currentMenu->menuTitle, menuX, 99.3f, fonts.normalFont, 0.45f, Color(0xFFFFFFFF), false, false, JustifyCenter);

	//Draw Tabs
	DrawRect(menuX - (width / 2), menuY - 159.0f + 20 + 1, width, 15.3f, JustifyLeft, Color(0, 0, 0, 150));

	float tabSpace = width / menuTabs.size();
	DrawRect(menuX - (width / 2) + (tabSpace * currentTabIndex), menuY - 159.0f + 20 + 1, tabSpace, 15.3f, JustifyLeft, mainMenuColor);

	for (int i = 0; i < menuTabs.size(); i++)
		DrawString(menuTabs.at(i)->rootMenu->menuTitle, 191.0f + (tabSpace * (i + 1)) + 5.0f, 115.3f, fonts.normalFont, 0.3f, Color(0xFFFFFFFF), false, false, JustifyCenter);

	for (int i = 0; i < (int)min(MaxOptions, tab->currentMenu->menuItems->size()); i++)
	{
		int Index = tab->currentMenu->menuItems->size() <= MaxOptions ? tab->selectedMenuItem : OptionIndex[1];
		MenuClassItem *menuItem = &tab->currentMenu->menuItems->operator[](OptionIndex[0] + i);

		switch (menuItem->menuItemType)
		{
		case MENU_TYPE_STATIC:
		case MENU_TYPE_STATIC_NUMERIC:
		case MENU_TYPE_STATIC_SUBMENU:
		{
			DrawRect(menuX - (width / 2), menuY - 159.0f + 20 + 16.3f + (i * optionHeight), width, optionHeight, JustifyLeft, i == Index ? mainMenuColor : Color(0, 0, 0, 130));
			DrawString(menuItem->itemText, 218, 130.0f + (i * optionHeight), fonts.normalFont, 0.3f, Color(0xFFFFFFFF), false, false, JustifyLeft);

			if (menuItem->subMenu != NULL)
				DrawString(">>", 421.5f, 132.0f + (i * optionHeight), fonts.normalFont, 0.4f, Color(0xFFFFFFFF), false, false, JustifyRight);
			else if (menuItem->itemToggleable && !menuItem->useNumeric)
			{
				bool toggled = (!tab->currentMenu->playerSubMenu && menuItem->itemToggled[0]) || (tab->currentMenu->playerSubMenu && menuItem->playerVals.boolValues[tab->currentMenu->playerIndex]);
				DrawOutlinedRect(413, 120.6f + (i * optionHeight), 6.5, 8.6, 1, toggled ? mainMenuColor : Color(0x38383800), Color(255, 255, 255, 255), true);
			}
			else if (!menuItem->itemToggleable && menuItem->useNumeric)
			{
				if (menuItem->numericType == NUMERIC_INT)
				{
					char buffer[10];
					sprintf(buffer, "< %i >", *menuItem->numericValue.iValue);
					//DrawOutlinedRect(413 - 43, 120.6f + (i * optionHeight), 48, 8.6f, 1, Color(0x38383800), Color(255, 255, 255, 255), true);
					//DrawRect(413 - 43, 120.06f + (i * optionHeight), (float)(46.0f / (float)(menuItem->numericMax.iValue - menuItem->numericMin.iValue)) * (*menuItem->numericValue.iValue - menuItem->numericMin.iValue), 6.6f, JustifyLeft, mainMenuColor);
					DrawString(buffer, 421.5f, 132.0f + (i * optionHeight), fonts.normalFont, 0.4f, Color(0xFFFFFFFF), false, false, JustifyRight);
				}
				else if (menuItem->numericType == NUMERIC_FLOAT)
				{
					char buffer[10];
					sprintf(buffer, menuItem->decimal3 ? "< %.3f >" : "< %.2f >", *menuItem->numericValue.fValue);
					//DrawOutlinedRect(825 - 86, 178.0f + (i * optionHeight), 96, 13, 1, Color(0x383838FF), Color(255, 255, 255, 255), true);
					//DrawRect(825 - 86, 178.0f + (i * optionHeight), (float)(96.0f / (float)(menuItem->numericMax.fValue - menuItem->numericMin.fValue)) * (*menuItem->numericValue.fValue - menuItem->numericMin.fValue), 13, JustifyLeft, mainMenuColor);
					DrawString(buffer, 421.0f, 132.0f + (i * optionHeight), fonts.normalFont, 0.4f, Color(0xFFFFFFFF), false, false, JustifyRight);
				}
				else if (menuItem->numericType == NUMERIC_STRING)
				{
					char buffer[50];
					sprintf(buffer, "< %s >", menuItem->numericValues.cValues[menuItem->currentNumIndex]);
					DrawString(buffer, 421.0f, 132.0f + (i * optionHeight), fonts.normalFont, 0.4f, Color(0xFFFFFFFF), false, false, JustifyRight);
				}
				else if (menuItem->numericType == NUMERIC_COLOR)
				{
					char buffer[10];
					sprintf(buffer, "< %i> ", *menuItem->numericValue.bValue);
					//DrawOutlinedRect(825 - 86, 178.0f + (i * optionHeight), 96, 13, 1, Color(0x383838FF), Color(255, 255, 255, 255), true);
					//DrawRect(825 - 86, 178.0f + (i * optionHeight), (float)(96.0f / 254) * *menuItem->numericValue.bValue, 13, JustifyLeft, *tab->currentMenu->color);
					DrawString(buffer, 421.0f, 132.0f + (i * optionHeight), fonts.normalFont, 0.4f, Color(0xFFFFFFFF), false, false, JustifyRight);
				}
			}
			break;
		}
		default:
			break;
		}

		if (i == Index && strcmp(menuItem->itemText, "FOV Range") == 0)
		{
			float radius = tanf(aimFOVValue) / tanf(65.0f / 2) * refDefWidth;
			DrawCircle(refDefWidth / 2, refDefHeight / 2, 20, radius, mainMenuColor);
		}
	}

	MenuClassItem* menuItem = &tab->currentMenu->menuItems->operator[](tab->selectedMenuItem);
	float y = 113.3f + (min(MaxOptions, tab->currentMenu->menuItems->size()) * optionHeight) + 4.6f;

	if (strlen(menuItem->description) == 0)
		DrawOutlinedRect(menuX - (width / 2), y, width, 20, 1, Color(0, 0, 0, 130), mainMenuColor);
	else
	{
		char* tip = WordWrap(menuItem->description, fonts.normalFont, 0.5f, width - 25, &descLines);
		DrawOutlinedRect(menuX - (width / 2), y, width, descLines == 1 ? optionHeight * 1.35f : descLines * optionHeight, 1, Color(0, 0, 0, 150), mainMenuColor);
		DrawString(tip, menuX - (width / 2) + 8, descLines == 1 ? y +  17.0f : y + 29.0f, fonts.normalFont, 0.35f, Color(0xFFFFFFFF), false, false, JustifyLeft);
	}
}