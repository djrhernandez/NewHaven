// Created by Junior on 6/14/2016.
#ifndef GROUNDSEGMENT_H
#define GROUNDSEGMENT_H
class Ground;


class GroundSegment
{
public:
	GroundSegment(float p[3], float s[2], Ground *prnt);
	virtual ~GroundSegment();

	virtual void	drawGL() = 0;

	double	pos[3];
	float	size[2];
	float	age;

	GroundSegment *prev;
	GroundSegment *next;

	Ground	*parent;

	friend class Ground;
};

#endif //GROUNDSEGMENT_H
