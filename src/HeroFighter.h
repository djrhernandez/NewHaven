// Created by Junior on 6/11/2016.
#ifndef HEROFIGHTER_H
#define HEROFIGHTER_H
#include <GL/gl.h>
#include "defns.h"
#include "windowcompat.h"
#include "itemScreen.h"
class Global;
class PowerUps;
class EnemyFleet;
#define DEATH_SPIKES 7
#define DEATH_TIME 50


class HeroFighter : public itemScreen
{
public:
    HeroFighter();
    ~HeroFighter();

    void moveEvent(int x, int y);
    void fireGun(bool);
    void shootGun();
    void useItem(int index);
    void useItem();
    void nextItem() {
            useItemArmed = 0.0;
            currentItemIndex = ++currentItemIndex % HERO_ITEMS;
    }

    void dropBomb();
    void doDamage(float d);
    void ammoDamage(float d, float vec[3]);
    void checkCollision(EnemyFleet *fleet);
    void checkPowerUps(PowerUps *powerUps);

    int currentItem() {
            return currentItemIndex;
    }
    float itemArmed() {
            return useItemArmed;
    }

    void setLives(int lives);
    int getLives() {
            return lives;
    }

    void addScore(float score);
    float getScore() {
            return score;
    }

    void addLife(bool score = false);
    void loseLife();
    void startdeath();

    void update();
    void drawGL();
    void reset();
    void newGame();
    void fullHealth();

    inline float getAmmoStock(int index) {
            return ammoStock[index];
    }
    inline float getDamage() {
            return damage;
    }
    inline float getShields() {
            return shields;
    }
    inline bool isVisible() {
            return !dontShow;
    }

    float *getPos();
    inline float getSize(int i) {
            return size[i];
    }

    GLuint heroTexture;
    GLuint bombTexture;

    void loadTextures();
    void deleteTextures();

    friend class StatusDisplay;

protected:
    void setAmmoStock(int, float);
    void deathExplosions();

    float gunFlash0[HERO_AMMO_TYPES];
    float gunFlash1[HERO_AMMO_TYPES];
    float gunActive[HERO_AMMO_TYPES];

private:
    float epos[3];
    float size[2];
    float bound[2][2];

    float deathCircle[DEATH_SPIKES][2];
    float deathStereo;

    int lives;
    int dontShow;
    int superBomb;
    float scoreStep;
    float scoreTarget;

    float ammoStock[HERO_AMMO_TYPES];
    float gunPause[HERO_AMMO_TYPES];
    bool gunSwap;
    bool gunTrigger;

    float damage;
    float shields;
    float score;

    int currentItemIndex;
    float useItemArmed;

    //Secondary Movement
    float move[2];
    float secondaryMove[2];

    inline void drawQuad(float sx, float sy)
    {
            glBegin(GL_TRIANGLE_STRIP);
                glTexCoord2f(1.0, 0.0); glVertex3f( sx,  sy, 0.0);
                glTexCoord2f(0.0, 0.0); glVertex3f(-sx,  sy, 0.0);
                glTexCoord2f(1.0, 1.0); glVertex3f( sx, -sy, 0.0);
                glTexCoord2f(0.0, 1.0); glVertex3f(-sx, -sy, 0.0);
            glEnd();
    }

    //Variables for Tips
    int tipShipPast;
    int tipSuperShield;

private:
    Global *game;
};

#endif //HEROFIGHTER_H
