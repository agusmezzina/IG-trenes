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
	void setTimestamp(long t);
	long getTimestamp() const;
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	long timestamp;
	mutable std::mutex m;
};

