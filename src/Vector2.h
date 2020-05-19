#ifndef H_VECTOR2
#define H_VECTOR2

#include "SDL.h"
#include <string>
#include <iostream>

class Vector2 {
public:
	float mX = 0;
	float mY = 0;

	Vector2(float ix = 0, float iy = 0) : mX(ix), mY(iy) {}

	inline void flip() { mX = -mX; mY = -mY; }


	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
	friend std::istream& operator>>(std::istream& is, Vector2& vec);
};


#endif // !H_VECTOR2


