#ifndef PARTICLE_H
#define PARTICLE_H

#include "LTexture.h"

class Particle
{
public:
	Particle(float x, float y);
	void render() const;
	bool isDead();
private:
	float mPosX, mPosY;
	int mFrame;
	LTexture* mTexture;
};
#endif // !PARTICLE_H
