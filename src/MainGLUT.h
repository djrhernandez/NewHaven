// Created by Junior on 6/13/2016.
#ifndef MAINGLUT_H
#define MAINGLUT_H
#ifdef USE_GLUT
#include "MainToolkit.h"
#include "MainGL.h"


class MainGLUT : public MainToolkit
{
public:
	MainGLUT(int argc = 0, char **argv = 0);
	~MainGLUT();

	bool run();
	bool checkErrors();
	void setVideoMode();
	void grabMouse(bool);

private:
	static void nullFunc();
	static void drawGame(int);
	static void updateGame();
	static void reshape(int w, int h);
	static void keyboardASCII(unsigned char key, int x, int y);
	static void keyboardSpecial(int key, int x, int y);
	static void mouseEvent(int button, int state, int x, int y);
	static void mouseMotion(int x, int y);

private:
	static int		fire;
	static int		xLast, yLast;
	static int		xMid, yMid;
	static bool		mouseToggle;

	static int last_time;
	static int now_time;

};

#endif //USE_GLUT

#endif //MAINGLUT_H
