// Created by Junior on 6/14/2016.
#ifndef MENUGL_H
#define MENUGL_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "TexFont.h"
#include "MainToolkit.h"
class Global;


class MenuGL
{
public:
	enum MenuSelection {
		NewGame, GameLevel, SkillLevel, Graphics,
		FullScreen, ScreenSize, Sound, Music,
		MouseSpeed, Quit, NumSelections };

	MenuGL();
	~MenuGL();

	void	startMenu();
	void	keyHit(MainToolkit::Key key);
	void	mousePress(MainToolkit::Button but, int x, int y);

	void	drawGL();
	void	loadTextures();
	void	deleteTextures();

protected:
	void	activateItem();
	void	incItem();
	void	decItem();

	void	drawIndicator();
	void	drawElectric();

	void	drawTitle();
	void	drawTitleBack();

	void	createLists(bool thick);
private:
	MenuSelection curSel;

	char *menuText[NumSelections];

	GLuint	listNewHaven;
	GLuint	listBSU;

	GLuint	envTex;
	GLuint	csrTex;
	GLuint	backTex;
	GLuint	elecTex;
	GLuint	updwnTex;

	float	elecOffX;
	float	elecOffY;
	float	elecStretch;
	float	textAngle;
	int		textCount;

	float	txtHeight;
	float	butHeight;
	float	butWidth;
	float	butOffset;

	bool	thickText;

	float 	titleTilt;

	float	mssgAlpha;
	char	mssgText[256];
	int		mssgIndex;
	int		mssgCount;
	bool	mssgHelpOverride;

private:
	Global	*game;
};



#endif //MENUGL_H
