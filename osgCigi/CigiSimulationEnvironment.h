#pragma once
#include <osg\Group>
#include <memory>
#include "CigiIG.h"
#include "World.h"
#include "CigiUpdateCallback.h"

namespace osgCigi
{
	class CigiSimulationEnvironment
	{
	public:
		void registerModel(/*const int& entityID, */const std::string& path);
		osg::Group* createSimulationScene();
		void start();
		bool toggleDR();
		int addSmoothness();
		int decreaseSmoothness();
		bool togglePredictionMethod();
		CigiSimulationEnvironment();
		virtual ~CigiSimulationEnvironment();
	private:
		osg::ref_ptr<osg::Group> simSubtree;
		osg::ref_ptr<CigiUpdateCallback> cb;
		std::unique_ptr<World> world;
		std::unique_ptr<World> ghost;
		std::unique_ptr<CigiIG> network;
	};
}

