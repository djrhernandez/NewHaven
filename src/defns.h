// Created by Junior on 6/11/2016.
#ifndef DEFNS_H
#define DEFNS_H

#define SRAND (Global::randS[(++Global::randIndex)%256])
#define FRAND (Global::randF[(++Global::randIndex)%256])
#define IRAND (Global::randI[(++Global::randIndex)%256])

#define AMMO_REFILL 150
#define HERO_Z 25.0f

#define SCREEN_SIZE_MIN 0   // Smallest res = 512 x 384
#define SCREEN_SIZE_MAX 4   // Greatest res = 1280 x 980

#define ENEMY_AMMO_TYPES 5
#define HERO_AMMO_TYPES 3
#define HERO_ITEMS 1

#define HERO_DAMAGE -500.0
#define HERO_SHIELDS 500.0

#define HISCORE_HISTORY 5

#endif //DEFNS_H

