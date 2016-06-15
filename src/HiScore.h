/* Created by Junior on 6/11/2016.
 * Purpose:
 *      Keep track of high scores.
 *      Also keep a High Scores List using HISCORE_HISTORY
 *      to retain the top 5 high scores for each skill level.
 *      Singleton
 */
#ifndef HISCORE_H
#define HISCORE_H
#include <stdio.h>
#include <time.h>
#include "defns.h"

class HiScore
{
public:
    ~HiScore();

    static HiScore *init();
    static HiScore *getInstance();
    static void destroy();

    double getScore(int skill, int index);
    const char *getName(int skill, int index);
    time_t getDate(int skill, int index);

    const char *getFileName();
    bool readFile();
    bool saveFile();
    int set(int skill, float score);
    int check(int skill, float score);
    void print(int skill);

private:
    void insertScore(int skill, int rank, float score);

private:
    static HiScore *instance;
    double hiScore[10][HISCORE_HISTORY];
    char hiScoreName[10][HISCORE_HISTORY][64];
    time_t hiScoreDate[10][HISCORE_HISTORY];

    HiScore();
};

#endif //HISCORE_H
