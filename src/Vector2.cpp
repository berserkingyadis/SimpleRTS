#include "Vector2.h"


Vector2 Vector2::operator+(const Vector2& other)
{
	return Vector2(mX + other.mY, mY + other.mY);
}

Vector2 Vector2::operator-(const Vector2& other)
{
	return Vector2(mX - other.mX, mY - other.mY);
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
