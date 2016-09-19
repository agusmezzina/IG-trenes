#pragma once
#include <memory>
#include <queue>
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include "DataPacket.h"
#include "SceneData.h"
#include "World.h"
#include "DeadReckoning.h"
#include "CigiManager.h"
#include "Semaphore.h"

using boost::asio::ip::udp;

class CigiHost
{
public:
	CigiHost(World* data, World* ghost, Semaphore* sem, std::queue<DataPacket>* rawData);
	void run();
	virtual ~CigiHost();
private:
	void setupNetwork(const std::string& ip, const std::string& port);
	float waitForRealTime();
	void setupCigi();
	void initializeTimer();
	void updateModelFromNetwork();
	void sendCigiPacket();

	boost::asio::io_service io_service;
	udp::socket socket;
	udp::endpoint receiver_endpoint;

	std::unique_ptr<CigiManager> cigi;
	std::unique_ptr<DeadReckoning> dr;
	World* data;
	World* ghost;
	std::queue<DataPacket>* rawData;
	Semaphore* s;
	float prevSimulationTime;
	std::chrono::high_resolution_clock::time_point prevRealTime;
	float simulationTime;
	std::chrono::high_resolution_clock::time_point realTime;
	std::chrono::high_resolution_clock::time_point initial;
};

