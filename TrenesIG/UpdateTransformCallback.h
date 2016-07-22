#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include <chrono>
#include "World.h"
#include "DeadReckoning.h"

class UpdateTransformCallback : public osg::NodeCallback
{
public:
	UpdateTransformCallback(World* data, World* ghost);
	virtual ~UpdateTransformCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	std::chrono::system_clock::time_point prevTime;
	float realY;
	osg::Vec3f oldP;
	World* _data;
	World* _ghost;
	bool predict;
	int step;
	int smoothness;
};

