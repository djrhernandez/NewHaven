// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_GNAT_H
#define ENEMYFIGHTER_GNAT_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;
class EnemyFighter_Boss01;


class EnemyFighter_Gnat : public EnemyFighter
{
    EnemyFighter_Gnat(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyAircraft_Gnat();

    void	update();
    void	init();
    void	init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

protected:
    void	calcShootInterval();
    void	move();

};

#endif //ENEMYFIGHTER_GNAT_H
