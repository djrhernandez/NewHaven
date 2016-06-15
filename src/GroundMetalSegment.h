// Created by Junior on 6/14/2016.
#ifndef GROUNDMETALSEGMENT_H
#define GROUNDMETALSEGMENT_H
#include "GroundSegment.h"
class Ground;
class GroundMetal;


//#define EXPERIMENTAL
class GroundMetalSegment : public GroundSegment
{
public:
	GroundMetalSegment(float p[3], float s[2], Ground *prnt);
	~GroundMetalSegment();

	void	drawGL();

	void	drawBlip(float rep, float S, float tilt, bool blipMirrorT);
	void	drawSurface(float *c0_clr,
						float *c1_clr,
						float *r0_clr,
						float *r1_clr,
						float *r2_clr);

#ifdef EXPERIMENTAL
	void	drawMultiTex(float rep, float S, float tilt, bool blipMirrorT,
						float *c0_clr,
						float *c1_clr,
						float *r0_clr,
						float *r1_clr,
						float *r2_clr);
#endif // EXPERIMENTAL

	friend class Ground;
	friend class GroundMetal;
};

#endif //GROUNDMETALSEGMENT_H
