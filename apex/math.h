#pragma once
#include "stdafx.h"


#define Assert( _exp ) ((void)0)
#define PI 3.14159265358979323846f
#define RADPI 57.295779513082f
#define DEG2RAD( x ) ( ( float )( x ) * ( float )( ( float )( PI ) / 180.0f ) )
#define RAD2DEG( x ) ( ( float )( x ) * ( float )( 180.0f / ( float )( PI ) ) )

#pragma region Structs
struct Vector3 {

	float x, y, z;

	Vector3() {};

	Vector3(float x, float y, float z) {
		this->x = x;  this->y = y; this->z = z;
	}

	float Distance(Vector3 a) {
		return sqrtf(powf(a.x - x, 2) + powf(a.y - y, 2) + powf(a.z - z, 2));
	}

	float Length() {
		return sqrtf(x * x + y * y + z * z);
	}

	float LengthSqr()
	{
		return (x * x + y * y + z * z);
	}

	float Length2D() {
		return sqrtf(x * x + y * y);
	}

	float Dot(Vector3 a) {
		return (a.x * x + a.y * y + a.z * z);
	}

	//TODO: Implement all operators and make them more effective

	Vector3 operator-( const Vector3 subtract) const {
		return Vector3(x - subtract.x,
			y - subtract.y,
			z - subtract.z);
	}

	Vector3 operator+(const Vector3& add) {
		return Vector3(x + add.x,
			y + add.y,
			z + add.z);
	}

	Vector3 operator/(const float& div) {
		return Vector3(x / div,
			y / div,
			z / div);
	}

	Vector3 operator*(const float& mult) {
		return Vector3(x * mult,
			y * mult,
			z * mult);
	}


	Vector3 operator*(Vector3& mult) {
		return Vector3(x * mult.x,
			y * mult.y,
			z * mult.z);
	}

	void operator-=(const Vector3& subtract) {
		x -= subtract.x;
		y -= subtract.y;
		z -= subtract.z;
	}

	void operator+=(const Vector3& add) {
		x += add.x;
		y += add.y;
		z += add.z;
	}

	void operator/=(const float div) {
		x /= div;
		y /= div;
		z /= div;
	}


	bool operator==(const Vector3& subtract) {
		return x == subtract.x &&
			y == subtract.y &&
			z == subtract.z;
	}

	bool operator<(const Vector3& subtract) {
		return x == subtract.x &&
			y == subtract.y &&
			z == subtract.z;
	}

	bool operator!=(const Vector3& subtract) {
		return x != subtract.x &&
			y != subtract.y &&
			z != subtract.z;
	}

};
typedef Vector3 Vector; //fuck typing that we all know vector is usually vector3
typedef Vector3 Vec3;

struct Vector4 {
	float w, x, y, z;
};
typedef Vector4 Vec4;

struct Vector2 {
	float x,y;
};
typedef Vector2 Vec2;

//Pasted from D3DXMATRIX
typedef struct _VMatrix {
	union {
		struct {
			float        _11, _12, _13, _14;
			float        _21, _22, _23, _24;
			float        _31, _32, _33, _34;
			float        _41, _42, _43, _44;
		};
		float matrix[4][4];
	};
	void Transpose()
	{
		int i, j;

		for (i = 0; i < 4; i++)
			for (j = 0; j < 4; j++) matrix[i][j] = matrix[j][i];
	}
} VMatrix;

#pragma endregion

namespace Math
{
	void AngleVectors(const Vector& angles, Vector* forward); //pasted from ayyware ;p
	void LinearSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount);
	void SqrtSmooth(const Vector& currentAngles,  Vector aimAngles, Vector* smoothedAngles, float amount);
	void QudraticSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount);
	void ExponentialSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount);
	void QudraticSmoothAssist(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount);
	void NormalizeAngles(Vector3* angles);
	void CalcAngle(const Vector src, const Vector dst, Vector* angles);
	float FovToAngle(const Vector& viewangle, const Vector& aimangle); //TODO: Implement into BasePlayer so that we can cache it and color players inside of FOV differently, or aimbot target
}