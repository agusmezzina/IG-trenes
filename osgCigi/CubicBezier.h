#pragma once
#include <osg/Vec3f>
#include <osg/Quat>
#include <osg/Matrix>

namespace osgCigi
{
	class CubicBezier
	{
	public:
		osg::Vec3f getPosition(float u) const;
		osg::Quat getOrientation(float u) const;
		osg::Matrix getMOrientation(float u) const;
		CubicBezier(osg::Vec3f p0, osg::Vec3f p1, osg::Vec3f p2, osg::Vec3f p3);
		virtual ~CubicBezier();
	private:
		osg::Vec3f p0, p1, p2, p3;
	};
}

