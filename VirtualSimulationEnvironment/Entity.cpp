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

void Entity::getVelocityState(double& vx, double& vy, double& vz) const
{
	vx = this->vx;
	vy = this->vy;
	vz = this->vz;
}

void Entity::setVelocityState(double x, double y, double z)
{
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
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
	this->vx = 0;
	this->vy = 0;
	this->vz = 0;
}

Entity::~Entity()
{
}
