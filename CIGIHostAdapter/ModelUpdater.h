#pragma once
#include <queue>
#include <atomic>
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include "SceneData.h"
#include "DataPacket.h"
#include "Semaphore.h"

using boost::asio::ip::udp;

class ModelUpdater
{
public:
	ModelUpdater(Semaphore* sem, std::queue<DataPacket>* data);
	void run(std::atomic_bool& quit);
	void stop();
	virtual ~ModelUpdater();
private:
	void handle_receive(const boost::system::error_code& error, std::size_t size);
	void recv_packet();
	DataPacket readData(std::string message);
	void enqueueData(DataPacket p);
	std::queue<DataPacket>* data;
	Semaphore* s;
	boost::asio::io_service io_service;
	udp::endpoint remote_endpoint;
	std::unique_ptr<udp::socket> socket;
	boost::array<unsigned char, 32768> recv_buf;
};

