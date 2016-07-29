//#include "stdafx.h"
#include "UpdateTransformCallback.h"
#include <iostream>
#include <cmath>

UpdateTransformCallback::UpdateTransformCallback(World* data, World* ghost) : _data(data), _ghost(ghost)
{
	prevTime = std::chrono::high_resolution_clock::now();
	startTime = std::chrono::high_resolution_clock::now();
	correctionStep = 0;
	dr = std::make_unique<DeadReckoning>(data, ghost);
	p_1 = _data->getEntity(1).getPosition();
	correcting = false;
	dataFile.open("data.csv");
	started = false;
	usingDR = true;
	x = osg::Vec2f(p_1.y(), 0.0f);
	x_1 = x;
	x_2 = x;
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	bool quadratic = true;

	auto actualTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> time = actualTime - prevTime;
	float deltaT = time.count();
	prevTime = actualTime;
	std::chrono::duration<float> elapsed = actualTime - startTime;

	auto p = _data->getEntity(1).getPosition();
	osg::Vec3f pDraw;

	if ((p.y() != 3.0f) && (!started))
	{
		started = true;
		startTime = std::chrono::high_resolution_clock::now();
	}

	if (usingDR)
	{
		if (p != p_1)
		{
			correcting = true;
			x_2 = x_1;
			x_1 = x;
			x = osg::Vec2f(p.y(), elapsed.count());
			if (calculateAngleOfEmbrace() > 90.0f)
				quadratic = true;
			else
				quadratic = false;
		}

		if (!correcting)
		{
			if (quadratic)
				dr->secondOrderUpdateGhost(1, deltaT);
			else
				dr->firstOrderUpdateGhost(1, deltaT);
		}
		else //correction
		{
			p_1 = p;
			dr->correctGhost(1, correctionStep);
			correctionStep++;
			if (correctionStep > 10)
			{
				correcting = false;
				correctionStep = 0;
			}
		}

		pDraw = _ghost->getEntity(1).getPosition();
	}
	else
		pDraw = p;

	if (started)
	{
		//std::chrono::duration<float> elapsed = actualTime - startTime;
		dataFile << pDraw.y() << ";" << elapsed.count() << ";" << calculateAngleOfEmbrace() << ";" << std::endl;
	}
	
	transformNode->setMatrix(osg::Matrix::translate(pDraw));
	traverse(node, nv);
}

float UpdateTransformCallback::calculateAngleOfEmbrace() const
{
	auto pi = atan(1) * 4;
	auto a = x_2 - x_1;
	auto b = x - x_1;
	auto angleRad = acosf((a * b) / (a.length() * b.length()));
	return angleRad * 180 / pi;
}

UpdateTransformCallback::~UpdateTransformCallback()
{
	dataFile.close();
}
