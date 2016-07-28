#include "CigiHost.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <iostream>
#include <future>
#include <chrono>
#define BUFFER_SIZE 32768

using boost::asio::ip::udp;

CigiHost::CigiHost(World* data, World* ghost, std::queue<DataPacket>* rawData)
{
	this->data = data;
	this->ghost = ghost;
	this->rawData = rawData;
	cigiSession = std::make_unique<CigiHostSession>(1, BUFFER_SIZE, 2, BUFFER_SIZE);
	ctrlProcessor = std::make_unique<IGControlProcessor>();
	dr = std::make_unique<DeadReckoning>(data, ghost);

	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;

	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(false);

	inMsg->SetReaderCigiVersion(3, 3);
	inMsg->UsingIteration(false);

	// register SOF
	Imsg.RegisterEventProcessor(CIGI_SOF_PACKET_ID_V3_2, ctrlProcessor.get());

	// initialize the IG Control
	igControl.SetIGMode(CigiBaseIGCtrl::Operate);

	// initialize the Ownship
	//  the other parameters are set by CigiEntityCtrlV3_3
	ownship.SetEntityID(1);
	ownship.SetEntityType(0);
	ownship.SetEntityState(CigiBaseEntityCtrl::Active);

	rateData.SetEntityID(1);
}

void CigiHost::run()
{
	bool usingDR = false;
	bool started = false;
	try
	{
		unsigned char* outBuffer;
		int outBufferSize = 0;

		boost::asio::io_service io_service;
		udp::socket socket(io_service, udp::endpoint(udp::v4(), 8001));

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", "8888");
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		boost::system::error_code ignored_error;

		outMsg->BeginMsg();
		auto prevTime = std::chrono::high_resolution_clock::now();

		for (;;)
		{
			std::this_thread::sleep_for(std::chrono::milliseconds(10));
			auto actualTime = std::chrono::high_resolution_clock::now();
			std::chrono::duration<float> deltaT = actualTime - prevTime;
			prevTime = actualTime;
			if (!rawData->empty())
			{
				started = true;
				auto lastData = rawData->front();
				data->updateEntityPosition(lastData.getID(), osg::Vec3f(lastData.getX(), lastData.getY(), lastData.getZ()));
				data->updateEntityVelocity(lastData.getID(), osg::Vec3f(lastData.getVx(), lastData.getVy(), lastData.getVz()));
				rawData->pop();
			}

			dr->updateGhost(1, deltaT.count());
			
			if ((!usingDR && started) || ((((dr->isThresholdViolated(1)) || (rawData->empty())) && started)) && (usingDR) )
			{
				dr->correctGhost(1);
				auto entity = data->getEntity(1);
				auto p = entity.getPosition();
				auto v = entity.getVelocity();

				if (rawData->empty())
					std::cout << v.y() << std::endl;

				// load the IG Control
				*outMsg << igControl;
				//Update position
				
				ownship.SetLon(p.y());
				rateData.SetYRate(v.y());
				*outMsg << ownship;
				*outMsg << rateData;
				outMsg->PackageMsg(&outBuffer, outBufferSize);

				//outMsg->UpdateIGCtrl(outBuffer, recv_buf.c_array());

				socket.send_to(boost::asio::buffer(outBuffer, outBufferSize),
					receiver_endpoint,
					0,
					ignored_error);

				outMsg->FreeMsg();
			}
		}
		socket.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

CigiHost::~CigiHost()
{
}
