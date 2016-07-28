#pragma once
#include <list>
#include <mutex>
#include <functional>
#include "Entity.h"

class World
{
public:
	void addEntity(Entity e);
	const Entity getEntity(int id) const;
	void updateEntityPosition(int id, osg::Vec3f position);
	void updateEntityVelocity(int id, osg::Vec3f velocity);
	void firstOrderPredictUpdate(
		const std::function<float (float p, float v)>& predictionFunction);
	//void instantCorrect(World* reference, double threshold);
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	mutable std::mutex m;
};
