//#include "stdafx.h"
#include "UpdateTransformCallback.h"
#include <iostream>

UpdateTransformCallback::UpdateTransformCallback(World* data, World* ghost) : _data(data), _ghost(ghost)
{
	prevTime = std::chrono::high_resolution_clock::now();
	startTime = std::chrono::high_resolution_clock::now();
	correctionStep = 0;
	dr = std::make_unique<DeadReckoning>(data, ghost);
	oldP = _data->getEntity(1).getPosition();
	correcting = false;
	dataFile.open("data.csv");
	started = false;
	usingDR = false;
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);

	auto actualTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time = actualTime - prevTime;
	float deltaT = time.count();
	prevTime = actualTime;

	auto p = _data->getEntity(1).getPosition();
	osg::Vec3f p1;

	if ((p.y() != 3.0f) && (!started))
	{
		started = true;
		startTime = std::chrono::high_resolution_clock::now();
	}

	if (usingDR)
	{
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

		p1 = _ghost->getEntity(1).getPosition();
	}
	else
		p1 = p;

	if (started)
	{
		std::chrono::duration<float> elapsed = actualTime - startTime;
		dataFile << p1.y() << ";" << elapsed.count() << std::endl;
	}
	
	transformNode->setMatrix(osg::Matrix::translate(p1));
	traverse(node, nv);
}

UpdateTransformCallback::~UpdateTransformCallback()
{
	dataFile.close();
}
