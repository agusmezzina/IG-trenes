#pragma once
#include "World.h"

class DeadReckoning
{
public:
	void updateGhost(int entityID, float deltaT);
	void correctGhost(int entityID, int step);
	void correctGhost(int entityID);
	bool isThresholdViolated(int entityID);
	DeadReckoning(World* model, World* ghost);
	virtual ~DeadReckoning();
private:
	float rThreshold;
	int smoothness;
	World* model;
	World* ghost;
};

