#include "stdafx.h"
#include "UpdateTransformCallback.h"


UpdateTransformCallback::UpdateTransformCallback(SceneData* data) : _data(data)
{
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	transformNode->setMatrix(osg::Matrix::translate(_data->getData()));
	/*if (!((++_count) % 60) && switchNode)
	{
		switchNode->setValue(0, !switchNode->getValue(0));
		switchNode->setValue(1, !switchNode->getValue(1));
	}*/
	traverse(node, nv);
}

UpdateTransformCallback::~UpdateTransformCallback()
{
}
