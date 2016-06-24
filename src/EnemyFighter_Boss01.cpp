// Created by Junior on 6/19/2016.
#include "EnemyFighter_Boss01.h"
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

EnemyFighter_Boss01::EnemyFighter_Boss01(EnemyType et, float *p, float randFact)
		: EnemyFighter(et, p, randFact)
{
	init(p, randFact);
}

EnemyFighter_Boss01::~EnemyFighter_Boss00()
{
}

//Gets rid of IRIX compiler warning
void EnemyFighter_Boss01::init()
{
	EnemyFighter::init();
}

//Initial values of Boss00
void EnemyFighter_Boss01::init(float *p, float randFact)
{
	EnemyFighter::init(p, randFact);

	damage = baseDamage = -10000.0 * game->gameSkill;
	size[0] = 2.6;
	size[1] = 2.3;
	collisionMove = 0.1;
	vel[1] = 0.02;
	age = 600;
}

//Note: Many of the firing rates are not adjusted by game->speedAdj so they
// 		will not be totally correct.
// 		Should be close enough (for jazz?), though.
void EnemyFighter_Boss01::update()
{
	EnemyFighter *tmpFighter = 0;
	//float *hpos = target->getPos();
	float *hpos = game->hero->getPos();
	float a = hpos[0] - pos[0];

	//Update age
	age++;
	shootInterval--;

	pos[0] += secondaryMove[0] * game->speedAdj;
	pos[1] += secondaryMove[1] * game->speedAdj;
	float s = (1.0 - game->speedAdj) + (game->speedAdj * 0.07);
	secondaryMove[0] *= s;
	secondaryMove[1] *= s;
	move();


	float p[3] = {pos[0], pos[1], pos[2]};
	if (fabs(a) < 5.0) {
		shootVec[1] = -0.65;
		preFire = (age % 6)/6.0;

		if (!(age % 5)) {
			shootSwap = !shootSwap;
			if (shootSwap) {
				p[0] = pos[0] + 0.55;
				p[1] = pos[1] - 1.7;
				game->enemyAmmo->addAmmo(0, p, shootVec);
				p[1] += 0.5;
				game->enemyAmmo->addAmmo(0, p, shootVec);
			} else {
				p[0] = pos[0] - 1.22;
				p[1] = pos[1] - 1.22;
				game->enemyAmmo->addAmmo(0, p, shootVec);
				p[1] += 0.5;
				game->enemyAmmo->addAmmo(0, p, shootVec);
			}
		}
	} else {
		if (preFire > 0.0)
			preFire -= 0.05;
		else
			preFire = 0.0;
	}

	//Add Gnats
	if (!((age/512) % 2)) {
		if (!((age/64) % 2) && !(age % 5)) {
			p[0] += 1.7;
			p[1] += 1.2;

			tmpFighter = game->addItem->dynamicEnemyAdd(EnemyGnat, p,
														game->gameFrame + 2);
			tmpFighter->over = this;
			tmpFighter->setTarget(this);
		}
	}
}

void EnemyFighter_Boss01::move()
{
	Config *config = Config::instance();
	float *hpos;

	if (target)
		hpos = target->getPos();
	else
		hpos = pos;
	float diff[2] = {hpos[0] - pos[0], hpos[1] - pos[1]};
	float approachDist;

	if ((((age + 25)/512) % 2))
		approachDist = 9.0 * (2.0 - game->gameSkill);
	else
		approachDist = 12.0 * (2.0 - game->gameSkill);

	if (fabs(diff[1]) < (approachDist + 2.0 * sin(game->frame * 0.05)))	{
		diff[1] = diff[1] * diff[1]/approachDist;
	}
	diff[0] += 5.0 * sin(age + 0.1);

	if (((age/512) % 2)) {
		lastMoveX = (0.98 * lastMoveX) + (0.0010 * game->gameSkill * diff[0]);
		lastMoveY = (0.90 * lastMoveY) + (0.0020 * game->gameSkill * diff[1]);
	} else { //Release The Gnats!
		lastMoveX = (0.90 * lastMoveX) + (0.0003 * game->gameSkill * diff[0]);
		lastMoveY = (0.90 * lastMoveY) + (0.0010 * game->gameSkill * diff[1]);
	}

	pos[0] += game->speedAdj * (lastMoveX);
	pos[1] += game->speedAdj * (lastMoveY + vel[1]);

	if (pos[0] < -config->screenBoundX())
		pos[0] = -config->screenBoundX();
	if (pos[0] >  config->screenBoundX())
		pos[0] =  config->screenBoundX();
}
