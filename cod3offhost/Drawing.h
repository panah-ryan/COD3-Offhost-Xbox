#pragma once
#include "stdafx.h"
#include "Engine.h"

struct RGBA 
{
	BYTE R;
	BYTE G;
	BYTE B;
	BYTE A;

	RGBA() {}

	RGBA(BYTE _r, BYTE _g, BYTE _b, BYTE _a) {
		R = _r;
		G = _g;
		B = _b;
		A = _a;
	}
};

struct Fonts {
	int bigDevFont;
	int smallDevFont;
	int consoleFont;
	int hudSmallFont;
	int hudBigFont;
	int objectiveFont;
	int extraBigFont;
	int normalFont;
	int boldFont;
	int smallFont;
	int bigFont;
};

class Color {
public:
	RGBA Color_;

	Color() {}

	Color(BYTE r, BYTE g, BYTE b, BYTE a = 255)
		: Color_(RGBA(r, g, b, a)) {
	}

	Color(unsigned int hex) {
		Color_.R = (hex >> 24) & 0xFF;
		Color_.G = (hex >> 16) & 0xFF;
		Color_.B = (hex >> 8) & 0xFF;
		Color_.A = hex & 0xFF;

		if (Color_.A == 0) {
			Color_.A = 255;
		}
	}

	float* GetFloatPtr() {
		static float ptr[4];
		ptr[0] = float(Color_.R / 255.f);
		ptr[1] = float(Color_.G / 255.f);
		ptr[2] = float(Color_.B / 255.f);
		ptr[3] = float(Color_.A / 255.f);
		return ptr;
	}

	Color WithOpacity(int opacity) {
		static Color col;
		col = *this;
		col.Color_.A = opacity;
		return col;
	}
};

enum eJustify {
	JustifyLeft,
	JustifyCenter,
	JustifyRight
};

void DrawOutlinedRect(float x, float y, float w, float h, float thickness, Color boxColor, Color borderColor, bool sides = false);
float CalculateXValueWithAlign(float x, char* text, int font, float scale, eJustify justify);
void DrawString(char* text, float x, float y, int font, float scale, Color color, bool outlined, bool shadow, eJustify justify);
void DrawShader(float x, float y, float width, float height, Color color, int material);
void DrawRect(float x, float y, float width, float height, eJustify justify, Color color);
char* WordWrap(char* text, int font, float fontScale, int maxX, int* lines);
void DrawInfoBox(char* text, int pos);
//void DrawOpenBox(char* text);
void Draw2DBox(Vector2 feetloc, float width, float height, Color color, bool fill);
void DrawLine(float x1, float y1, float x2, float y2, float width, Color color);
void DrawEntityBox(int entity, Vector3 origin, Vector3 viewAngles, Color color);
void DrawCornerBox(Vector2 feetloc, float width, float height, Color color);
void DrawBones(entity_s* client, Color color);
void DrawTextBackground(char* text, float x, float y, float scale);
void DrawText(char* text, float x, float y, float scale);
void DrawFOVBox(float x, float y, float w, float h, float thickness, Color color);
void DrawCircle(float x, float y, float points, float radius, Color color);