#include "DataPacket.h"


DataPacket::DataPacket(int id, double x, double y, double z, double t)
{
	this->id = id;
	this->x = x;
	this->y = y;
	this->z = z;
	this->t = t;
}

int DataPacket::getID() const
{
	return this->id;
}

double DataPacket::getX() const
{
	return this->x;
}

double DataPacket::getY() const
{
	return this->y;
}

double DataPacket::getZ() const
{
	return this->z;
}

double DataPacket::getTime() const
{
	return this->t;
}

DataPacket::~DataPacket()
{
}
