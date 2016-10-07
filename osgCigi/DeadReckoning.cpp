#include "DeadReckoning.h"
#include <cmath>

using namespace osgCigi;

void DeadReckoning::firstOrderUpdateGhost(int entityID, float deltaT)
{
	//ghost->firstOrderPredictUpdate([deltaT](float p, float v) {return p + v * deltaT; });
	auto ghostEntity = ghost->getEntity(entityID);
	auto p = ghostEntity.getPosition();
	auto v = ghostEntity.getVelocity();
	ghost->updateEntityPosition(entityID, p + v * deltaT);
}

void DeadReckoning::secondOrderUpdateGhost(int entityID, float deltaT)
{
	//ghost->firstOrderPredictUpdate([deltaT](float p, float v) {return p + v * deltaT; });
	auto ghostEntity = ghost->getEntity(entityID);
	auto p = ghostEntity.getPosition();
	auto v = ghostEntity.getVelocity();
	auto a = ghostEntity.getAcceleration();
	ghost->updateEntityPosition(entityID, p + v * deltaT + a * 0.5f * pow(deltaT, 2));
	ghost->updateEntityVelocity(entityID, v + a * deltaT);
}

void DeadReckoning::setConvergencePoint(int entityID, float deltaT)
{
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();
	convergencePoint = p + v * deltaT + a * 0.5f * pow(deltaT, 2);
}

void DeadReckoning::correctGhost(int entityID, int step)
{
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();
	auto pg = ghost->getEntity(entityID).getPosition();

	/*if (step == 1)
	{
		auto convergenceTime = smoothness * deltaT;
		convergencePoint = p + v * convergenceTime + a * 0.5f * pow(convergenceTime, 2);
	}*/

	ghost->updateEntityPosition(entityID, pg + (convergencePoint - pg) * step / smoothness);
	ghost->updateEntityVelocity(entityID, v);
	ghost->updateEntityAcceleration(entityID, a);
}

void DeadReckoning::correctGhost(int entityID)
{
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();
	ghost->updateEntityPosition(entityID, p);
	ghost->updateEntityVelocity(entityID, v);
	ghost->updateEntityAcceleration(entityID, a);
}

bool DeadReckoning::isThresholdViolated(int entityID)
{
	auto entity = model->getEntity(entityID);
	auto ghostEntity = ghost->getEntity(entityID);
	auto p0 = entity.getPosition();
	auto p1 = ghostEntity.getPosition();
	
	if (sqrt(pow(p0.x() - p1.x(), 2) + pow(p0.y() - p1.y(), 2) + pow(p0.z() - p1.z(), 2)) > rThreshold)
	{
		return true;
	}
	return false;
}

int DeadReckoning::getSmoothness() const
{
	return this->smoothness;
}

DeadReckoning::DeadReckoning(World* model, World* ghost) : model(model), ghost(ghost)
{
	rThreshold = 0.1f;
	smoothness = 10;
}


DeadReckoning::~DeadReckoning()
{
}
