// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_STRAIGHT_H
#define ENEMYFIGHTER_STRAIGHT_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_Straight : public EnemyAircraft
{
public:
    EnemyFighter_Straight(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyAircraft_Straight();

    void	update();
    void	init();
    void	init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

protected:
    void	calcShootInterval();
    void	move();
};

#endif //ENEMYFIGHTER_STRAIGHT_H
