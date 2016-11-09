#include "CubicBezier.h"
#include <cmath>

osg::Vec3f CubicBezier::getPosition(float u) const
{
	return p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
}

float CubicBezier::getOrientation(float u) const
{
	return 0;
}

CubicBezier::CubicBezier(osg::Vec3f p0, osg::Vec3f p1, osg::Vec3f p2, osg::Vec3f p3)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;
}


CubicBezier::~CubicBezier()
{
}
