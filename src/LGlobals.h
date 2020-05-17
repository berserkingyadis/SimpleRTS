#ifndef LGLOBALS_H
#define LGLOBALS_H


#define MIN(a,b) ((a) < (b) ? (a) : (b))
#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define CLAP(a, min, max) (MIN(MAX((a),(min))(max)))
#define ABS(a) ((a) < 0 ? (-a) : (a))

#ifndef M_PI
	#define M_PI 3.141592653589793
#endif // !M_PI


//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 50;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;

struct Circle {
	int x, y, r;
};

#endif