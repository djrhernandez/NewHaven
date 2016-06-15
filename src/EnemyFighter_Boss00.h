// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_BOSS00_H
#define ENEMYFIGHTER_BOSS00_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_Boss00 : public EnemyFighter
{
protected:
    void move();

public:
    EnemyFighter_Boss00(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyFighter_Boss00();

    void update();
    void init();
    void init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

};

#endif //ENEMYBOSS01_H
