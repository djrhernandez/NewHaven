// Created by Junior on 6/14/2016.
#ifndef GROUNDSEA_H
#define GROUNDSEA_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "Ground.h"
class GroundSegment;



class GroundSea  : public Ground
{
public:
	GroundSea();
	~GroundSea();

	void	drawGL();
	void	setVariation(int index);

protected:
	float size;
	static float	vert[4][3];
};

#endif //GROUNDSEA_H
