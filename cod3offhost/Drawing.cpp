#include "Drawing.h"
#include "Engine.h"
#include "stdafx.h"
#include <algorithm>

using namespace std;

int matWhite;
Fonts fonts;
extern Color mainMenuColor;

void DrawOutlinedRect(float x, float y, float w, float h, float thickness, Color boxColor, Color borderColor, bool sides)
{
	CG_FillRect(x, y, w, h, boxColor.GetFloatPtr()); // bg 
	CG_FillRect(x, y - thickness, w, thickness, borderColor.GetFloatPtr()); // top bar
	CG_FillRect(x, y + h, w, thickness, borderColor.GetFloatPtr()); // bottom bar

	if (sides) 
	{
		CG_FillRect(x - thickness, y - thickness, thickness, h + (thickness * 2), borderColor.GetFloatPtr()); // left bar
		CG_FillRect(x + w, y - thickness, thickness, h + (thickness * 2), borderColor.GetFloatPtr()); // right bar
	}
}

float CalculateXValueWithAlign(float x, char* text, int font, float scale, eJustify justify) 
{
	if (justify == JustifyLeft)
		return x;

	if (justify == JustifyCenter)
		return x - (RE_Text_Width(text, scale) / 2);

	if (justify == JustifyRight)
		return x - RE_Text_Width(text, scale);

	return 0;
}

void DrawString(char* text, float x, float y, int font, float scale, Color color, bool outlined, bool shadow, eJustify justify) 
{
	float defaultX = CalculateXValueWithAlign(x, text, font, scale, justify);

	CG_DrawString(defaultX, y, scale, color.GetFloatPtr(), text);

	if (outlined)
		CG_DrawString(defaultX, y, scale + 0.02f, Color(0, 0, 0, 255).GetFloatPtr(), text);

	if (shadow)
		CG_DrawString(defaultX + 3.0f, y + 3.0f, scale, Color(0, 0, 0, 255).GetFloatPtr(), text);
}

void DrawShader(float x, float y, float width, float height, Color color, int material) 
{
	CG_FillRect(x, y, width, height, color.GetFloatPtr());
}

void DrawRect(float x, float y, float width, float height, eJustify justify, Color color)
{
	if (justify == JustifyLeft)
		DrawShader(x, y, width, height, color, matWhite);

	if (justify == JustifyCenter)
		DrawShader(x - (width / 2), y, width, height, color, matWhite);

	if (justify == JustifyRight)
		DrawShader(x - width, y, width, height, color, matWhite);
}

char* WordWrap(char* text, int font, float fontScale, int maxX, int* lines) 
{
	*lines = 1;
	vector<int> newLines(1);
	string newText = string(text);


	if (strstr(text, "\n")) {
		for (size_t i = 0; i < newText.length(); i++) {
			string newstr = newText.substr(0, i);
			int pos = newstr.find_last_of("\n");
			if (pos > 1) {
				auto vit = find(newLines.begin(), newLines.end(), pos);
				if (vit == newLines.end()) {
					newLines.push_back(pos);
					*lines = *lines + 1;
				}
			}
		}
	}

	int curPos = 0;
	string query = text;

loop:
	auto len = RE_Text_Width((char*)query.c_str(), fontScale) * fontScale;
	if (len >= maxX) {
		for (int i = 0; i < query.size(); i++) {
			auto str = query.substr(0, i);
			len = RE_Text_Width((char*)str.c_str(), fontScale) * fontScale;
			if (len >= maxX) {
				auto lastSpace = str.find_last_of(' ');
				if (lastSpace) {
					text[curPos + lastSpace] = '\n';
					curPos += lastSpace;
					query = newText.substr(curPos);
					*lines = *lines + 1;
					goto loop;
				}
			}
		}
	}

	return text;
}

void DrawInfoBox(char* text, int pos)
{
	int textwidth = RE_Text_Width(text, 0.3f);
	DrawOutlinedRect(640.0f - textwidth - 8, (pos * 16.0f) + 15.0f + (pos * 5.0f), textwidth + 5.0f, 13.0f, 1, Color(0, 0, 0, 100), mainMenuColor, true);
	DrawString(text, CalculateXValueWithAlign(635.0f, text, fonts.normalFont, 0.54f, JustifyLeft), 27.0f + (pos * 16.0f) + (pos * 5.0f), fonts.normalFont, 0.3f, Color(0xFFFFFFFF), false, false, JustifyRight);
}

/*void DrawOpenBox(char* text)
{
	int textWidth = R_TextWidth(text, fonts.normalFont);
	DrawOutlinedRect(5.0f, 690.0f, (textWidth * 0.54f) + 10.0f, 22.0f, 2, Color(0, 0, 0, 100), mainMenuColor, true);
	DrawString(text, CalculateXValueWithAlign(10.0f, text, fonts.normalFont, 0.54f, JustifyLeft), 710.0f, fonts.normalFont, 0.54f, Color(0xFFFFFFFF), false, false, JustifyLeft);
}*/

void Draw2DBox(Vector2 feetloc, float width, float height, Color color, bool fill)
{
	if (fill)
		DrawShader(feetloc.x - (width / 2), feetloc.y - height, width, height, color, matWhite);
	else
	{
		DrawShader((feetloc.x - (width / 2)), (feetloc.y - height), width, 1, color, matWhite);
		DrawShader((feetloc.x - (width / 2)), feetloc.y, width, 1, color, matWhite);
		DrawShader((feetloc.x - (width / 2)), (feetloc.y - height), 1, height, color, matWhite);
		DrawShader((feetloc.x + (width / 2)), (feetloc.y - height), 1, height + 1, color, matWhite);
	}
}

void DrawLine(float x1, float y1, float x2, float y2, float width, Color color)
{
	float x, y, angle, l1, l2, h1;

	h1 = y2 - y1;
	l1 = x2 - x1;
	l2 = sqrt(l1 * l1 + h1 * h1);
	x = x1 + ((l1 - l2) / 2);
	y = y1 + (h1 / 2);
	angle = atan(h1 / l1) * (180 / XM_PI);

	CG_DrawRotatedPic(x, y, l2, width, angle, color.GetFloatPtr());
}

void DrawEntityBox(int entity, Vector3 origin, Vector3 viewAngles, Color color)
{
	/*Vector3 min, max;
	Entity_GetBounds(entity, &min, &max);
	min += origin;
	max += origin;

	Vector3 _1 = Vector3(max.x, min.y, min.z);
	Vector3 _2 = Vector3(max.x, min.y, max.z);
	Vector3 _3 = Vector3(min.x, min.y, max.z);
	Vector3 _4 = Vector3(min.x, max.y, max.z);
	Vector3 _5 = Vector3(min.x, max.y, min.z);
	Vector3 _6 = Vector3(max.x, max.y, min.z);

	RotatePoint2D(_1, origin, viewAngles, &_1);
	RotatePoint2D(_2, origin, viewAngles, &_2);
	RotatePoint2D(_3, origin, viewAngles, &_3);
	RotatePoint2D(_4, origin, viewAngles, &_4);
	RotatePoint2D(_5, origin, viewAngles, &_5);
	RotatePoint2D(_6, origin, viewAngles, &_6);
	RotatePoint2D(min, origin, viewAngles, &min);
	RotatePoint2D(max, origin, viewAngles, &max);

	Vector2 smin, smax, crnr1, crnr2, crnr3, crnr4, crnr5, crnr6;
	if (WorldToScreen(min, smin) && WorldToScreen(max, smax)
		&& WorldToScreen(_1, crnr1) && WorldToScreen(_2, crnr2)
		&& WorldToScreen(_3, crnr3) && WorldToScreen(_4, crnr4)
		&& WorldToScreen(_5, crnr5) && WorldToScreen(_6, crnr6))
	{
		// From min to 2, 4 and 6 
		DrawLine(smin.x, smin.y, crnr1.x, crnr1.y, 1.0f, color);
		DrawLine(smin.x, smin.y, crnr3.x, crnr3.y, 1.0f, color);
		DrawLine(smin.x, smin.y, crnr5.x, crnr5.y, 1.0f, color);

		// From max to 5, 7 and 3 
		DrawLine(smax.x, smax.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(smax.x, smax.y, crnr6.x, crnr6.y, 1.0f, color);
		DrawLine(smax.x, smax.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 2 to 7 and 3 
		DrawLine(crnr1.x, crnr1.y, crnr6.x, crnr6.y, 1.0f, color);
		DrawLine(crnr1.x, crnr1.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 4 to 5 and 3 
		DrawLine(crnr3.x, crnr3.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(crnr3.x, crnr3.y, crnr2.x, crnr2.y, 1.0f, color);

		// From 6 to 5 and 7 
		DrawLine(crnr5.x, crnr5.y, crnr4.x, crnr4.y, 1.0f, color);
		DrawLine(crnr5.x, crnr5.y, crnr6.x, crnr6.y, 1.0f, color);
	}*/
}

void DrawCornerBox(Vector2 feetloc, float width, float height, Color color)
{
	DrawShader((feetloc.x - (width / 2)), (feetloc.y - height), (width / 4), 1, color, matWhite);
	DrawShader((feetloc.x - (width / 2)), feetloc.y, (width / 4), 1, color, matWhite);
	DrawShader((feetloc.x - (width / 2)), (feetloc.y - height), 1, (width / 4), color, matWhite);
	DrawShader((feetloc.x + (width / 2)), (feetloc.y - height), 1, (width / 4), color, matWhite);

	DrawShader((feetloc.x + ((width / 2) - (width / 4))), (feetloc.y - height), (width / 4), 1, color, matWhite);
	DrawShader((feetloc.x + ((width / 2) - (width / 4))), feetloc.y, (width / 4) + 1, 1, color, matWhite);
	DrawShader((feetloc.x - (width / 2)), (feetloc.y - (width / 4)), 1, (width / 4), color, matWhite);
	DrawShader((feetloc.x + (width / 2)), (feetloc.y - (width / 4)), 1, (width / 4), color, matWhite);
}

void DrawBoneLine(entity_s* ent, char* tagname1, char* tagname2, Color color)
{
	Vector3 orig1 = GetTagPos(ent, tagname1);
	Vector3 orig2 = GetTagPos(ent, tagname2);
	Vector2 loc1, loc2;
	if (WorldToScreen(orig1, loc1) && WorldToScreen(orig2, loc2))
		DrawLine(loc1.x, loc1.y, loc2.x, loc2.y, 1.25f, color);
}

void DrawBones(entity_s* client, Color color)
{
	DrawBoneLine(client, "bip01 l calf", "bip01 pelvis", color);
	DrawBoneLine(client, "bip01 r forearm", "bip01 r hand", color);
	DrawBoneLine(client, "bip01 neck", "bip01 r forearm", color);
	DrawBoneLine(client, "bip01 r foot", "bip01 r calf", color);
	DrawBoneLine(client, "bip01 l forearm", "bip01 l hand", color);
	DrawBoneLine(client, "bip01 l foot", "bip01 l calf", color);
	DrawBoneLine(client, "bip01 r calf", "bip01 pelvis", color);
	DrawBoneLine(client, "bip01 pelvis", "bip01 neck", color);
	DrawBoneLine(client, "bip01 neck", "bip01 l forearm", color);
	DrawBoneLine(client, "bip01 neck", "bip01 head", color);
	DrawBoneLine(client, "bip01 head", "bip01 helmet", color);
}

void DrawTextBackground(char* text, float x, float y, float scale)
{
	float newScalex = 0.0f, newScaley = 0.0f;
	float width = RE_Text_Width(text, scale);
	float height = RE_Text_Height(5, scale);
	x = floor(-((width * 0.5f) - x) + 0.5f);
	y = floor(y + 0.5f);
	DrawShader(x, y, width, height, Color(0, 0, 0, 77), matWhite);
	CG_DrawString(x, y + height, scale, Color(0xFFFFFFFF).GetFloatPtr(), text);
}

void DrawText(char* text, float x, float y, float scale)
{
	/*float width = R_TextWidth(text, fonts.normalFont) * scale;
	float height = R_TextHeight(fonts.normalFont) * scale;
	float newScale = R_NormalizedTextScale(fonts.normalFont, scale);
	x = floor(-(((width * newScale) * 0.5f) - x) + 0.5f);
	y = floor(y + 0.5f);
	R_AddCmdDrawText(text, 0x7FFFFFFF, fonts.normalFont, x, y + height, scale, scale, 0.0f, Color(0xFFFFFFFF).GetFloatPtr(), 0);*/
}

void DrawFOVBox(float x, float y, float w, float h, float thickness, Color color)
{
	CG_FillRect(x, y - thickness, w, thickness, color.GetFloatPtr()); // top bar
	CG_FillRect(x, y + h, w, thickness, color.GetFloatPtr()); // bottom bar
	CG_FillRect(x - thickness, y - thickness, thickness, h + (thickness * 2), color.GetFloatPtr()); // left bar
	CG_FillRect(x + w, y - thickness, thickness, h + (thickness * 2), color.GetFloatPtr()); // right bar
}

void DrawCircle(float x, float y, float points, float radius, Color color)
{
	float step = XM_PI * 2.0f / points;

	for (float a = 0; a < (XM_PI * 2.0f); a += step)
	{
		float startx = radius * cosf(a) + x;
		float starty = radius * sinf(a) + y;
		float endx = radius * cosf(a + step) + x;
		float endy = radius * sinf(a + step) + y;

		DrawLine(startx, starty, endx, endy, 2.0f, color);
	}
}