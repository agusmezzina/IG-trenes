#include "CigiHost.h"
#include <iostream>
#include <fstream>
#include <future>
#include <chrono>
#define BUFFER_SIZE 32768

CigiHost::CigiHost(World* data, World* ghost, Semaphore* sem, std::queue<DataPacket>* rawData) : io_service(), socket(io_service)
{
	this->data = data;
	this->ghost = ghost;
	this->s = sem;
	this->rawData = rawData;
	cigi = std::make_unique<CigiManager>();
	dr = std::make_unique<DeadReckoning>(data, ghost);
}

void CigiHost::setupNetwork(const std::string& ip, const std::string& port)
{
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), ip, port);
	receiver_endpoint = *resolver.resolve(query);
	socket.open(udp::v4());
}

float CigiHost::waitForRealTime()
{
	auto deltaSimTime = simulationTime - prevSimulationTime;
	auto realTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaRealTime = realTime - prevRealTime;
	std::chrono::duration<float> elapsedTime = realTime - initial;
	//log << "Sim: " << simulationTime << "; Real: " << elapsedTime.count() << "; DeltaSim = " << deltaSimTime << "; DeltaReal =" << deltaRealTime.count() << "; ";
	int waitFor = (deltaSimTime - deltaRealTime.count()) * 1000;
	std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
	prevSimulationTime = simulationTime;
	prevRealTime = std::chrono::high_resolution_clock::now();
	//log << waitFor << std::endl;
	return deltaSimTime;
}

void CigiHost::setupCigi()
{

}

void CigiHost::initializeTimer()
{
	prevSimulationTime = 0;
	prevRealTime = std::chrono::high_resolution_clock::now();
	initial = prevRealTime;
}

void CigiHost::updateModelFromNetwork()
{
	s->wait();
	auto lastData = rawData->front();
	data->updateEntityPosition(lastData.getID(), osg::Vec3f(lastData.getX(), lastData.getY(), lastData.getZ()));
	data->updateEntityVelocity(lastData.getID(), osg::Vec3f(lastData.getVx(), lastData.getVy(), lastData.getVz()));
	data->updateEntityAcceleration(lastData.getID(), osg::Vec3f(lastData.getAx(), lastData.getAy(), lastData.getAz()));
	simulationTime = lastData.getTime();
	rawData->pop();
}

void CigiHost::sendCigiPacket()
{

}

void CigiHost::run()
{
	bool usingDR = true;
	bool started = false;
	bool quadratic = true;
	
	bool first = true;
	try
	{
		std::ofstream log;
		log.open("logHost.txt");

		unsigned char* outBuffer;
		int outBufferSize = 0;
		setupNetwork("127.0.0.1", "8888");
		setupCigi();

		for (;;)
		{
			updateModelFromNetwork();
			if (first)
			{
				initializeTimer();
			}
			
			auto deltaSimTime = waitForRealTime();

			if (quadratic)
				dr->secondOrderUpdateGhost(1, deltaSimTime);
			else
				dr->firstOrderUpdateGhost(1, deltaSimTime);

			bool sendUpdate = true;
			if (usingDR)
				sendUpdate = dr->isThresholdViolated(1) || first;

			if (first)
				first = false;

			if (sendUpdate){
				auto tick = std::chrono::high_resolution_clock::now();
				std::chrono::duration<float> elapsed = tick - initial;

				auto entity = data->getEntity(1);
				auto p = entity.getPosition();
				auto v = entity.getVelocity();
				auto a = entity.getAcceleration();
				auto pg = ghost->getEntity(1).getPosition();
				log << "Correcting Time = " << simulationTime << "; ghost = " << pg.x() << "; model = " << p.x() << "; " << v.x() << ";" << a.x() << "; " << elapsed.count() << std::endl;
				dr->correctGhost(1);
				cigi->packData(entity, &outBuffer, outBufferSize);

				boost::system::error_code ignored_error;
				socket.send_to(boost::asio::buffer(outBuffer, outBufferSize),
					receiver_endpoint,
					0,
					ignored_error);

				cigi->freeMessage();
			}
		}
		socket.close();
		log.close();
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
}

CigiHost::~CigiHost()
{
}
