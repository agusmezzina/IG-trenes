#pragma once
#include <list>
#include <mutex>
#include <functional>
#include <osg\Vec3f>
#include "Entity.h"

class World
{
public:
	void addEntity(Entity e);
	const Entity getEntity(int id) const;
	void updateEntityPosition(int id, osg::Vec3f position);
	void updateEntityVelocity(int id, osg::Vec3f velocity);
	void updateEntityAcceleration(int id, osg::Vec3f acceleration);
	void setTimestamp(float t);
	float getTimestamp() const;
	void firstOrderPredictUpdate(
		const std::function<float (float p, float v)>& predictionFunction);
	//void instantCorrect(World* reference, double threshold);
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	float timestamp;
	mutable std::mutex m;
};

