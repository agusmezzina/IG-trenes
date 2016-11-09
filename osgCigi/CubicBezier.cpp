#include "CubicBezier.h"
#include <cmath>
//#include <osg/Matrix>

using namespace osgCigi;

osg::Vec3f CubicBezier::getPosition(float u) const
{
	return p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
}

osg::Quat CubicBezier::getOrientation(float u) const
{
	//osg::Vec3f p = p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
	osg::Vec3f v = (p1 - p0) * 3 * powf(1 - u, 2) + (p2 - p1) * 6 * (1 - u) * u + (p3 - p2) * 3 * powf(u, 2);
	osg::Matrix matrix;
	osg::Quat quad;

	matrix.makeLookAt(osg::Vec3d(), v, osg::Y_AXIS);
	matrix.get(quad);
	return quad;
}

osg::Matrix CubicBezier::getMOrientation(float u) const
{
	//osg::Vec3f p = p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
	osg::Vec3f v = (p1 - p0) * 3 * powf(1 - u, 2) + (p2 - p1) * 6 * (1 - u) * u + (p3 - p2) * 3 * powf(u, 2);
	osg::Matrix matrix;
	osg::Quat quad;

	matrix.makeLookAt(osg::Vec3d(), v, osg::Y_AXIS);
	return matrix;
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