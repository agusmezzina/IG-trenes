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
	auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
	long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	auto t0 = model->getTimestamp();
	float compTime = ((float)(t1 - t0)) / 1000;

	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();
	auto ghostEntity = ghost->getEntity(entityID);
	auto pg = ghostEntity.getPosition();

	auto compP = p + v * compTime + a * 0.5f * pow(compTime, 2);
	auto compV = v + a * compTime;

	startPoint = pg;
	convergencePoint = compP + v * deltaT + a * 0.5f * pow(deltaT, 2);
	convergenceVelocity = compV + a * deltaT;
}

osg::Vec3f DeadReckoning::getConvergencePoint()
{
	return convergencePoint;
}

void DeadReckoning::correctGhost(int entityID, int step)
{
	/*if (step == 1)
		this->compensateAndCorrectGhost(entityID);*/

	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();

	auto ghostEntity = ghost->getEntity(entityID);
	auto pg = ghostEntity.getPosition();
	auto vg = ghostEntity.getVelocity();
	auto ag = ghostEntity.getAcceleration();

	ghost->updateEntityPosition(entityID, startPoint + (convergencePoint - startPoint) * step / smoothness);
	ghost->updateEntityVelocity(entityID, convergenceVelocity);
	ghost->updateEntityAcceleration(entityID, a);

	//ghost->updateEntityPosition(entityID, pg + (convergencePoint - pg) * step / smoothness);
	//ghost->updateEntityVelocity(entityID, v);
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


void DeadReckoning::compensateAndCorrectGhost(int entityID)
{
	auto t = std::chrono::high_resolution_clock::now().time_since_epoch();
	long t1 = std::chrono::duration_cast<std::chrono::milliseconds>(t).count();
	auto t0 = model->getTimestamp();
	float deltaT = ((float)(t1 - t0)) / 1000;
	auto entity = model->getEntity(entityID);
	auto p = entity.getPosition();
	auto v = entity.getVelocity();
	auto a = entity.getAcceleration();
	ghost->updateEntityPosition(entityID, p + v * deltaT + a * 0.5f * pow(deltaT, 2));
	ghost->updateEntityVelocity(entityID, v + a * deltaT);
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

void DeadReckoning::setSmoothness(int smoothness)
{
	this->smoothness = smoothness;
}

float DeadReckoning::getThreshold() const
{
	return this->rThreshold;
}

void DeadReckoning::setThreshold(float thresh)
{
	this->rThreshold = thresh;
}

DeadReckoning::DeadReckoning(World* model, World* ghost) : model(model), ghost(ghost)
{
	rThreshold = 0.5f;
	smoothness = 10;
}


DeadReckoning::~DeadReckoning()
{
}
