// Created by Junior on 6/13/2016.
#ifndef ENEMYFIGHTER_H
#define ENEMYFIGHTER_H
#include <GL/gl.h>
#include "itemScreen.h"
class ActiveAmmo;
class Global;
class EnemyFighter_Boss01;
enum EnemyType { EnemyStraight, EnemyOmni, EnemyRayGun, EnemyTank,
    EnemyGnat, EnemyBoss00, EnemyBoss01, NumEnemyTypes };


class EnemyFighter : public itemScreen
{
public:
    EnemyFighter(EnemyType et, float p[3], float randFact = 1.0);
    virtual ~EnemyFighter();

    virtual void update() = 0;
    virtual void init();
    virtual void init(float *p, float randFact = 1.0);

    bool checkHit(ActiveAmmo *ammo);
    void setTarget(itemScreen *t) { target = t; }

    void drawGL(GLuint texture, GLuint extraTexture);

    EnemyType type;
    float size[2];
    float damage;
    float baseDamage;
    float collisionMove;
    float secondaryMove[2];
    float preFire;

    static EnemyFighter *makeNewEnemy(EnemyType et, float p[3], float randFact = 1.0);
protected:
    virtual void calcShootInterval();
    virtual void move() = 0;

protected:
    EnemyFighter *next;
    EnemyFighter *prev;
    EnemyFighter *over;

    float shootVec[3];
    int shootPause;
    int shootInterval;
    int shootSwap;

    float randMoveX;
    float lastMoveX;
    float lastMoveY;

    float speedAdj;
    itemScreen *target;

protected:
    Global *game;

    inline void drawQuad(float sx, float sy)
    {
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1.0, 0.0); glVertex3f( sx,  sy, 0.0);
            glTexCoord2f(0.0, 0.0); glVertex3f(-sx,  sy, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f( sx, -sy, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(-sx, -sy, 0.0);
        glEnd();
    }

private:
    static int allocated;
public:
    static void printNumAllocated(void);

    friend class EnemyFleet;
    friend class addItemScreen;
    friend class EnemyFighter_Boss01;
};

#endif //ENEMYFIGHTER_H
