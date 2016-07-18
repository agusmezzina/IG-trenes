#pragma once
class DataPacket
{
public:
	DataPacket(int id, double x, double y, double z, double t);
	int getID() const;
	double getX() const;
	double getY() const;
	double getZ() const;
	double getTime() const;
	~DataPacket();
private:
	int id;
	double x, y, z, t;
};

