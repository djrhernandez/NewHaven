// Created by Junior on 6/11/2016.
#ifndef POWERUPS_H
#define POWERUPS_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "itemScreen.h"
#define WOBBLE0 45
#define WOBBLE1 75
class Global;
class PowerUp;


class PowerUps
{
public:
    enum Type { Shields, SuperShields, Repair, HeroAmmo00,
    HeroAmmo01, HeroAmmo02, NumPowerUps};

    PowerUps();
    ~PowerUps();

    void addPowerUp(PowerUp*);

    void update();
    void drawGL();
    void clear();

    PowerUp *getFirst();
    PowerUp *getNext();
    void remove(PowerUp*);

    void loadTextures();
    void deleteTextures();

private:
    PowerUp *PUPRoot;
    PowerUp *currentPUP;
    float PUPSize[NumPowerUps][2];
    float PUPColor[NumPowerUps][4];

    Gluint Texture[NumPowerUps];
    Gluint powerUpTexture;

    float speed;
    float wobble0[WOBBLE0];
    float wobble1[WOBBLE1];

    int activeCount;

private:
    Global *game;
};

class PowerUp : public itemScreen
{
    PowerUp(PowerUps::Type t, float position[3],
            float pwr = 1.0, float *v = 0);
    ~PowerUp();

    PowerUps::Type type;
    int thisCount() {
        return count_this;
    }
    float power;

    void seal();
    PowerUp *next;
    PowerUp *prev;

    friend class PowerUps;

private:
    static int count_this;
};

#endif //POWERUPS_H