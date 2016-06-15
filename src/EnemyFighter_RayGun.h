// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_RAYGUN_H
#define ENEMYFIGHTER_RAYGUN_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_RayGun : public EnemyFighter
{
public:
    EnemyFighter_RayGun(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyAircraft_RayGun();

    void update();
    void init();
    void init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

protected:
    void move();
};

#endif //NEW_HAVEN_ENEMYFIGHTER_RAYGUN_H
