#include "stdafx.h"
#include "Entity.h"

void Entity::getPositionState(double& x, double& y, double& z) const
{
	x = this->x;
	y = this->y;
	z = this->z;
}

void Entity::setPositionState(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

int Entity::getID() const
{
	return this->id;
}

Entity::Entity(int id)
{
	this->id = id;
	this->x = 0;
	this->y = 0;
	this->z = 0;
	this->yaw = 0;
	this->pitch = 0;
	this->roll = 0;
}

Entity::~Entity()
{
}
