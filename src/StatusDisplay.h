// Created by Junior on 6/11/2016.
#ifndef STATUSDISPLAY_H
#define STATUSDISPLAY_H
#include <GL/gl.h>
#include "windowcompat.h"
#include "HeroFighter.h"
class Global;


class StatusDisplay
{
public:
    StatusDisplay();
    ~StatusDisplay();

    void darkenGL();
    void drawGL(HeroFighter *hero);

    void setAmmoAlpha(float in)
    {
        ammoAlpha = in;
    }
    void setDamageAlpha(float in)
    {
        damagaAlpha = in;
    }
    void setShieldAlpha(float in)
    {
        shieldAlpha = in;
    }
    void enemyWarning(float v)
    {
        if (v > enemyWarn) {
            enemyWarn = v;
        }
    }

private:
    GLuint statTexture;
    GLuint shieldTexture;
    GLuint topTexture;
    GLuint heroSuperTexture;
    GLuint heroShieldTexture;
    GLuint heroAmmoFlash[HERO_AMMO_TYPES];
    GLuint useFocus;
    GLuint useItem[HERO_ITEMS];

    float ammoAlpha;
    float damageAlpha;
    float shieldAlpha;
    bool blink;

    inline void drawQuad(float sx, float sy)
    {
        glBegin(GL_TRIANGLE_STRIP);
            glTexCoord2f(1.0, 0.0); glVertex3f( sx, sy, 0.0);
            glTexCoord2f(0.0, 0.0); glVertex3f(-sx, sy, 0.0);
            glTexCoord2f(1.0, 1.0); glVertex3f( sx,-sy, 0.0);
            glTexCoord2f(0.0, 1.0); glVertex3f(-sx,-sy, 0.0);
        glEnd();
    }

    int tipShipShow;
    int tipSuperShow;
    float enemyWarn;

private:
    Global *game;
};
#endif //STATUSDISPLAY_H
