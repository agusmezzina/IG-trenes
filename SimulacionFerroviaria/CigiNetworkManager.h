#pragma once
#define PORT 8888
#define RECV_BUFFER_SIZE 32768
#define SEND_BUFFER_SIZE 32768
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <CigiIGSession.h>
#include <CigiSOFV3_2.h>
#include <memory>
#include "DataEventProcessor.h"
#include "ControlEventProcessor.h"

using boost::asio::ip::udp;

class CigiNetworkManager
{
public:
	CigiNetworkManager();
	void run();
	virtual ~CigiNetworkManager();
private:
	int inBufferSize;
	int outBufferSize;
	boost::array<unsigned char, RECV_BUFFER_SIZE> inBuffer;
	//unsigned char inBuffer[RECV_BUFFER_SIZE];
	unsigned char* outBuffer;
	boost::asio::io_service io_service;
	udp::endpoint remote_endpoint;
	std::unique_ptr<udp::socket> socket;
	std::unique_ptr<CigiIGSession> cigiSession;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	std::unique_ptr<DataEventProcessor> dataProcessor;
	std::unique_ptr<ControlEventProcessor> controlProcessor;
	std::unique_ptr<CigiSOFV3_2> startOfFrame;
};

