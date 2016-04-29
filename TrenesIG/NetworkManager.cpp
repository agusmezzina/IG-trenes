#include "stdafx.h"
#include "NetworkManager.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\algorithm\string.hpp>

using boost::asio::ip::udp;

NetworkManager::NetworkManager()
{
}

std::vector<DataPacket> NetworkManager::readSimulationData(std::string ip, std::string port, int sampleSize)
{
	std::vector<DataPacket> data;

	boost::asio::io_service io_service;
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), ip, port);
	udp::endpoint sender_endpoint = *resolver.resolve(query);
	udp::socket socket(io_service);
	socket.open(udp::v4());
	socket.bind(sender_endpoint);

	boost::array<char, 128> recv_buf;
	int i = 0;
	while (i < sampleSize){
		size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);
		std::string msg(recv_buf.c_array(), len);
		if (msg.back() == '\f'){
			msg.pop_back();
			std::vector<std::string> fields;
			boost::split(fields, msg, boost::is_any_of(";"));
			if (fields.size() == 4)
				data.push_back(
					DataPacket(
						std::stod(fields[0]),
						std::stod(fields[1]),
						std::stod(fields[2]),
						std::stod(fields[3])));
		}
		i++;
	}

	return data;
}

NetworkManager::~NetworkManager()
{
}
