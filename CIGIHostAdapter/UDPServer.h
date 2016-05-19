#pragma once
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <CigiHostSession.h>
#include <CigiIGCtrlV3_2.h>
#include <CigiEntityCtrlV3_3.h>
#include <memory>
#include "IGControlProcessor.h"

#define RECV_BUFFER_SIZE 32768

using boost::asio::ip::udp;

class UDPServer
{
public:
	UDPServer();
	virtual ~UDPServer();
	void run();
private:
	std::unique_ptr<CigiHostSession> cigiSession;
	std::unique_ptr<IGControlProcessor> ctrlProcessor;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	CigiIGCtrlV3_2 igControl;
	CigiEntityCtrlV3_3 ownship;
//	boost::asio::io_service io_service;
//	udp::socket socket;
};

