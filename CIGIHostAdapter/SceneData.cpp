#include "SceneData.h"
#include <iostream>

SceneData::SceneData()
{	
	simulationData.push(EntityState(0, 0.5, 0));
}

EntityState SceneData::getCurrent()
{
	std::lock_guard<std::mutex> g(m);
	return simulationData.front();
}

void SceneData::addNew(EntityState state)
{
	std::lock_guard<std::mutex> g(m);
	simulationData.push(state);
}

void SceneData::updateCurrent()
{
	std::lock_guard<std::mutex> g(m);
	if (simulationData.size() >= 2)
		simulationData.pop();

	//std::cout << simulationData.size() << std::endl;
}

SceneData::~SceneData()
{
}
