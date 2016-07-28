#pragma once
class DataPacket
{
public:
	DataPacket(int id, float x, float y, float z, float vx, float vy, float vz, float t);
	int getID() const;
	float getX() const;
	float getY() const;
	float getZ() const;
	float getVx() const;
	float getVy() const;
	float getVz() const;
	float getTime() const;
	~DataPacket();
private:
	int id;
	float x, y, z, vx, vy, vz, t;
};

