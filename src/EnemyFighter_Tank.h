// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_TANK_H
#define ENEMYFIGHTER_TANK_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_Tank : public EnemyAircraft
{
public:
    EnemyFighter_Tank(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyAircraft_Tank();

    void	update();
    void	init();
    void	init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

protected:
    void	move();
};

#endif //ENEMYFIGHTER_TANK_H
