#include "stdafx.h"
#include "CigiNetworkManager.h"
#include <iostream>

CigiNetworkManager::CigiNetworkManager(SceneData* data) : data(data), inBufferSize(0), outBufferSize(0)
{
	socket = std::make_unique<udp::socket>(io_service, udp::endpoint(udp::v4(), PORT));
	cigiSession = std::make_unique<CigiIGSession>(1, RECV_BUFFER_SIZE, 2, SEND_BUFFER_SIZE);
	dataProcessor = std::make_unique<DataEventProcessor>(data);
	controlProcessor = std::make_unique<ControlEventProcessor>();
	startOfFrame = std::make_unique<CigiSOFV3_2>();
	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;

	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(true);

	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);

	//Agregar handlers
	inMsg->RegisterEventProcessor(CIGI_IG_CTRL_PACKET_ID_V3_2, controlProcessor.get());
	inMsg->RegisterEventProcessor(CIGI_ENTITY_CTRL_PACKET_ID_V3, dataProcessor.get());

	startOfFrame->SetDatabaseID(1);
	startOfFrame->SetIGStatus(0);
	startOfFrame->SetIGMode(CigiBaseSOF::Operate);
	startOfFrame->SetTimeStampValid(false);
	startOfFrame->SetEarthRefModel(CigiBaseSOF::WGS84);
	startOfFrame->SetTimeStamp(0);
	startOfFrame->SetFrameCntr(0);
}

int CigiNetworkManager::cancel(){
	done = true;
	while (isRunning()) YieldCurrentThread();
	return 0;
}

void CigiNetworkManager::run(){
	done = false;
	do
	{
		try{
			outMsg->BeginMsg();

			//--------------------------------------IN------------------------------------------------------
			boost::system::error_code error;
			std::cout << "Esperando..." << std::endl;
			inBufferSize = socket->receive_from(boost::asio::buffer(inBuffer), remote_endpoint, 0, error);

			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			if (inBufferSize > 0)
			{
				inMsg->ProcessIncomingMsg(inBuffer.c_array(), inBufferSize);
			}

			//--------------------------------------OUT-----------------------------------------------------
			boost::system::error_code ignored_error;
			*outMsg << *startOfFrame;
			outMsg->PackageMsg(&outBuffer, outBufferSize);
			outMsg->UpdateSOF(outBuffer);
			int sent = socket->send_to(boost::asio::buffer(outBuffer, outBufferSize), remote_endpoint, 0, ignored_error);
			//int sentBytes = network.send(pCigiOutBuf, CigiOutSz);
			outMsg->FreeMsg();
		}
		catch (std::exception& e){
			std::cerr << e.what() << std::endl;
		}
	} while (!done);
}

CigiNetworkManager::~CigiNetworkManager()
{
}
