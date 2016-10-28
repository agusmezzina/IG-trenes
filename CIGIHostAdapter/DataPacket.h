#pragma once
class DataPacket
{
public:
	DataPacket(int id, float x, float y, float z, float vx, float vy, float vz, float ax, float ay, float az, float alpha, float alphaV, float t);
	int getID() const;
	float getX() const;
	float getY() const;
	float getZ() const;
	float getVx() const;
	float getVy() const;
	float getVz() const;
	float getAx() const;
	float getAy() const;
	float getAz() const;
	float getAlpha() const;
	float getAlphaV() const;
	float getTime() const;
	~DataPacket();
private:
	int id;
	float x, y, z, vx, vy, vz, ax, ay, az, alpha, alphaV, t;
};

