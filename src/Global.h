// Created by Junior on 6/14/2016.
#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <time.h>
#include "defns.h"
#include "TexFont.h"
#include "MainToolkit.h"
class HeroFighter;
class HeroAmmo;
class EnemyFleet;
class EnemyAmmo;
class Explosions;
class PowerUps;
class Audio;
class Ground;
class MenuGL;
class MainGL;
class StatusDisplay;
class addItemScreen;


class Global
{
public:
    enum GameMode { Game, Menu, LevelOver, HeroDead };
    ~Global();

    static Global *init();
    static Global *getInstance();
    static Global destroy();

    void newGame();
    void nextLevel();   //gotoNextLevel();

    static void generateRandom(bool r = true);

    void createGame();
    void deleteGame();

    void loadTextures();
    void deleteTextures();

    static MainToolkit *toolkit;
    static bool mouseActive;

    static TexFont *texFont;
    static float fps;
    static int frame;
    static int gameFrame;
    static float gameSpeed;
    static float gameSkill;
    static float speedAdj;
    static float scrollSpeed;

    static GameMode gameMode;
    bool game_pause;
    bool game_quit;

    static int heroDeath;
    static int heroSuccess;

    static HeroFighter *hero;
    static EnemyFleet *enemyFleet;
    static HeroAmmo *heroAmmo;
    static EnemyAmmo *enemyAmmo;
    static addItemScreen *addItem;

    static Explosions *explosions;
    static PowerUps *powerUps;
    static Ground *ground;
    static Ground *groundGame;
    static Ground *groundMenu;

    static Audio *audio;
    static MenuGL *menu;
    static MainGL *mainGL;
    static StatusDisplay *statusDisplay;

    static float cursorPos[3];
    static int randI[256];
    static int randF[256];
    static int randS[256];
    static int randIndex;

    static FILE *eventFile;
    static int tipShipPast;
    static int tipSuperShield;

private:
    static Global *instance;
    Global();
};

#endif //GLOBAL_H
