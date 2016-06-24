// Created by Junior on 6/19/2016.
#include "EnemyFighter_Boss00.h"
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

EnemyFighter_Boss00::EnemyFighter_Boss00(EnemyType et, float *p, float randFact)
		: EnemyFighter(et, p, randFact)
{
	init(p, randFact);
}

EnemyFighter_Boss00::~EnemyFighter_Boss00()
{
}

//Gets rid of IRIX compiler warning
void EnemyFighter_Boss00::init()
{
	EnemyFighter::init();
}

//Initial values of Boss00
void EnemyFighter_Boss00::init(float *p, float randFact)
{
	EnemyFighter::init(p, randFact);

	damage = baseDamage = -10000.0 * game->gameSkill;
	size[0] = 3.5;
	size[1] = 2.275;
	collisionMove = 0.05;
	vel[1] = 0.02;
}

//Note: Many of the firing rates are not adjusted by game->speedAdj so they
// 		will not be totally correct.
// 		Should be close enough (for jazz?), though.
void EnemyFighter_Boss00::update()
{
	float v[3] = {0.0, -0.2, 0.0};
	//float *hpos = target->getPos();
	float *hpos = game->hero->getPos();
	float a = hpos[0] - pos[0];
	float b = hpos[1] - pos[1];
	float dist;
	float ammoSpeed = 0.35 * game->speedAdj;

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
	if (fabs(a) < 1.6) { //Big Center Gun
		v[1] = -0.6;
		p[1] = pos[1] - 1.7;
		game->enemyAmmo->addAmmo(3, p, v);
	}

	if (!(age % 5)) { //Side Cannons
		shootSwap = ++shootSwap % 15;
		if (shootSwap < 6) {
			v[1] = -0.2;
			p[1] = pos[1]-1.9;
			p[0] = pos[0]+2.0+((shootSwap % 3) * 0.4);
			game->enemyAmmo->addAmmo(0, p, v);
			p[0] = pos[0]-2.0-((shootSwap % 3) * 0.4);
			game->enemyAmmo->addAmmo(0, p, v);
		}
	}
	if (!((age - 1) % 7)) {
		dist = fabs(a) + fabs(b);
		shootVec[0] = ammoSpeed * a/dist;
		shootVec[1] = ammoSpeed * b/dist;
	}
	if (!((age/200) % 2)) { //Omni Shooters
		if (!((age/100) % 2)) {
			if (!((age/50) % 2)) {
				p[1] = pos[1] - 0.45;
				p[0] = pos[0] - 1.1;
				game->enemyAmmo->addAmmo(1, p, shootVec);
				p[0] = pos[0] + 1.1;
				game->enemyAmmo->addAmmo(1, p, shootVec);
			}
			preFire = (age % 100)/100.0f;
		}
		else if (!(age % 10)) {
			p[0] = pos[0] - 1.1;

			a = hpos[0] - p[0];

			p[1] = pos[1] - 0.45;
			b = hpos[1] - p[1];

			dist = fabs(a) + fabs(b);
			shootVec[0] = 2.0 * ammoSpeed *a/dist;
			shootVec[1] = 2.0 * ammoSpeed *b/dist;
			game->enemyAmmo->addAmmo(2, p, shootVec);

			p[0] = pos[0] + 1.1;
			a = hpos[0] - p[0];

			dist = fabs(a) + fabs(b);
			shootVec[0] = 2.0 * ammoSpeed * a/dist;
			shootVec[1] = 2.0 * ammoSpeed * b/dist;
			game->enemyAmmo->addAmmo(2, p, shootVec);

			preFire -= 0.4;
			if (preFire < 0.0)
				preFire = 0.0;
		}
		else
			preFire += 0.035;
	}
	else
		preFire = 0.0;
}

void EnemyFighter_Boss00::move()
{
	Config *config = Config::instance();
	float *hpos;

	if (target)
		hpos = target->getPos();
	else
		hpos = pos;
	float diff[2] = {hpos[0] - pos[0], hpos[1] - pos[1]};

	float approachDist;
	approachDist = 7.0 * (2.0 - game->gameSkill);

	if (fabs(diff[1]) < (approachDist + 0.0 * sin(game->frame * 0.05)))	{
		diff[1] = diff[1] * diff[1]/approachDist;
	}

	diff[0] += 5.0 * sin(age * 0.1);
	lastMoveX = (0.98 * lastMoveX) + (0.0005 * game->gameSkill * diff[0]);
	lastMoveY = (0.90 * lastMoveY) + (0.001  * game->gameSkill * diff[1]);
	pos[0] += game->speedAdj * (lastMoveX);
	pos[1] += game->speedAdj * (lastMoveY + vel[1]);

	if (pos[0] < -config->screenBoundX())
		pos[0] = -config->screenBoundX();
	if (pos[0] >  config->screenBoundX())
		pos[0] =  config->screenBoundX();
}