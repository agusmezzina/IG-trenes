#include "CigiHost.h"
#include <iostream>
#define BUFFER_SIZE 32768

using boost::asio::ip::udp;

CigiHost::CigiHost(SimulationState* ss)
{
	this->state = ss;
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
	ownship.SetEntityID(0);
	ownship.SetEntityType(0);
	ownship.SetEntityState(CigiBaseEntityCtrl::Active);
}

void CigiHost::runCigi()
{
	try
	{
		unsigned char* outBuffer;
		int outBufferSize = 0;
		boost::asio::io_service io_service;

		/*udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", "8888");
		udp::endpoint ig_endpoint = *resolver.resolve(query);
		udp::socket socketIG(io_service);
		socketIG.open(udp::v4());*/
		udp::socket socketIG(io_service, udp::endpoint(udp::v4(), 8001));
		udp::endpoint remote_endpoint;

		outMsg->BeginMsg();
		for (;;)
		{
			boost::array<unsigned char, BUFFER_SIZE> recv_buf;
			boost::system::error_code error;
			boost::system::error_code ignored_error;

			std::size_t len = socketIG.receive_from(boost::asio::buffer(recv_buf),
				remote_endpoint,
				0,
				error);
			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			inMsg->ProcessIncomingMsg(recv_buf.c_array(), len);

			double x, y, z = 0;
			state->getData(x, y, z);
			ownship.SetLon(y);

			// load the IG Control
			*outMsg << igControl;
			*outMsg << ownship;
			outMsg->PackageMsg(&outBuffer, outBufferSize);

			outMsg->UpdateIGCtrl(outBuffer, recv_buf.c_array());

			socketIG.send_to(boost::asio::buffer(outBuffer, outBufferSize),
				remote_endpoint,
				0,
				ignored_error);

			outMsg->FreeMsg();
		}
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

CigiHost::~CigiHost()
{
}
