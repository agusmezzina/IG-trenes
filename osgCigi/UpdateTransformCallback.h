#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include <chrono>
#include <memory>
#include <fstream>
#include "World.h"
#include "DeadReckoning.h"

class UpdateTransformCallback : public osg::NodeCallback
{
public:
	UpdateTransformCallback(World* data, World* ghost);
	virtual ~UpdateTransformCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
	float calculateAngleOfEmbrace() const;
protected:
	bool modelChanged();

	std::ofstream dataFile;
	std::ofstream logFile;
	std::chrono::system_clock::time_point prevTime;
	std::chrono::system_clock::time_point startTime;
	World* _data;
	World* _ghost;
	int correctionStep;
	std::unique_ptr<DeadReckoning> dr;
	osg::Vec3f p_1;
	osg::Vec3f v_1;
	osg::Vec3f a_1;
	bool correcting;
	bool started;
	bool usingDR;
	osg::Vec2f x_2;
	osg::Vec2f x_1;
	osg::Vec2f x;
};

