#pragma once
#include <list>
#include <mutex>
#include "Entity.h"

class World
{
public:
	void addEntity(Entity e);
	void updateEntityPosition(int id, double x, double y, double z);
	World();
	virtual ~World();
private:
	std::list<Entity> entities;
	std::mutex m;
};

