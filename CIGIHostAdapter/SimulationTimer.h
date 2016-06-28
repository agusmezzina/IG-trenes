#pragma once
#include "SceneData.h"
#include <queue>

class SimulationTimer
{
public:
	SimulationTimer(SceneData* data);
	void run();
	virtual ~SimulationTimer();
private:
	SceneData* _data;
};

