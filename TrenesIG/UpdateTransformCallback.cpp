//#include "stdafx.h"
#include "UpdateTransformCallback.h"


UpdateTransformCallback::UpdateTransformCallback(World* data) : _data(data)
{
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	double x{ 0 }, y{ 0 }, z{ 0 };
	_data->getEntity(1).getPositionState(x, y, z);
	transformNode->setMatrix(osg::Matrix::translate(osg::Vec3f(x, y, z)));
	traverse(node, nv);
}

UpdateTransformCallback::~UpdateTransformCallback()
{
}
