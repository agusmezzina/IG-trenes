//#include "stdafx.h"
#include "CigiNetworkManager.h"
#include <iostream>
#include <boost\bind.hpp>

CigiNetworkManager::CigiNetworkManager(World* data) : data(data), inBufferSize(0), outBufferSize(0)
{
	socket = std::make_unique<udp::socket>(io_service, udp::endpoint(udp::v4(), 8888));
	cigiSession = std::make_unique<CigiIGSession>(1, RECV_BUFFER_SIZE, 2, SEND_BUFFER_SIZE);
	dataProcessor = std::make_unique<DataEventProcessor>(data);
	controlProcessor = std::make_unique<ControlEventProcessor>();
	rateProcessor = std::make_unique<RateEventProcessor>(data);
	startOfFrame = std::make_unique<CigiSOFV3_2>();

	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;

	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(false);
	
	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);

	//Agregar handlers
	inMsg->RegisterEventProcessor(CIGI_IG_CTRL_PACKET_ID_V3_2, controlProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_ENTITY_CTRL_PACKET_ID_V3_3, dataProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_RATE_CTRL_PACKET_ID_V3_2, rateProcessor.get());

	startOfFrame->SetDatabaseID(1);
	startOfFrame->SetIGStatus(0);
	startOfFrame->SetIGMode(CigiBaseSOF::Operate);
	startOfFrame->SetTimeStampValid(false);
	startOfFrame->SetEarthRefModel(CigiBaseSOF::WGS84);
	startOfFrame->SetTimeStamp(0);
	startOfFrame->SetFrameCntr(0);

	recvPacket();
	//start_receive();
	/*udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), "127.0.0.1", "8001");
	receiver_endpoint = *resolver.resolve(query);*/
}

//void CigiNetworkManager::sendSOF()
//{
//	if (!send)
//		return;
//	outMsg->BeginMsg();
//	boost::system::error_code ignored_error;
//	*outMsg << *startOfFrame;
//	outMsg->PackageMsg(&outBuffer, outBufferSize);
//	outMsg->UpdateSOF(outBuffer);
//
//	//int sent = socket->send_to(boost::asio::buffer(outBuffer, outBufferSize), receiver_endpoint, 0, ignored_error);
//	//outMsg->FreeMsg();
//
//	socket->async_send_to(boost::asio::buffer(outBuffer, outBufferSize),
//		receiver_endpoint,
//		boost::bind(&CigiNetworkManager::handle_send,
//		this,
//		boost::asio::placeholders::error,
//		boost::asio::placeholders::bytes_transferred));
//
//	send = false;
//}

void CigiNetworkManager::recvPacket()
{
	socket->async_receive_from(boost::asio::buffer(inBuffer),
		remote_endpoint,
		boost::bind(&CigiNetworkManager::handle_receive,
		this,
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
}

void CigiNetworkManager::handle_receive(const boost::system::error_code& error, std::size_t size)
{
	if (error && error != boost::asio::error::message_size)
		throw boost::system::system_error(error);

	if (size > 0)
	{
		inMsg->ProcessIncomingMsg(inBuffer.c_array(), size);
	}

	//send = true;
	recvPacket();
}

//void CigiNetworkManager::handle_send(const boost::system::error_code& error, std::size_t size)
//{
//	outMsg->FreeMsg();
//	recvPacket();
//}

int CigiNetworkManager::cancel(){
	done = true;
	while (isRunning()) YieldCurrentThread();
	return 0;
}

void CigiNetworkManager::run(){
	done = false;
	while (!done)
	{
		io_service.run();
		io_service.reset();
	}
}

CigiNetworkManager::~CigiNetworkManager()
{
	socket->close();
}
