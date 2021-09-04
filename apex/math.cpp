#include "math.h"

void Math::AngleVectors(const Vector& angles, Vector* forward)
{
	Assert(forward);

	float	sp, sy, cp, cy;

	sy = std::sinf(DEG2RAD(angles.y));
	cy = std::cosf(DEG2RAD(angles.y));

	sp = std::sinf(DEG2RAD(angles.x));
	cp = std::cosf(DEG2RAD(angles.x));

	forward->x = cp * cy;
	forward->y = cp * sy;
	forward->z = -sp;
}

void Math::LinearSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount)
{
	Math::NormalizeAngles(&aimAngles);
	Vector3 delta = aimAngles - currentAngles;
	Math::NormalizeAngles(&delta);

	float divisor = fabsf(amount - 1.f);

	if (divisor > 0.0)
	{
		delta.y = delta.y * divisor;
		delta.x = delta.x * divisor;
	}

	*smoothedAngles = Vector(currentAngles) + delta;
}

void Math::SqrtSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount)
{
	Math::NormalizeAngles(&aimAngles);
	float fovToPoint = Math::FovToAngle(currentAngles, aimAngles);
	Vector3 delta = aimAngles - currentAngles;
	Math::NormalizeAngles(&delta);

	float divisor = sqrtf((amount / 10) * (-1 * fovToPoint + 91));
	if (divisor > 1.0)
	{
		delta.y = delta.y / divisor;
		delta.x = delta.x / divisor;
	}
	*smoothedAngles = Vector(currentAngles) + delta;
}

void Math::QudraticSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount)
{
	Math::NormalizeAngles(&aimAngles);
	float fovToPoint = Math::FovToAngle(currentAngles, aimAngles);
	Vector3 delta = aimAngles - currentAngles;
	Math::NormalizeAngles(&delta);

	float divisor = (-1)*((amount + 48.f) / (8 * 1000.f)) * (fovToPoint) * (fovToPoint - 181);
	//float ydivisor = amount * .75;
	if (divisor > 1.0)
	{
		delta.y = delta.y / divisor;
		delta.x = delta.x / divisor;
	}
	*smoothedAngles = Vector(currentAngles) + delta;
}

void Math::ExponentialSmooth(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount)
{

}

void Math::QudraticSmoothAssist(const Vector& currentAngles, Vector aimAngles, Vector* smoothedAngles, float amount)
{
	Math::NormalizeAngles(&aimAngles);
	float fovToPoint = Math::FovToAngle(currentAngles, aimAngles);
	Vector3 delta = aimAngles - currentAngles;
	Math::NormalizeAngles(&delta);

	float divisor = (.0005) * powf((fovToPoint - 90), 2) + amount;
	if (divisor > 1.0)
	{
		delta.y = delta.y / divisor;
		delta.x = delta.x / divisor;
	}
	*smoothedAngles = Vector(currentAngles) + delta;
}

void Math::NormalizeAngles(Vector3* angles)
{

	if (!std::isfinite(angles->x) || !std::isfinite(angles->y))
	{
		*angles = Vector3(0, 0, 0);
		return;
	}

	angles->x = std::clamp(angles->x, -89.9999f, 89.9999f);
	angles->y = std::remainderf(angles->y, 360.0f);
	angles->z = 0.0f;
}

void Math::CalcAngle(const Vector src, const Vector dst, Vector* angles)
{


	Vector relative = src - dst;
	float magnitude = sqrtf(powf(relative.x, 2) + powf(relative.y, 2));
	float yawAngle = RAD2DEG(std::atanf(relative.y / relative.x));

	angles->x = RAD2DEG(std::atanf(relative.z / magnitude));
	angles->y = (relative.x) > 0 ? yawAngle + 180.f : yawAngle;
	angles->z = 0.f;

	NormalizeAngles(angles);
}



float Math::FovToAngle(const Vector& viewangle, const Vector& aimangle)
{
	Vector ang, aim;

	AngleVectors(viewangle, &aim);
	AngleVectors(aimangle, &ang);

	return (RAD2DEG(std::acosf(aim.Dot(ang) / aim.LengthSqr())));
}