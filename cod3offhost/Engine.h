#pragma once

typedef float vec_t;
class Vector3
{
public:
	// Construction/destruction
	float x, y, z;

	inline Vector3(void)
	{

	}

	inline Vector3(float X, float Y, float Z)
	{
		x = X; y = Y; z = Z;
	}

	inline Vector3(double X, double Y, double Z)
	{
		x = (float)X; y = (float)Y; z = (float)Z;
	}

	inline Vector3(int X, int Y, int Z)
	{
		x = (float)X; y = (float)Y; z = (float)Z;
	}

	inline Vector3(float X, float Y, int Z)
	{
		x = (float)X; y = (float)Y; z = (int)Z;
	}

	inline Vector3(const Vector3& v)
	{
		x = v.x; y = v.y; z = v.z;
	}

	inline Vector3(float rgfl[3])
	{
		x = rgfl[0]; y = rgfl[1]; z = rgfl[2];
	}

	inline void Init(vec_t ix = 0.0f, vec_t iy = 0.0f, vec_t iz = 0.0f)
	{
		x = ix; y = iy; z = iz;
	}

	inline Vector3 operator-(void) const
	{
		return Vector3(-x, -y, -z);
	}

	inline int operator==(const Vector3& v) const
	{
		return x == v.x && y == v.y && z == v.z;
	}

	inline int operator!=(const Vector3& v) const
	{
		return !(*this == v);
	}

	inline Vector3 operator+(const Vector3& v) const
	{
		return Vector3(x + v.x, y + v.y, z + v.z);
	}

	inline Vector3 operator-(const Vector3& v) const
	{
		return Vector3(x - v.x, y - v.y, z - v.z);
	}

	inline Vector3 operator*(float fl) const
	{
		return Vector3(x*fl, y*fl, z*fl);
	}

	inline Vector3 operator/(float fl) const
	{
		return Vector3(x / fl, y / fl, z / fl);
	}

	void operator += (const Vector3& add)
	{
		x += add.x; y += add.y; z += add.z;
	}

	void operator += (const float add)
	{
		x += add; y += add; z += add;
	}

	void operator -= (const Vector3& sub)
	{
		x -= sub.x; y -= sub.y; z -= sub.z;
	}

	void operator *= (const float mul)
	{
		x *= mul; y *= mul; z *= mul;
	}

	void operator /= (const float mul)
	{
		x /= mul; y /= mul; z /= mul;
	}

	inline void CopyToArray(float* rgfl) const
	{
		rgfl[0] = x, rgfl[1] = y, rgfl[2] = z;
	}

	inline float Length(void) const
	{
		return sqrtf(x*x + y * y + z * z);
	}

	inline float LengthXY(void) const
	{
		return sqrtf(x*x + y * y);
	}

	operator float *()
	{
		return &x;
	} // Vector3s will now automatically convert to float * when needed

	operator const float *() const
	{
		return &x;
	} // Vector3s will now automatically convert to float * when needed

	inline Vector3 Normalize(void) const
	{
		float flLen = Length();
		if (flLen == 0) return Vector3(0, 0, 1); // ????
		flLen = 1 / flLen;
		return Vector3(x * flLen, y * flLen, z * flLen);
	}

	inline vec_t* Base()
	{
		return (vec_t*)this;
	}

	inline vec_t const* Base() const
	{
		return (vec_t const*)this;
	}

	inline float Length2D(void) const
	{
		return sqrtf(x*x + y * y);
	}

	inline bool NormalizeAngle()
	{
		if (x != x || y != y || z != z) return false;

		if (x > 180) x -= 360.f;
		if (x < -180) x += 360.f;
		if (y > 180) y -= 360.f;
		if (y < -180) y += 360.f;

		return x >= -180.f && x <= 180.f && y >= -180.f && y <= 180.f;
	}

	inline bool ClampAngle()
	{
		if (x > 85.f) x = 85.f;
		if (x < -85.f) x = -85.f;

		z = 0.f;

		return x >= -85.f && x <= 85.f && y >= -180.f && y <= 180.f;
	}

	inline Vector3 AddFloat(float f) {
		return Vector3(x + f, y + f, z + f);
	}

	inline float NormalizeInPlace()
	{
		Vector3& v = *this;

		float iradius = 1.f / (this->Length() + 1.192092896e-07F); //FLT_EPSILON

		v.x *= iradius;
		v.y *= iradius;
		v.z *= iradius;

		return iradius;
	}

	inline float Dot(const Vector3& vOther) const
	{
		const Vector3& a = *this;

		return(a.x*vOther.x + a.y*vOther.y + a.z*vOther.z);
	}

	inline bool IsZero()
	{
		return this->x == 0 && this->y == 0 && this->z == 0;
	}

	inline void SetIfLessThan(Vector3 val) {
		if (val.x < x) {
			x = val.x;
		}
		if (val.y < y) {
			y = val.y;
		}
		if (val.z < z) {
			z = val.z;
		}
	}

	inline void SetIfGreaterThan(Vector3 val) {
		if (val.x > x) {
			x = val.x;
		}
		if (val.y > y) {
			y = val.y;
		}
		if (val.z > z) {
			z = val.z;
		}
	}
};

class Vector3Aligned : public Vector3
{
public:
	Vector3Aligned()
	{
		x = y = z = 0;
	}

	Vector3Aligned(const Vector3& in)
	{
		x = in.x;
		y = in.y;
		z = in.z;
	}

	float w;
};

struct Vector2
{
	float x, y;
};

enum entityType_t : char
{
	ET_GENERAL,
	ET_PLAYER,
	ET_ITEM,
	ET_MISSILE,
	ET_MOVER,
	ET_PORTAL,
	ET_INVISIBLE,
	ET_SCRIPTMOVER,
	ET_SOUND_BLEND,
	ET_LOOP_FX,
	ET_MG42,
	ET_ACTOR,
	ET_ACTOR_SPAWNER,
	ET_ACTOR_CORPSE,
	ET_VEHICLE,
	ET_VEHICLE_CORPSE,
	ET_VEHICLE_COLLMAP,
	ET_PROP_COLLMAP
};

struct mpeer_s
{
	char _0x00[0x68];
	char gamertag[32];
	XUID xuid;
};

struct entity_s
{
	entityType_t type;
	char _0x01[0x14F];
	Vector3 pos; //0x150;
	char _0x15C[0x88];
	int number; //0x1E4
	int _0x1E8;
	DWORD dobj;
	char _0x1F0[0x10];
	DWORD _0x200;
	int _0x204;
	DWORD _0x208;

	bool isDead()
	{
		if (_0x200)
			return *(int*)(_0x200 + 0x24) == 6;

		return true;
	}

	int getTeam()
	{
		if (_0x208)
			return *(int*)(_0x208 + 4);

		return 0;
	}

	mpeer_s* getPeer()
	{
		DWORD manager = *(DWORD*)*(DWORD*)0x82B8C9C0 + 0xAEB0;
		return ((mpeer_s*(*)(DWORD, entity_s*))0x823AB018)(manager, this);
	}
};

struct DObjSkelMat
{
	char _0x00[0x30];
	Vector3 pos;
};

struct refdef_s
{
	int x; //0x00
	int y; //0x04
	int width; //0x08
	int height; //0x0C
	Vector2 fov; //0x10
	char _padding[8]; //0x18
	Vector3 vieworg; //0x20
	int _padding1; //0x2C
	Vector3 viewaxis[3]; //0x30
};

struct trace_t
{
	float _0x00[4];
	float _0x10[4];
	float fraction;
	int _0x24;
	int _0x28;
	int _0x2C;
	int _0x30;
	int _0x34;
	int _0x38;
	char _0x3C;
	char _0x3D;
	char _padding1[2];
	float _0x40;
	char _0x44;
	char _padding2[3];
};

struct collision_context_t
{
	int cxtFilter;
	int _0x04;
	int _0x08;
	int _0x0C;
	int _0x10;
	int contentMask;
};

struct clientActive_t
{
	char _0x00[0x98];
	Vector3 baseAngles;
	char _0xA4[0x5E0];
	Vector3 viewAngles;
};
//0x82BEF274 viewAngles
//0x82BEEC88 baseAngle?

enum BoneTags_e
{
	TAG_HELMET,
	TAG_NECK,
	TAG_HEAD,
	TAG_HIP_LE,
	TAG_HIP_RI,
	TAG_KNEE_LE,
	TAG_KNEE_RI,
	TAG_SPINEUPPER,
	TAG_ANKLE_LE,
	TAG_ANKLE_RI,
	TAG_SHOULDER_LE,
	TAG_SHOULDER_RI,
	TAG_ELBOW_LE,
	TAG_ELBOW_RI,
	TAG_JAW,
	TAG_THUMB_LE,
	TAG_THUMB_RI,
	TAG_PINKY_LE,
	TAG_PINKY_RI,
	TAG_SIZE
};

struct TraceBones
{
	int tag;
	Vector3 pos;
	Vector3 previousPos;
	bool visible;
	bool canDamageWall;
	float damage;
};

struct BoneTargetPriority
{
	int tag;
	int priority;
};

extern BoneTargetPriority boneTargets[20];
extern bool aimTargetBestBone;
extern int boneTargetValue;
struct AimTarget //Our own struct
{
	int bestTag;
	bool vis;
	bool canWallBang;
	bool valid;
	int priority;
	int index;
	float distance;
	bool dog;
	bool shieldPrimary;
	bool shieldBack;
	TraceBones bones[TAG_SIZE];

	bool isVisible()
	{
		if (aimTargetBestBone)
		{
			int j = 0;
			for (int i = 0; i < TAG_SIZE; i++)
			{
				if (bones[i].tag != boneTargets[j].tag)
					continue;

				j++;
				if (bones[i].visible)
					return true;
			}
		}
		else
			return bones[boneTargetValue].visible;

		return false;
	}

	bool canHitThroughWalls()
	{
		if (aimTargetBestBone)
		{
			int j = 0;
			for (int i = 0; i < TAG_SIZE; i++)
			{
				if (bones[i].tag != boneTargets[j].tag)
					continue;

				j++;
				if (bones[i].canDamageWall)
					return true;

			}
		}
		else
			return bones[boneTargetValue].canDamageWall;

		return false;
	}
};

struct GeoLocation
{
	XUID xuid;
	char isp[100];
	char city[100];
	char state[100];
	char country[100];
};

void CG_FillRect(float x, float y, float w, float h, float* color);
void CG_DrawString(float x, float y, float scale, float* color, char* text);
int RE_Text_Width(char* text, float scale);
char* Cvar_GetString(char* cvar);
void Cvar_GetBuffer(char* cvar, char* buffer, int size);
DWORD CalculateFPS();
int GetPlayerCount();
void Cbuf_ExecuteText(char* buf);
char* va(char* fmt, ...);
bool Entity_IsLocalPlayer(entity_s* ent);
Vector3 GetTagPos(entity_s* ent, char* tag);
bool WorldToScreen(Vector3 World, Vector2& screen);
int RE_Text_Height(int font, float scale);
void R_NormalizedTextScale(float* scalex, float* scaley);
void CG_DrawRotatedPic(float x, float y, float width, float height, float rotation, float* color);
bool IsTagVisible(entity_s* ent, char* tag);
float GetDistance(Vector3 c1, Vector3 c2);
entity_s* GetOurEntity();
Vector3 VectorToAngles(Vector3 &Delta);
void AnglesNormalize(Vector3 &angles);
void Sentient_GetEyePosition(entity_s* ent, Vector3* pos);
