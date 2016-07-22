#include "DataPacket.h"


DataPacket::DataPacket(int id, float x, float y, float z, float vx, float vy, float vz, float t)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;
	this->vx = vx;
	this->vy = vy;
	this->vz = vz;
	this->t = t;
}

int DataPacket::getID() const
{
	return this->id;
}

float DataPacket::getX() const
{
	return this->x;
}

float DataPacket::getY() const
{
	return this->y;
}

float DataPacket::getZ() const
{
	return this->z;
}

float DataPacket::getVx() const
{
	return this->vx;
}

float DataPacket::getVy() const
{
	return this->vy;
}

float DataPacket::getVz() const
{
	return this->vz;
}

float DataPacket::getTime() const
{
	return this->t;
}

DataPacket::~DataPacket()
{
}
