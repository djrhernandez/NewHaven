// Created by Junior on 6/13/2016.
#ifndef ADDITEMSCREEN_H
#define ADDITEMSCREEN_H
#include "itemScreen.h"
#include "EnemyFighter.h"
class Global;
class ItemThing;
class EnemyWave;


class addItemScreen
{
public:
    addItemScreen();
    ~addItemScreen();

    void putScreenItems();
    void killItemScreen(itemScreen *);
    bool loadScreenItems(const char *);
    void clear();

    EnemyFighter *dynamicEnemyAdd(EnemyType et, float *pos,
                                  int relTime);
private:
    void clearDeadPool();
    void addItem(int relTime, itemScreen *newItem);
    void addWave(EnemyWave &wave);

    void loadLevelXXX();
    void loadLevel1();
    void loadLevel2();
    void loadLevel3();

    void addStraightWave(int o, int duration,float density = 1.0);
    void addOmniWave(int o, int duration, float density = 1.0);
    void addStraightArrowWave(int o, int duration, float density = 1.0);
    void addOmniArrowWave(int o, int duration, float density = 1.0);
    void addGnatWave(int o, int duration, float density = 1.0, bool mixed = true);

    void addAmmunition(int o, int duration, int a = 0, int b = 100, int c = 1000);
    void addPowerUps(int 0, int duration, int a = 300, int b = 1200, int c = 1000);

    ItemThing *root;
    EnemyFighter *deadPool[NumEnemyTypes];

private:
    Global *game;
};

class ItemThing
{
public:
    ItemThing();
    ItemThing(int relTime, itemScreen *newScreenItem);

    int releaseTime;
    itemScreen *item;

    ItemThing *next;
};

class EnemyWave
{
public:
    EnemyWave(EnemyType t);
    enum Formation { None, Arrow, NumFormations };

    EnemyType type;
    int begin;
    int end;
    int period;
    int jitter;
    Formation formation;
    float pos[3];
    float xJitter;

    void setInOut(int b, int e)
    {
        begin = b;
        end = e;
        if (e <= b) {
            end = b + 1;
        }
    }
    void setPosition(float x, float y)
    {
        pos[0] = x;
        pos[1] = y;
    }
    void setXRand(float j)
    {
        xJitter = j;
    }
    void setFrequency(int p, int j = 0)
    {
        period = p;
        jitter = j;
    }
    void setFormation(Formation f)
    {
        formation = f;
    }
};

#endif //ADDITEMSCREEN_H
