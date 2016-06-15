// Created by Junior on 6/14/2016.
#ifndef GROUNDMETAL_H
#define GROUNDMETAL_H
#include "windowcompat.h"
#include <GL/gl.h>
#include "Ground.h"
class GroundSegment;

class GroundMetal : public Ground
{
public:
	GroundMetal();
	~GroundMetal();

	void	drawGL();
	void	setVariation(int index);

	void	loadTextures();
	void	deleteTextures();

	float	size;

	friend class GroundSegment;
	friend class GroundMetalSegment;
};

#endif //GROUNDMETAL_H
