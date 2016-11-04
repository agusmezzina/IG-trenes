//#include "stdafx.h"
#include "CigiIG.h"
#include <iostream>
#include <boost\bind.hpp>
#include <thread>

using namespace osgCigi;

CigiIG::CigiIG(World* data)
{
	socket = std::make_unique<udp::socket>(io_service, udp::endpoint(udp::v4(), 8888));
	cigi = std::make_unique<CigiSessionHandler>(data, BUFFER_SIZE);
	recvPacket();
}

void CigiIG::recvPacket()
{
	socket->async_receive_from(boost::asio::buffer(inBuffer),
		remote_endpoint,
		boost::bind(&CigiIG::handle_receive,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void CigiIG::handle_receive(const boost::system::error_code& error, std::size_t size)
{
	recvPacket();
	std::this_thread::sleep_for(std::chrono::milliseconds(300)); //DELAY
	if (error && error != boost::asio::error::message_size)
		throw boost::system::system_error(error);

	if (size > 0)
	{
		cigi->processIncomingMessage(inBuffer.c_array(), size);
	}	
}

int CigiIG::cancel(){
	done = true;
	while (isRunning()) YieldCurrentThread();
	return 0;
}

void CigiIG::run(){
	done = false;
	io_service.run();
	/*while (!done)
	{
		io_service.run();
		io_service.reset();
	}*/
}

CigiIG::~CigiIG()
{
	socket->close();
}
