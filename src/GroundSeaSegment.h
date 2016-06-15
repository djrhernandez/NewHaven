// Created by Junior on 6/14/2016.
#ifndef GROUNDSEASEGMENT_H
#define GROUNDSEASEGMENT_H
#include "GroundSegment.h"
class Ground;
class GroundMetal;


class GroundSeaSegment : public GroundSegment
{
public:
	GroundSeaSegment(float p[3], float s[2], Ground *prnt);
	~GroundSeaSegment();

	void	drawGL();
	friend class Ground;
	friend class GroundMetal;

private:
	float vert[4][3];
};

#endif //GROUNDSEASEGMENT_H
