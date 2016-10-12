#pragma once
#define BUFFER_SIZE 32768
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <OpenThreads\Thread>
#include <memory>
#include "World.h"
#include "CigiSessionHandler.h"

using boost::asio::ip::udp;

namespace osgCigi
{
	class CigiIG : public OpenThreads::Thread
	{
	public:
		CigiIG(World* data);
		virtual int cancel();
		virtual void run();
		void recvPacket();
		virtual ~CigiIG();
	private:
		void handle_receive(const boost::system::error_code& error, std::size_t size);

		boost::array<unsigned char, BUFFER_SIZE> inBuffer;
		boost::asio::io_service io_service;
		udp::endpoint remote_endpoint;
		std::unique_ptr<udp::socket> socket;

		std::unique_ptr<CigiSessionHandler> cigi;
		bool done = false;
		bool send = true;
	};
}

