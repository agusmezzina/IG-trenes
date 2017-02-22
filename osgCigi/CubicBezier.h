#pragma once
#include <osg/Vec3f>
#include <osg/Quat>
#include <osg/Matrix>
#include <vector>
#include <string>

namespace osgCigi
{
	class CubicBezier
	{
	public:
		osg::Vec3f getPosition(float u) const;
		osg::Vec3f getPositionByArcLength(float t) const;
		osg::Quat getOrientation(float u) const;
		osg::Quat getOrientationByArcLength(float t) const;
		osg::Matrix getMOrientation(float u) const;
		osg::Vec3f getPositiveExtension(float t);
		osg::Vec3f getNegativeExtension(float t);
		/*Builds curve from control points*/
		CubicBezier(osg::Vec3f p0, osg::Vec3f p1, osg::Vec3f p2, osg::Vec3f p3);
		/*Reads control point from file*/
		CubicBezier(std::string filename);
		float getTotalLength();
		virtual ~CubicBezier();
	private:
		osg::Vec3f p0, p1, p2, p3;
		std::vector<float> arcLengths;
		float totalLength;
		void computeLengths();
	};
}

