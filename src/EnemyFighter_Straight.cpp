// Created by Junior on 6/19/2016.
#include "EnemyFighter_Straight.h"
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

EnemyFighter_Straight::EnemyFighter_Straight(EnemyType et, float p[3], float randFact)
		: EnemyFighter(et, p, randFact)
{
	init(p, randFact);
}

EnemyFighter_Straight::~EnemyFighter_Straight()
{
}

//Gets rid of the IRIX compiler warning...
void EnemyFighter_Straight::init()
{
	EnemyFighter::init();
}

void EnemyFighter_Straight::init(float *p, float randFact)
{
	EnemyFighter::init(p, randFact);
	float frand = FRAND * randFact;

	damage = baseDamage = -110.0 * game->gameSkill;
	size[0] = 0.75;
	size[1] = 1.02;
	collisionMove = 0.5;
	vel[1] = -0.046 - frand * 0.04;
}

//NOTE: Many of the firing rates are not adjusted by game->speedAdj so they
// 		will not be totally correct.
// 		Should be close enough (for jazz?), though.
void EnemyFighter_Straight::update()
{
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

	if (shootInterval < 10)
		preFire = (10 - shootInterval)/10.0;
	else
		preFire = 0.0;
	if (!shootInterval)
	{
		calcShootInterval();
		p[1] -= 0.9;
		game->enemyAmmo->addAmmo(0, p, shootVec);
	}
}

void EnemyFighter_Straight::calcShootInterval()
{
	shootInterval = (int)((30.0 + FRAND * 90.0)/game->speedAdj);
}

void EnemyFighter_Straight::move()
{
	Config *config = Config::instance();
	float *hpos;
	if (target)
		hpos = target->getPos();
	else
		hpos = pos;

	pos[1] += (game->speedAdj * (vel[1] * game->gameSkill));
	//fprintf(stderr, "EnemyAircraft_Straight %p ... pos[1] = %g,
	// vel[1] = %g\n", this, pos[1], vel[1]);

	if (pos[0] < -config->screenBoundX())
		pos[0] = -config->screenBoundX();
	if (pos[0] >  config->screenBoundX())
		pos[0] =  config->screenBoundX();
}
