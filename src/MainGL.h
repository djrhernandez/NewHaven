// Created by Junior on 6/11/2016.
#ifndef MAINGL_H
#define MAINGL_H
class Global;


class MainGL
{
public:
    MainGL();
    ~MainGL();

    int initGL();
    void drawGL();
    void drawGameGL();
    void drawDeadGL();
    void drawSuccessGL();
    void drawTextGL(char *string, float pulse, float scale);
    void reshapeGL(int w, int h);

    void loadTextures();
    void deleteTextures();

private:
    Global *game;
};

#endif //MAINGL_H
