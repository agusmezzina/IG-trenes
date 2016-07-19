#pragma once
#include "World.h"

class DeadReckoning
{
public:
	void updateGhost(double deltaT);
	void correctGhost();
	bool isThresholdViolated(int entityID);
	DeadReckoning(World* model, World* ghost);
	virtual ~DeadReckoning();
private:
	double rThreshold;
	World* model;
	World* ghost;
};

