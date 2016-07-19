#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include "World.h"

class UpdateTransformCallback : public osg::NodeCallback
{
public:
	UpdateTransformCallback(World* data);
	virtual ~UpdateTransformCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	World* _data;
};

