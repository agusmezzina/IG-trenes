#include "ModelUpdater.h"
#include "EntityState.h"
#include <boost\array.hpp>
#include <boost\asio.hpp>
#include <boost\algorithm\string.hpp>
#include <iostream>

using boost::asio::ip::udp;

ModelUpdater::ModelUpdater(SceneData* data)
{
	this->data = data;
}

void ModelUpdater::run()
{
	boost::asio::io_service io_service;
	udp::socket socket(io_service, udp::endpoint(udp::v4(), 8887));

	while (true)
	{
		boost::array<unsigned char, 32768> recv_buf;
		udp::endpoint remote_endpoint;
		boost::system::error_code error;

		std::size_t len = socket.receive_from(boost::asio::buffer(recv_buf),
			remote_endpoint,
			0,
			error);
		if (error && error != boost::asio::error::message_size)
			throw boost::system::system_error(error);

		double x, y, z = 0;
		std::string msg(reinterpret_cast<char*>(recv_buf.c_array()), len);
		if (msg.back() == '\f'){
			msg.pop_back();
			std::vector<std::string> fields;
			boost::split(fields, msg, boost::is_any_of(";"));
			if (fields.size() == 4)
			{
				x = std::stod(fields[0]);
				y = std::stod(fields[1]);
				z = std::stod(fields[2]);
			}
			EntityState state(x, y, z);
			data->addNew(state);
		}
		//std::cout << x << std::endl;
		//std::cout << y << std::endl;
		//std::cout << z << std::endl;
	}
	socket.close();
}

ModelUpdater::~ModelUpdater()
{
}
