//#include "stdafx.h"
#include "UpdateTransformCallback.h"
#include <iostream>

UpdateTransformCallback::UpdateTransformCallback(World* data, World* ghost) : _data(data), _ghost(ghost)
{
	prevTime = std::chrono::high_resolution_clock::now();
	correctionStep = 0;
	dr = std::make_unique<DeadReckoning>(data, ghost);
	oldP = _data->getEntity(1).getPosition();
	bool correcting = false;
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);

	auto actualTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time = actualTime - prevTime;
	float deltaT = time.count();
	prevTime = actualTime;

	auto p = _data->getEntity(1).getPosition();

	if (p != oldP)
		correcting = true;

	if (!correcting)
		dr->updateGhost(1, deltaT);
	else //correction
	{
		oldP = p;
		dr->correctGhost(1, correctionStep);
		correctionStep++;
		if (correctionStep > 10)
		{
			correcting = false;
			correctionStep = 0;
		}
	}
	
	auto p1 = _ghost->getEntity(1).getPosition();
	
	transformNode->setMatrix(osg::Matrix::translate(p1));
	traverse(node, nv);
}

UpdateTransformCallback::~UpdateTransformCallback()
{
}
