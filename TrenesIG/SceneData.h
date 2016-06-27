#pragma once
#include <OpenThreads\Mutex>
#include <OpenThreads\ScopedLock>
#include <osg\Vec3d>

class SceneData
{
public:
	SceneData();
	virtual ~SceneData();
	void setData(double x, double y, double z);
	osg::Vec3d getData() const;
protected:
	osg::Vec3d data;
	mutable OpenThreads::Mutex m;
};

