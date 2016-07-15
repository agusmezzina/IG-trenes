#pragma once
#include "World.h"

class DeadReckoning
{
public:
	void updateGhost(double deltaT);
	void correctGhost();
	DeadReckoning();
	virtual ~DeadReckoning();
private:
	double rThreshold;
	World* model;
	World* ghost;
};

