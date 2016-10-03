#pragma once
#include <osg\Group>
#include <memory>
#include "CigiNetworkManager.h"
#include "World.h"

class CigiSimulationEnvironment
{
public:
	void registerModel(const int& entityID, const std::string& path);
	osg::Group* createSimulationScene();
	void start();
	CigiSimulationEnvironment();
	virtual ~CigiSimulationEnvironment();
private:
	osg::ref_ptr<osg::Group> simSubtree;
	std::unique_ptr<World> world;
	std::unique_ptr<World> ghost;
	std::unique_ptr<CigiNetworkManager> network;
};

