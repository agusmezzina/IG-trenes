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
	logFile.open("logIG.txt");
	started = false;
	usingDR = true;
	x = osg::Vec2f(p_1.y(), 0.0f);
	x_1 = x;
	x_2 = x;
}

void UpdateTransformCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	bool quadratic = false;

	auto actualTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaT = actualTime - prevTime;
	prevTime = actualTime;
	std::chrono::duration<float> elapsed = actualTime - startTime;

	auto p = _data->getEntity(1).getPosition();
	osg::Vec3f pDraw;

	if ((p.y() != 0.0f) && (!started))
	{
		started = true;
		startTime = std::chrono::high_resolution_clock::now();
	}

	if (usingDR)
	{
		if (p != p_1)
		{
			p_1 = p;
			dr->correctGhost(1);
			logFile << "Corrected" << std::endl;

		}
		else
		{
			if (quadratic)
			{
				auto entity = _ghost->getEntity(1);
				auto pg = entity.getPosition();
				auto vg = entity.getVelocity();
				auto ag = entity.getAcceleration();
				logFile << pg.y() << "; " << vg.y() << "; " << ag.y() << "; " << deltaT.count() << "; " << elapsed.count() << std::endl;
				dr->secondOrderUpdateGhost(1, deltaT.count());
			}
			else
				dr->firstOrderUpdateGhost(1, deltaT.count());
		}

		pDraw = _ghost->getEntity(1).getPosition();
	}
	else
		pDraw = p;

	if (started)
	{
		//std::chrono::duration<float> elapsed = actualTime - startTime;
		dataFile << pDraw.y() << ";" << elapsed.count() << /*";" << calculateAngleOfEmbrace() << ";" << */std::endl;
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
	logFile.close();
}
