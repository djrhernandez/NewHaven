// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_OMNI_H
#define ENEMYFIGHTER_OMNI_H
#include "EnemyFighter.h"
class ActiveAmmo;
class Global;


class EnemyFighter_Omni : public EnemyFighter
{
public:
    EnemyFighter_Omni(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyAircraft_Omni();

    void	update();
    void	init();
    void	init(float *p, float randFact = 1.0);

    friend class EnemyFleet;
    friend class addItemScreen;

protected:
    void	move();
};

#endif //ENEMYFIGHTER_OMNI_H
