#include "UDPServer.h"
#include <iostream>
#include <boost\algorithm\string.hpp>
#define PORT_MODEL 8887
#define PORT_IG "8888"
#define BUFFER_SIZE 32768

UDPServer::UDPServer()
{
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

void UDPServer::run()
{
	try
	{
		unsigned char* outBuffer;
		int outBufferSize = 0;
		boost::asio::io_service io_service;
		udp::socket socketModel(io_service, udp::endpoint(udp::v4(), PORT_MODEL));

		udp::resolver resolver(io_service);
		udp::resolver::query query(udp::v4(), "127.0.0.1", PORT_IG);
		udp::endpoint ig_endpoint = *resolver.resolve(query);
		udp::socket socketIG(io_service);
		socketIG.open(udp::v4());

		outMsg->BeginMsg();
		for (;;)
		{
			boost::array<unsigned char, BUFFER_SIZE> recv_buf;
			udp::endpoint remote_endpoint;
			boost::system::error_code error;

			std::cout << "Esperando datos." << std::endl;

			std::size_t len = socketModel.receive_from(boost::asio::buffer(recv_buf),
				remote_endpoint,
				0,
				error);
			if (error && error != boost::asio::error::message_size)
				throw boost::system::system_error(error);

			/*std::string msg(recv_buf.c_array(), len);
			std::cout << msg << std::endl;*/
			boost::system::error_code ignored_error;
			
			inMsg->ProcessIncomingMsg(recv_buf.c_array(), len);
			// load the IG Control
			*outMsg << igControl;
			//Update position
			std::string msg(reinterpret_cast<char*>(recv_buf.c_array()), len);
			if (msg.back() == '\f'){
				msg.pop_back();
				std::vector<std::string> fields;
				boost::split(fields, msg, boost::is_any_of(";"));
				if (fields.size() == 4)
					ownship.SetLon(std::stod(fields[1]));
			}

			*outMsg << ownship;
			outMsg->PackageMsg(&outBuffer, outBufferSize);

			outMsg->UpdateIGCtrl(outBuffer, recv_buf.c_array());

			socketIG.send_to(boost::asio::buffer(outBuffer, outBufferSize),
				ig_endpoint,
				0,
				ignored_error);

			outMsg->FreeMsg();
			/*std::string message = "2.5";
			boost::system::error_code ignored_error;
			socketModel.send_to(boost::asio::buffer(message),
				remote_endpoint,
				0,
				ignored_error);*/
		}
		socketIG.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}


UDPServer::~UDPServer()
{
}
