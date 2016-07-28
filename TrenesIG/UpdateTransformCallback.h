#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include <chrono>
#include <memory>
#include <iostream>
#include <fstream>
#include "World.h"
#include "DeadReckoning.h"

class UpdateTransformCallback : public osg::NodeCallback
{
public:
	UpdateTransformCallback(World* data, World* ghost);
	virtual ~UpdateTransformCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	std::ofstream dataFile;
	std::chrono::system_clock::time_point prevTime;
	std::chrono::system_clock::time_point startTime;
	World* _data;
	World* _ghost;
	int correctionStep;
	std::unique_ptr<DeadReckoning> dr;
	osg::Vec3f oldP;
	bool correcting;
	bool started;
	bool usingDR;
};

