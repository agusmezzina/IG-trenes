#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include "SceneData.h"

class UpdateTransformCallback : public osg::NodeCallback
{
public:
	UpdateTransformCallback(SceneData* data);
	virtual ~UpdateTransformCallback();
	virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
protected:
	SceneData* _data;
};

