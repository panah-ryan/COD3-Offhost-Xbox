#include "stdafx.h"
#include "Engine.h"
#include <setjmp.h>

void CG_FillRect(float x, float y, float w, float h, float* color)
{
	((void(*)(float, float, float, float, float*))0x82411830)(x, y, w, h, color);
}

void CG_DrawString(float x, float y, float scale, float* color, char* text)
{
	((void(*)(float, float, int, float, float*, char*, float, int, int))0x82406CE8)(x, y, 5, scale, color, text, scale, 0, 0);
}

int RE_Text_Width(char* text, float scale)
{
	return ((int(*)(char*, int, float, float, int))0x82406D20)(text, 5, scale, scale, 0);
}

char* Cvar_GetString(char* cvar)
{
	return ((char*(*)(char*))0x82511A98)(cvar);
}

void Cvar_GetBuffer(char* cvar, char* buffer, int size)
{
	((void(*)(char*, char*, int))0x82511A30)(cvar, buffer, size);
}

DWORD dwLastFrameTime = GetTickCount();
DWORD dwLastFrameTimes[64] = { 0 };
DWORD dwLastFPSValue = 60;
DWORD dwLastStableValue = 60;
DWORD dwFramesSinceChange = 0;
DWORD CalculateFPS()
{
	DWORD dwRunningTotal = 0;
	DWORD dwFrameTime = GetTickCount();
	for (DWORD i = 0; i < 63; i++) {
		dwLastFrameTimes[i] = dwLastFrameTimes[i + 1];
		dwRunningTotal += dwLastFrameTimes[i];
	}
	dwLastFrameTimes[63] = dwFrameTime - dwLastFrameTime;
	dwRunningTotal += dwLastFrameTimes[31];
	dwLastFrameTime = dwFrameTime;

	if (dwRunningTotal != 0) {
		DWORD dwAverage = dwRunningTotal / 64;
		if (dwAverage != 0) {
			DWORD dwFPS = (FLOAT)(1000 / dwAverage);
			if (dwLastFPSValue != dwFPS) {
				if (dwLastStableValue != dwLastFPSValue) {
					dwLastStableValue = dwLastFPSValue;
					dwFramesSinceChange = 0;
				}

				if (dwFPS != dwLastStableValue) {
					dwFramesSinceChange++;
				}

				if (dwFramesSinceChange > 1) {
					if (dwLastFPSValue > dwFPS) {
						dwLastFPSValue--;
					}
					else {
						dwLastFPSValue++;
					}
					if (dwLastFPSValue > 60) {
						dwLastFPSValue = 60;
					}
				}
			}
		}
	}
	return dwLastFPSValue;
}

int BrocSys_GetPlayerIndex(int entNumber)
{
	return ((int(*)(int))0x82484050)(entNumber);
}

extern entity_s** entityArray;
int GetPlayerCount()
{
	int count = 0;
	for (int i = 0; i < *(int*)0x82BA9EC4; i++)
	{
		entity_s* ent = entityArray[i];
		if (ent && ent->type == ET_PLAYER)
			count++;
	}
	return count;
}

char* va(char* fmt, ...)
{
	static char buffer[512];
	va_list args;
	va_start(args, fmt);
	vsnprintf(buffer, 512, fmt, args);
	return buffer;
}

void Cbuf_ExecuteText(char* buf)
{
	((void(*)(int, char*))0x82467ED0)(0, buf);
}

bool Entity_IsLocalPlayer(entity_s* ent)
{
	return ((bool(*)(entity_s*))0x824438C0)(ent);
}

DWORD HashString_CalcHash(char* tag)
{
	return ((DWORD(*)(char*))0x820D0EC8)(tag);
}

bool CG_DObjGetWorldTagMatrix(entity_s* ent, DWORD dobj, DWORD tag, DObjSkelMat* mat)
{
	return ((bool(*)(entity_s*, DWORD, DWORD, DObjSkelMat*))0x824211B0)(ent, dobj, tag, mat);
}

Vector3 GetTagPos(entity_s* ent, char* tag)
{
	DObjSkelMat mat;
	ZeroMemory(&mat, sizeof(DObjSkelMat));
	if (ent && ent->dobj)
		CG_DObjGetWorldTagMatrix(ent, ent->dobj, HashString_CalcHash(tag), &mat);
	return mat.pos;
}

bool WorldToScreen(Vector3 World, Vector2& screen)
{
	__vector4 out = { 0, 0, 0, 0 };
	__vector4 in = { World.x, World.y, World.z, 0.0f };
	((void(*)(__vector4*, __vector4*, DWORD))0x82136300)(&out, &in, *(DWORD*)0x82A4C22C);

	if (out.z < 1.0f)
		return false;

	screen.x = out.x;
	screen.y = out.y;
	return true;
}

int RE_Text_Height(int font, float scale)
{
	return ((int(*)(int, float))0x823F71C0)(font, scale);
}

void R_NormalizedTextScale(float* scalex, float* scaley)
{
	((void(*)(float*, float*))0x8240F150)(scalex, scaley);
}

void CG_DrawRotatedPic(float x, float y, float width, float height, float rotation, float* color)
{
	DWORD texture = ((DWORD(*)(char*, int, char*))0x82405EB0)("white", 0, "mp_frontEnd");
	((void(*)(float*))0x823F5078)(color);
	((void(*)(float, float, float, float, float, DWORD))0x82419B10)(x, y, width, height, rotation, texture);
	((void(*)(float*))0x823F5078)(NULL);
}

void CG_LocationalTrace(trace_t* trace, Vector3* start, Vector3* end, collision_context_t* cxt)
{
	Vector3 stub = Vector3(0, 0, 0);
	Vector3 stub1 = Vector3(0, 0, 0);

	((void(*)(trace_t*, Vector3*, Vector3*, Vector3*, Vector3*, collision_context_t*))0x82425A78)(trace, start, &stub, &stub1, end, cxt);
}

void SetupCollisionCxt(collision_context_t* cxt, entity_s* ent, int mask)
{
	cxt->cxtFilter = 0x8205C1FC;
	cxt->_0x04 = ent->number;
	cxt->_0x08 = 0;
	cxt->_0x0C = 0;
	cxt->_0x10 = 0;
	cxt->contentMask = mask;
}

bool IsTagVisible(entity_s* ent, char* tag)
{
	refdef_s* ref = (refdef_s*)0x82C3E6B0;
	Vector3 tagPos = GetTagPos(ent, tag);
	if (!tagPos.IsZero() && !ref->vieworg.IsZero())
	{
		trace_t results;
		collision_context_t cxt;
		SetupCollisionCxt(&cxt, ent, 0x803003);
		CG_LocationalTrace(&results, &ref->vieworg, &tagPos, &cxt);
		if (results.fraction == 1.0f)
			return true;
	}
	return false;
}

float GetDistance(Vector3 c1, Vector3 c2)
{
	float dx = c2.x - c1.x;
	float dy = c2.y - c1.y;
	float dz = c2.z - c1.z;

	return (sqrt((float)((dx * dx) + (dy * dy) + (dz * dz))) / 55.0f);
}

entity_s* GetOurEntity()
{
	for (int i = 0; i < *(int*)0x82BA9EC4; i++)
	{
		entity_s* ent = entityArray[i];
		if (Entity_IsLocalPlayer(ent))
			return ent;
	}
	return nullptr;
}

#define M_PI 3.14159265358979323846
Vector3 VectorToAngles(Vector3 &Delta)
{
	float Forward, Yaw, Pitch;

	if (Delta.x == 0 && Delta.y == 0)
	{
		Yaw = 0;
		if (Delta.z > 0)
			Pitch = 90.0f;
		else
			Pitch = 270.0f;
	}
	else
	{
		if (Delta.x != -1)
			Yaw = (atan2(Delta.y, Delta.x) * 180.0f / M_PI);
		else if (Delta.y > 0)
			Yaw = 90.0f;
		else
			Yaw = 270.0f;

		if (Yaw < 0)
			Yaw += 360.0f;

		Forward = sqrt((Delta.x * Delta.x) + (Delta.y * Delta.y));
		Pitch = (atan2(Delta.z, Forward) * 180.0f / M_PI);

		if (Pitch < 0)
			Pitch += 360;
	}
	return Vector3(-Pitch, Yaw, 0);
}

void AnglesNormalize(Vector3 &angles)
{
	while (angles.x < -180) angles.x += 360;
	while (angles.x > 180) angles.x -= 360;
	while (angles.y < -180) angles.y += 360;
	while (angles.y > 180) angles.y -= 360;
	while (angles.z < -180) angles.z += 360;
	while (angles.z > 180) angles.z -= 360;
}

void Sentient_GetEyePosition(entity_s* ent, Vector3* pos)
{
	entity_s* pointer = ent;
	((void(*)(entity_s**, Vector3*))0x8237B520)(&pointer, pos);
}
