#pragma once
#define RECV_BUFFER_SIZE 32768
#define SEND_BUFFER_SIZE 32768
#include <boost\asio.hpp>
#include <boost\array.hpp>
#include <CigiIGSession.h>
#include <CigiSOFV3_2.h>
#include <OpenThreads\Thread>
#include <memory>
#include "DataEventProcessor.h"
#include "ControlEventProcessor.h"
#include "SceneData.h"

using boost::asio::ip::udp;

class CigiNetworkManager : public OpenThreads::Thread
{
public:
	CigiNetworkManager(SceneData* data);
	virtual int cancel();
	virtual void run();
	void sendSOF();
	void recvPacket();
	virtual ~CigiNetworkManager();
private:
	void handle_receive(const boost::system::error_code& error, std::size_t size);
	void handle_send(const boost::system::error_code& error, std::size_t size);

	SceneData* data;

	bool done;

	int inBufferSize;
	int outBufferSize;
	boost::array<unsigned char, RECV_BUFFER_SIZE> inBuffer;
	unsigned char* outBuffer;

	boost::asio::io_service io_service;
	udp::endpoint receiver_endpoint;
	udp::endpoint remote_endpoint;
	std::unique_ptr<udp::socket> socket;

	std::unique_ptr<CigiIGSession> cigiSession;
	CigiOutgoingMsg* outMsg;
	CigiIncomingMsg* inMsg;
	std::unique_ptr<DataEventProcessor> dataProcessor;
	std::unique_ptr<ControlEventProcessor> controlProcessor;
	std::unique_ptr<CigiSOFV3_2> startOfFrame;

	bool send = true;
};

