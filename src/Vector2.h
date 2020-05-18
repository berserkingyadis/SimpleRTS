#ifndef H_VECTOR2
#define H_VECTOR2

#include "SDL.h"
#include <string>
#include <iostream>

class Vector2 {
public:
	int32_t mX = 0;
	int32_t mY = 0;

	Vector2(int32_t ix = 0, int32_t iy = 0) : mX(ix), mY(iy) {}
	Vector2(Vector2& v) : mX(v.mX), mY(v.mY) {}

	inline void flip() { mX = -mX; mY = -mY; }


	Vector2 operator+(const Vector2& other);
	Vector2 operator-(const Vector2& other);
	Vector2& operator+=(const Vector2& other);
	Vector2& operator-=(const Vector2& other);

	friend std::ostream& operator<<(std::ostream& os, const Vector2& vec);
	friend std::istream& operator>>(std::istream& is, Vector2& vec);
};


#endif // !H_VECTOR2


