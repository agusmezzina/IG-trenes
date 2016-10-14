#pragma once
#include <memory>
#include <queue>
#include <fstream>
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include "DataPacket.h"
#include "SceneData.h"
#include "World.h"
#include "DeadReckoning.h"
#include "CigiPacker.h"
#include "Semaphore.h"
#include "RealTimeClock.h"

using boost::asio::ip::udp;

class CigiHost
{
public:
	CigiHost(World* data, World* ghost, Semaphore* sem, std::queue<DataPacket>* rawData);
	void run();
	virtual ~CigiHost();
private:
	void setupNetwork(const std::string& ip, const std::string& port);
	void syncWithRealTime();
	void setupCigi();
	//void initializeTimer();
	void updateModelFromNetwork();
	void sendCigiPacket();

	boost::asio::io_service io_service;
	udp::socket socket;
	udp::endpoint receiver_endpoint;

	std::unique_ptr<CigiPacker> cigi;
	std::unique_ptr<DeadReckoning> dr;
	std::unique_ptr<RealTimeClock> clock;
	World* data;
	World* ghost;
	std::queue<DataPacket>* rawData;
	Semaphore* s;
	float prevSimulationTime;
	float simulationTime;

	std::ofstream log;
};

