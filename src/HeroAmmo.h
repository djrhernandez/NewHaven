// Created by Junior on 6/11/2016.
#ifndef HEROAMMO_H
#define HEROAMMO_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "defns.h"
#include "Ammo.h"
class Global;
class EnemyFleet;


class HeroAmmo
{
public:
    HeroAmmo();
    ~HeroAmmo();

    void addAmmo(int type, float position[3]);
    void updateAmmo();

    void checkForHits(EnemyFleet *fleet);

    void printList(int type);
    void drawGL();
    void clear();

    void loadTextures();
    void deleteTextures();

protected:
    ActiveAmmo *getNewAmmo();
    void killAmmo(ActiveAmmo *dead);

private:
    ActiveAmmo *ammoRoot[HERO_AMMO_TYPES];
    ActiveAmmo *ammoPool;
    float ammoSize[HERO_AMMO_TYPES][2];
    float ammoDamage[HERO_AMMO_TYPES];

    GLuint ammoTexture[HERO_AMMO_TYPES];

private:
    Global *game;

};

#endif //HEROAMMO_H
