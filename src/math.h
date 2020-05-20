#ifndef H_MATH
#define H_MATH

#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAP(a, min, max) (MIN(MAX((a),(min))(max)))
#define ABS(a) ((a < 0 ) ? (-a) : (a))

struct Circle {
	float x, y, r;
};

#ifndef M_PI
#define M_PI 3.141592653589793
#endif // !M_PI

float lerp(float v0, float v1, float t);
float distanceSquared(float x1, float y1, float x2, float y2);

#endif // !H_MATH
