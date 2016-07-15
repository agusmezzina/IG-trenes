#include "stdafx.h"
#include "DeadReckoning.h"

void DeadReckoning::updateGhost(double deltaT)
{
	ghost->firstOrderPredictUpdate([deltaT](double p, double v) {return p + v * deltaT; });
}


DeadReckoning::DeadReckoning()
{

}


DeadReckoning::~DeadReckoning()
{
}
