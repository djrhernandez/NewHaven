// Created by Junior on 6/19/2016.
#include "EnemyFighter_Omni.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "Config.h"
#include "defns.h"
#include "Global.h"
#include "addItemScreen.h"

#include "HeroFighter.h"

#include "Ammo.h"
#include "EnemyAmmo.h"

EnemyFighter_Omni::EnemyFighter_Omni(EnemyType et, float p[3], float randFact)
		: EnemyFighter(et, p, randFact)
{
	init(p, randFact);
}

EnemyFighter_Omni::~EnemyFighter_Omni()
{
}

//Gets rid of the IRIX compiler warning...
void EnemyFighter_Omni::init()
{
	EnemyFighter::init();
}

void EnemyFighter_Omni::init(float *p, float randFact)
{
	EnemyFighter::init(p, randFact);
	float frand = FRAND * randFact;

	damage = baseDamage = -45.0;
	size[0] = 0.7;
	size[1] = 0.7;
	collisionMove = 0.7;
	vel[1] = -0.071 - frand * 0.04;
}

//NOTE: Many of the firing rates are not adjusted by game->speedAdj so they
// 		will not be totally correct.
// 		Should be close enough (for jazz?), though.
void EnemyFighter_Omni::update()
{
	Config	*config = Config::instance();
	float v[3] = {0.0, -0.2, 0.0};
	//float	*hpos = target->getPos();
	float *hpos = game->hero->getPos();
	float a = hpos[0] - pos[0];
	float b = hpos[1] - pos[1];
	float dist;
	float ammoSpeed = 0.35 * game->speedAdj;
	int omniSwap = 108;

	//Update age
	age++;
	shootInterval--;

	pos[0] += secondaryMove[0] * game->speedAdj;
	pos[1] += secondaryMove[1] * game->speedAdj;
	float s = (1.0 - game->speedAdj) + (game->speedAdj * 0.7);
	secondaryMove[0] *= s;
	secondaryMove[1] *= s;
	move();

	float p[3] = {pos[0], pos[1], pos[2]};

	shootSwap = shootSwap%omniSwap;
	if (shootSwap < 18)
	{
		if (!(shootSwap % 6))
		{
			//ammoSpeed = 0.22;
			ammoSpeed = 0.3 * game->gameSkill * game->speedAdj;
			dist = fabs(a) + fabs(b);
			//dist = sqrt(a*a+b*b);
			v[0] = a/dist;
			v[1] = b/dist;

			shootVec[0] = ammoSpeed * v[0];
			shootVec[1] = ammoSpeed * v[1]; //+vel[1]
		}
		game->enemyAmmo->addAmmo(1, p, shootVec);
	}
	if (pos[1] < config->screenBoundY())
		shootSwap++;

}

//----------------------------------------------------------
void EnemyFighter_Omni::move()
{
	Config *config = Config::instance();
	float *hpos;
	if (target)
		hpos = target->getPos();
	else
		hpos = pos;
	float diff[2] = {hpos[0]-pos[0], hpos[1]-pos[1]};

	lastMoveX = (0.9 * lastMoveX) + (0.1 * (0.01 * diff[0]));
	pos[0] += game->speedAdj * (randMoveX * lastMoveX);
	pos[1] += (game->speedAdj * (vel[1] * game->gameSkill));


	if (pos[0] < -config->screenBoundX())
		pos[0] = -config->screenBoundX();
	if (pos[0] >  config->screenBoundX())
		pos[0] =  config->screenBoundX();
}
