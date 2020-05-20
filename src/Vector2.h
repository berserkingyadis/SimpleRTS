#ifndef H_VECTOR2
#define H_VECTOR2

#include "SDL.h"
#include <string>
#include <iostream>
class Vector2;



class Vector2 {
public:

	float mX = 0;
	float mY = 0;

	Vector2(float ix = 0, float iy = 0) : mX(ix), mY(iy) {}

	float distanceTo(const Vector2& other);
	float length();
	float angle();

	Vector2 normalize();

	inline void flip() { mX = -mX; mY = -mY; }

	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2 operator*(float other);
	Vector2 operator/(float other);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
	friend std::istream& operator>>(std::istream& is, Vector2& vec);
};

static const Vector2 nullVektor(0.f, 0.f);
static const Vector2 oneVector(1.f, 1.f);

#endif // !H_VECTOR2


