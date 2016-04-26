#include "DataPacket.h"


DataPacket::DataPacket(double x, double y, double z, double t) : _x(x), _y(y), _z(z), _t(t)
{
}

double DataPacket::getX() const{
	return _x;
}

double DataPacket::getY() const{
	return _y;
}

double DataPacket::getZ() const{
	return _z;
}

double DataPacket::getT() const{
	return _t;
}

DataPacket::~DataPacket()
{
}
