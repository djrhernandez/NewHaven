// Created by Junior on 6/14/2016.
#ifndef ENEMYFLEET_H
#define ENEMYFLEET_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "EnemyFighter.h"
class Global;


class EnemyFleet
{
public:
    EnemyFleet();
    ~EnemyFleet();

    void drawGL();
    void clear();
    void addEnemy(EnemyFighter *);
    void killEnemy(EnemyFighter *);
    void destroyAll();
    void retarget(EnemyType et, itemScreen *newTarget);

    void toFirst();
    EnemyFighter *getShip();

    void update();
    void loadTextures();
    void deleteTextures();

private:
    inline void drawQuad(float sx, float sy)
    {
        glBegin(GL_TRIANGLE_STRIP);
        glTexCoord2f(1.0, 0.0); glVertex3f( sx,  sy, 0.0);
        glTexCoord2f(0.0, 0.0); glVertex3f(-sx,  sy, 0.0);
        glTexCoord2f(1.0, 1.0); glVertex3f( sx, -sy, 0.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-sx, -sy, 0.0);
        glEnd();
    }

    void	bossExplosion(EnemyFighter *);

private:
    GLuint			shipTexture[NumEnemyTypes];
    GLuint			extraTexture[NumEnemyTypes];

    EnemyAircraft	*squadRoot;
    EnemyAircraft	*currentShip;

private:
    Global	*game;
};

#endif //ENEMYFLEET_H
