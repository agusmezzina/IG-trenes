#include "EntityState.h"


EntityState::EntityState()
{
}

EntityState::EntityState(double x, double y, double z) : x(x), y(y), z(z)
{
}

void EntityState::getState(double& x, double& y, double& z)
{
	x = this->x;
	y = this->y;
	z = this->z;
}

void EntityState::setState(double x, double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

EntityState::~EntityState()
{
}
