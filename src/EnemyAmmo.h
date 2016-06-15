// Created by Junior on 6/11/2016.
#ifndef ENEMYAMMO_H
#define ENEMYAMMO_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "defns.h"
#include "Ammo.h"
class Global;
class HeroFighter;


class EnemyAmmo
{
public:
    EnemyAmmo();
    ~EnemyAmmo();

    void addAmmo(int type, float pos[3], float vel[3]);
    void updateAmmo();

    void checkForHits(HeroFighter *hero);

    void drawGL();
    void clear();

    void loadTextures();
    void deleteTextures();

protected:
    ActiveAmmo *getNewAmmo();
    void killAmmo(ActiveAmmo *dead);

private:
    ActiveAmmo *ammoRoot[ENEMY_AMMO_TYPES];
    ActiveAmmo *ammoPool;
    float ammoSize[ENEMY_AMMO_TYPES][2];
    float ammoDamage[ENEMY_AMMO_TYPES];

    GLuint ammoTexture[ENEMY_AMMO_TYPES];

private:
    Global *game;
};

#endif //ENEMYAMMO_H
