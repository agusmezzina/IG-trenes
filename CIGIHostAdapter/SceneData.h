#pragma once
#include <mutex>
#include <queue>
#include "EntityState.h"

class SceneData
{
public:
	SceneData();
	EntityState getCurrent();
	void addNew(EntityState state);
	void updateCurrent();
	~SceneData();
private:
	std::queue<EntityState> simulationData;
	mutable std::mutex m;
};

