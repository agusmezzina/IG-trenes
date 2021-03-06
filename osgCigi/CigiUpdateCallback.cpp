//#include "stdafx.h"
#include "CigiUpdateCallback.h"
#include <iostream>
#include <cmath>
# define PI 3.14159265358979323846

using namespace osgCigi;

CigiUpdateCallback::CigiUpdateCallback(World* data, World* ghost) : _data(data), _ghost(ghost)
{
	prevTime = std::chrono::high_resolution_clock::now();
	startTime = std::chrono::high_resolution_clock::now();
	correctionStep = 0;
	dr = std::make_unique<DeadReckoning>(data, ghost);
	/*trajectory = std::make_unique<CubicBezier>(
		osg::Vec3f(0.0f, 0.0f, 0.0f),
		osg::Vec3f(0.0f, 0.0f, 100.0f),
		osg::Vec3f(50.0f, 0.0f, 100.0f),
		osg::Vec3f(100.0f, 0.0f, 100.0f));*/
	trajectory = std::make_unique<osgCigi::CubicBezier>("C:\\ObjetosVarios\\curve.txt");
	p_1 = _data->getEntity(1).getPosition();
	v_1 = _data->getEntity(1).getVelocity();
	a_1 = _data->getEntity(1).getAcceleration();
	correcting = false;
	dataFile.open("dataIG.csv");
	logFile.open("logIG.txt");
	started = false;
	usingDR = true;
	quadratic = false;
}

bool CigiUpdateCallback::modelChanged()
{
	auto p = _data->getEntity(1).getPosition();
	auto v = _data->getEntity(1).getVelocity();
	auto a = _data->getEntity(1).getAcceleration();
	bool result = (p != p_1) || (v != v_1) || (a != a_1);
	if (result)
		p_1 = p;
		v_1 = v;
		a_1 = a;
	return result;
}

bool CigiUpdateCallback::toggleDR()
{
	usingDR = !usingDR;
	return usingDR;
}

int CigiUpdateCallback::addSmoothness()
{
	int smooth = dr->getSmoothness();
	dr->setSmoothness(smooth + 1);
	return smooth + 1;
}

int CigiUpdateCallback::decreaseSmoothness()
{
	int smooth = dr->getSmoothness();
	if (smooth > 1)
		dr->setSmoothness(smooth - 1);
	return dr->getSmoothness();
}

bool CigiUpdateCallback::togglePredictionMethod()
{
	quadratic = !quadratic;
	return quadratic;
}

void CigiUpdateCallback::operator()(osg::Node* node, osg::NodeVisitor* nv){
	osg::MatrixTransform* transformNode = static_cast<osg::MatrixTransform*>(node);
	auto currentTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaT = currentTime - prevTime;
	prevTime = currentTime;
	std::chrono::duration<float> elapsed = currentTime - startTime;
	auto p = _data->getEntity(1).getPosition();
	osg::Vec3f pDraw;
	osg::Quat alphaDraw;
	float uDraw = 0;
	bool changed = modelChanged();

	if (changed && !started)
	{
		logFile << "Started" << std::endl;
		started = true;
		startTime = std::chrono::high_resolution_clock::now();
		dr->compensateAndCorrectGhost(1);
		/*auto pg = _ghost->getEntity(1).getPosition();
		logFile << "Ghost Pos = (" << pg.x() << "; " << pg.y() << "; " << pg.z() << ")" << std::endl;*/
	}
	else
	{
		if (usingDR)
		{
			if (changed)
			{
				correcting = true;
				dr->setConvergencePoint(1, dr->getSmoothness() * deltaT.count());
				/*auto cp = dr->getConvergencePoint();
				auto pg = _ghost->getEntity(1).getPosition();
				logFile << "Time = " << elapsed.count() << 
					"; " << "Ghost Pos = (" << pg.x() << "; " << pg.y() << "; " << pg.z() << ")" <<
					"; " << "Conv. Point = (" << cp.x() << "; " << cp.y() << "; " << cp.z() << ")" << std::endl;*/
				correctionStep = 1;
			}

			if (correcting)
			{
				/*dr->compensateAndCorrectGhost(1);
				auto p = _ghost->getEntity(1).getPosition();
				auto v = _ghost->getEntity(1).getVelocity();
				auto a = _ghost->getEntity(1).getAcceleration();
				logFile << "Correcting Time = " << elapsed.count() <<
					"; model = P=(" << p.x() << "; " << p.y() << "; " << p.z() << ") " <<
					"V=(" << v.x() << "; " << v.y() << +"; " << v.z() << ") " <<
					"A=(" << a.x() << "; " << a.y() << +"; " << a.z() << ") " << std::endl;
				correcting = false;*/

				dr->correctGhost(1, correctionStep);
				correctionStep++;
				if (correctionStep > dr->getSmoothness())
				{
					correcting = false;
					correctionStep = 0;
				}
			}
			else
			{
				if (quadratic)
					dr->secondOrderUpdateGhost(1, deltaT.count());
				else
					dr->firstOrderUpdateGhost(1, deltaT.count());
			}

			//pDraw = _ghost->getEntity(1).getPosition();
			uDraw = _ghost->getEntity(1).getPosition().x();
		}
		else
		{
			logFile << "Time = " << elapsed.count() <<
				"; " << "Pos = (" << p.x() << "; " << p.y() << "; " << p.z() << ")" << std::endl;
			//pDraw = p;
			uDraw = p.x();
			
		}
	}

	if (started)
	{
		auto duration = std::chrono::high_resolution_clock::now().time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		dataFile << millis % 100000 << ";" << uDraw << ";" << pDraw.x() << "; " << pDraw.z() << std::endl;
	}
	pDraw = trajectory->getPosition(uDraw);
	alphaDraw = trajectory->getOrientation(uDraw);
	osg::Matrix mat;
	mat.setTrans(pDraw);
	mat.setRotate(alphaDraw);
	transformNode->setMatrix(mat);
	//transformNode->setMatrix(osg::Matrix::rotate(alphaDraw, osg::Vec3f(0.0f, 1.0f, 0.0f)) * osg::Matrix::translate(pDraw));
	traverse(node, nv);
}

CigiUpdateCallback::~CigiUpdateCallback()
{
	dataFile.close();
	logFile.close();
}
