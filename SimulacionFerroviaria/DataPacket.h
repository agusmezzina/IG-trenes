#pragma once
class DataPacket
{
private:
	double _x;
	double _y;
	double _z;
	double _t;
public:
	DataPacket(double x, double y, double z, double t);
	double getX() const;
	double getY() const;
	double getZ() const;
	double getT() const;
	~DataPacket();
};

