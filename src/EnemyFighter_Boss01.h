// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_BOSS01_H
#define ENEMYFIGHTER_BOSS01_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_Boss01 : public EnemyFighter
{
protected:
    void move();

public:
    EnemyFighter_Boss01(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyFighter_Boss00();

    void update();
    void init();
    void init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

};

#endif //BOSS01_H
