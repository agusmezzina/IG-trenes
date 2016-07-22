//#include "stdafx.h"
#include "UpdateTransformCallback.h"
#include <iostream>

UpdateTransformCallback::UpdateTransformCallback(World* data, World* ghost) : _data(data), _ghost(ghost)
{
	prevTime = std::chrono::high_resolution_clock::now();
	realY = _data->getEntity(1).getPosition().y();
	oldP = _data->getEntity(1).getPosition();
	predict = true;
	step = 0;
	smoothness = 20;
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	bool dr = true;
	osg::Vec3f p1;

	if (dr)
	{
		auto actualTime = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> time = actualTime - prevTime;
		float deltaT = time.count();
		prevTime = actualTime;

		//float newY = _data->getEntity(1).getPosition().y();
		auto p = _data->getEntity(1).getPosition();


		//if (newY == realY)
		if (p == oldP)
		{
			auto p0 = _ghost->getEntity(1).getPosition();
			auto v0 = _ghost->getEntity(1).getVelocity();
			_ghost->updateEntityPosition(1, osg::Vec3f(0.0f, p0.y() + v0.y()*deltaT, 0.0f));
		}
		else //correction
		{
			oldP = oldP + (p - oldP) / smoothness;
			//_ghost->updateEntityPosition(1, _data->getEntity(1).getPosition());
			_ghost->updateEntityPosition(1, oldP);
			_ghost->updateEntityVelocity(1, _data->getEntity(1).getVelocity());
		}
		p1 = _ghost->getEntity(1).getPosition();
	}
	else
		p1 = _data->getEntity(1).getPosition();
	//std::cout << deltaT << std::endl;
	//std::cout << v1.y() << std::endl;
	
	//auto p1 = _data->getEntity(1).getPosition();
	
	transformNode->setMatrix(osg::Matrix::translate(p1));
	traverse(node, nv);
}

UpdateTransformCallback::~UpdateTransformCallback()
{
}
