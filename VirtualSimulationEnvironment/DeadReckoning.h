#pragma once
#include "World.h"

class DeadReckoning
{
public:
	void extrapolate();
	DeadReckoning();
	virtual ~DeadReckoning();
private:
	double rThreshold;
	World* ghost;
};

