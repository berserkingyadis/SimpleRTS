#include "Vector2.h"

#include "LGlobals.h"
#include "math.h"

#include <cmath>

float Vector2::distanceTo(const Vector2& other)
{	
	float xDif = mX - other.mX;
	float yDif = mY - other.mY;
	return std::sqrtf((xDif * xDif) + (yDif * yDif));
}

float Vector2::length()
{
	return distanceTo(nullVektor);
}

float Vector2::angle()
{
	return std::atan2(mX,-mY) * 180.f / M_PI;
}

Vector2 Vector2::normalize()
{
	float length = this->length();

	return Vector2(mX / length, mY / length);
}

Vector2 Vector2::operator+(const Vector2& other)
{
	return Vector2(mX + other.mX, mY + other.mY);
}

Vector2 Vector2::operator-(const Vector2& other)
{
	return Vector2(mX - other.mX, mY - other.mY);
}

Vector2 Vector2::operator*(float scalar)
{
	return Vector2(mX * scalar, mY * scalar);
}

Vector2 Vector2::operator/(float scalar)
{
	return Vector2(mX / scalar, mY / scalar);
}



Vector2& Vector2::operator+=(const Vector2& other)
{
	mX += other.mX;
	mY += other.mY;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& other)
{
	mX -= other.mX;
	mY -= other.mY;
	return *this;
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec)
{
	os << "Vector2[" << vec.mX << ", " << vec.mY << "]";
	return os;
}

std::istream& operator>>(std::istream& is, Vector2& vec)
{
	is >> vec.mX;
	is >> vec.mY;
	return is;
}
