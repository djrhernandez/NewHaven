// Created by Junior on 6/14/2016.
#ifndef GROUND_H
#define GROUND_H
#include "windowcompat.h"
#include <GL/gl.h>
class Global;
class GroundSegment;

class Ground
{
public:
	Ground();
	virtual ~Ground();

	enum TexNames { Base, Blip, NumGroundTex };

	virtual void drawGL() = 0;
	virtual void setVariation(int index) = 0;
	void nextVariation();

	virtual void loadTextures() = 0;
	virtual void deleteTextures() = 0;

	friend class GroundMetalSegment;
	friend class GroundSeaSegment;

protected:
	int		variation;
	float	pos[3];
	GLuint	tex[NumGroundTex];

	GroundSegment *rootSegment;

protected:
	Global	*game;
};

#endif //GROUND_H
