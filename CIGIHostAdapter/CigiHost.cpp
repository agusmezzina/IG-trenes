#include "CigiHost.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <iostream>
#define BUFFER_SIZE 32768

using boost::asio::ip::udp;

CigiHost::CigiHost(World* data)
{
	this->data = data;
	cigiSession = std::make_unique<CigiHostSession>(1, BUFFER_SIZE, 2, BUFFER_SIZE);
	ctrlProcessor = std::make_unique<IGControlProcessor>();

	CigiOutgoingMsg &Omsg = cigiSession->GetOutgoingMsgMgr();
	CigiIncomingMsg &Imsg = cigiSession->GetIncomingMsgMgr();
	outMsg = &Omsg;
	inMsg = &Imsg;

	cigiSession->SetCigiVersion(3, 3);
	cigiSession->SetSynchronous(true);

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
}

void CigiHost::run()
{
	try
	{
		unsigned char* outBuffer;
		int outBufferSize = 0;

		boost::asio::io_service io_service;
		udp::socket socket(io_service, udp::endpoint(udp::v4(), 8001));

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", "8888");
		udp::endpoint receiver_endpoint = *resolver.resolve(query);

		outMsg->BeginMsg();
		for (;;)
		{
			boost::array<unsigned char, BUFFER_SIZE> recv_buf;
			udp::endpoint remote_endpoint;
			boost::system::error_code error;
			boost::system::error_code ignored_error;

			std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf),
				remote_endpoint,
				0,
				error);
			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			inMsg->ProcessIncomingMsg(recv_buf.c_array(), len);

			// load the IG Control
			*outMsg << igControl;
			//Update position
			double x{ 0 }, y{ 0 }, z{ 0 }, vx{ 0 }, vy{ 0 }, vz{ 0 };
			//data->getCurrent().getState(x, y, z);
			auto entity = data->getEntity(1);
			entity.getState(x, y, z, vx, vy, vz);
			std::cout << y << std::endl;
			ownship.SetLon(y);
			*outMsg << ownship;
			outMsg->PackageMsg(&outBuffer, outBufferSize);

			outMsg->UpdateIGCtrl(outBuffer, recv_buf.c_array());

			socket.send_to(boost::asio::buffer(outBuffer, outBufferSize),
				receiver_endpoint,
				0,
				ignored_error);

			outMsg->FreeMsg();
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
