//#include "stdafx.h"
#include "SceneData.h"


SceneData::SceneData() : data(osg::Vec3f())
{
}

void SceneData::setData(double x, double y, double z){
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
	data = osg::Vec3f(x, y, z);
}

osg::Vec3d SceneData::getData() const{
	OpenThreads::ScopedLock<OpenThreads::Mutex> lock(m);
	return data;
}

SceneData::~SceneData()
{
}
