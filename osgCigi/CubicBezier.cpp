#include "CubicBezier.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <boost\algorithm\string.hpp>
//#include <osg/Matrix>

using namespace osgCigi;

osg::Vec3f CubicBezier::getPosition(float u) const
{
	//u = u - floorf(u);
	return p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
}

osg::Vec3f CubicBezier::getPositionByArcLength(float t) const
{
	//t = t - floorf(t);
	auto targetLength = t * this->totalLength;
	
	auto it = std::upper_bound(arcLengths.begin(), arcLengths.end(), targetLength);
	if (it != arcLengths.begin()) {
		it--; // not at end of array so rewind to previous item
	}
	else {
		it = arcLengths.end(); // no items before this point, so return end()
	}

	auto index = it - arcLengths.begin();
	auto lengthBefore = *it;
	if (lengthBefore == targetLength) {
		return this->getPosition(index / 100);
	}
	else {
		float mapU = (index + (targetLength - lengthBefore) / (arcLengths[index + 1] - lengthBefore)) / 100;
		return this->getPosition(mapU);
	}
}

osg::Quat CubicBezier::getOrientation(float u) const
{
	//u = u - floorf(u);
	osg::Vec3f p(1.0f, 0.0f, 0.0f); /*= p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);*/
	osg::Vec3f v = (p1 - p0) * 3 * powf(1 - u, 2) + (p2 - p1) * 6 * (1 - u) * u + (p3 - p2) * 3 * powf(u, 2);
	
	osg::Quat rot;

	if ((p.length() == 0) || (v.length() == 0))
		rot.makeRotate(0, osg::Y_AXIS);
	else
		rot.makeRotate(p, v);
	return rot;
}

osg::Quat CubicBezier::getOrientationByArcLength(float t) const
{
	auto targetLength = t * this->totalLength;

	auto it = std::upper_bound(arcLengths.begin(), arcLengths.end(), targetLength);
	if (it != arcLengths.begin()) {
		it--; // not at end of array so rewind to previous item
	}
	else {
		it = arcLengths.end(); // no items before this point, so return end()
	}

	auto index = it - arcLengths.begin();
	auto lengthBefore = *it;
	if (lengthBefore == targetLength) {
		return this->getOrientation(index / 100);
	}
	else {
		float mapU = (index + (targetLength - lengthBefore) / (arcLengths[index + 1] - lengthBefore)) / 100;
		return this->getOrientation(mapU);
	}
}

osg::Matrix CubicBezier::getMOrientation(float u) const
{
	osg::Vec3f p = p0 * powf(1 - u, 3) + p1 * 3 * powf(1 - u, 2) * u + p2 * 3 * (1 - u) * powf(u, 2) + p3 * powf(u, 3);
	osg::Vec3f v = (p1 - p0) * 3 * powf(1 - u, 2) + (p2 - p1) * 6 * (1 - u) * u + (p3 - p2) * 3 * powf(u, 2);
	osg::Matrix matrix;
	osg::Quat quad;

	matrix.makeLookAt(p, v, osg::Y_AXIS);
	return matrix;
}

osg::Vec3f CubicBezier::getPositiveExtension(float t)
{
	auto dir = (p3 - p2);
	dir.normalize();
	return dir * t + p3;
}

osg::Vec3f CubicBezier::getNegativeExtension(float t)
{
	auto dir = p0 - p1;
	dir.normalize();
	return dir * t + p0;
}

float CubicBezier::getTotalLength()
{
	return this->totalLength;
}

CubicBezier::CubicBezier(std::string filename) : arcLengths(101)
{
	std::ifstream config;
	config.open(filename);
	if (config.is_open())
	{
		std::string line;
		std::vector<std::string> fields;
		std::getline(config, line);
		boost::split(fields, line, boost::is_any_of(";"));
		this->p0 = osg::Vec3f(std::stof(fields[0]), std::stof(fields[1]), std::stof(fields[2]));
		std::getline(config, line);
		boost::split(fields, line, boost::is_any_of(";"));
		this->p1 = osg::Vec3f(std::stof(fields[0]), std::stof(fields[1]), std::stof(fields[2]));
		std::getline(config, line);
		boost::split(fields, line, boost::is_any_of(";"));
		this->p2 = osg::Vec3f(std::stof(fields[0]), std::stof(fields[1]), std::stof(fields[2]));
		std::getline(config, line);
		boost::split(fields, line, boost::is_any_of(";"));
		this->p3 = osg::Vec3f(std::stof(fields[0]), std::stof(fields[1]), std::stof(fields[2]));
	}
	config.close();
	computeLengths();
}

CubicBezier::CubicBezier(osg::Vec3f p0, osg::Vec3f p1, osg::Vec3f p2, osg::Vec3f p3) : arcLengths(101)
{
	this->p0 = p0;
	this->p1 = p1;
	this->p2 = p2;
	this->p3 = p3;

	computeLengths();
}

void CubicBezier::computeLengths()
{
	int len = 100;
	float clen = 0;
	arcLengths[0] = 0;
	auto pi = this->getPosition(0);
	for (int i = 1; i <= len; i++)
	{
		auto p = this->getPosition(i * 0.01);
		auto dp = p - pi;
		clen += dp.length();
		arcLengths[i] = clen;
		pi = p;
	}
	this->totalLength = clen;
}

CubicBezier::~CubicBezier()
{
}