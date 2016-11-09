#pragma once
#include <osg\NodeCallback>
#include <osg\MatrixTransform>
#include <chrono>
#include <memory>
#include <fstream>
#include "World.h"
#include "DeadReckoning.h"
#include "CubicBezier.h"

namespace osgCigi
{
	class CigiUpdateCallback : public osg::NodeCallback
	{
	public:

		bool toggleDR();
		int addSmoothness();
		int decreaseSmoothness();
		bool togglePredictionMethod();
		virtual void operator()(osg::Node* node, osg::NodeVisitor* nv);
		CigiUpdateCallback(World* data, World* ghost);
		virtual ~CigiUpdateCallback();
	protected:
		bool modelChanged();

		std::ofstream dataFile;
		std::ofstream logFile;
		std::chrono::system_clock::time_point prevTime;
		std::chrono::system_clock::time_point startTime;
		World* _data;
		World* _ghost;
		int correctionStep;
		std::unique_ptr<DeadReckoning> dr;
		std::unique_ptr<CubicBezier> trajectory;
		osg::Vec3f p_1;
		osg::Vec3f v_1;
		osg::Vec3f a_1;
		bool correcting;
		bool started;
		bool usingDR;
		bool quadratic;
	};
}

