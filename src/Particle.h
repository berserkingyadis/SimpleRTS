#ifndef PARTICLE_H
#define PARTICLE_H

#include "LTexture.h"

class Particle
{
public:
	Particle(int x, int y);
	void render() const;
	bool isDead();
private:
	int mPosX, mPosY;
	int mFrame;
	LTexture* mTexture;
};
#endif // !PARTICLE_H
