#pragma once
#include <string>
#include <vector>
#include "DataPacket.h"

class NetworkManager
{
public:
	NetworkManager();
	std::vector<DataPacket> readSimulationData(std::string ip, std::string port, int sampleSize);
	~NetworkManager();
};

