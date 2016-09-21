//#include <windows.h>
#include <iostream>
#include <future>
#include <chrono>
#include "CigiHost.h"
#define BUFFER_SIZE 32768

CigiHost::CigiHost(World* data, World* ghost, Semaphore* sem, std::queue<DataPacket>* rawData) : io_service(), socket(io_service)
{
	this->data = data;
	this->ghost = ghost;
	this->s = sem;
	this->rawData = rawData;
	cigi = std::make_unique<CigiManager>();
	dr = std::make_unique<DeadReckoning>(data, ghost);
	log.open("logHost.txt");
	prevSimulationTime = 0;
	compensationTime = 0;
}

void CigiHost::setupNetwork(const std::string& ip, const std::string& port)
{
	udp::resolver resolver(io_service);
	udp::resolver::query query(udp::v4(), ip, port);
	receiver_endpoint = *resolver.resolve(query);
	socket.open(udp::v4());
}

void CigiHost::syncWithRealTime()
{
	auto deltaSimTime = simulationTime - prevSimulationTime;
	auto realTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> deltaRealTime = realTime - prevRealTime;
	int waitFor = (deltaSimTime - deltaRealTime.count() - compensationTime) * 1000;
	//prevSimulationTime = simulationTime;
	//prevRealTime = realTime;
	//log << elapsedTime.count() << std::endl;
	if (waitFor > 0)
		std::this_thread::sleep_for(std::chrono::milliseconds(waitFor));
	else
		waitFor = 0;

	prevRealTime = std::chrono::high_resolution_clock::now();
	std::chrono::duration<float> elapsedTime = prevRealTime - initial;
	compensationTime = elapsedTime.count() - simulationTime;
	log << "Sim: " << simulationTime << "; DeltaSim = " << deltaSimTime << "; DeltaReal =" << deltaRealTime.count() << "; ElapsedTime =" << elapsedTime.count() << "; Waited =" << waitFor << std::endl;
}

void CigiHost::setupCigi()
{

}

void CigiHost::initializeTimer()
{
	prevRealTime = std::chrono::high_resolution_clock::now();
	initial = prevRealTime;
}

void CigiHost::updateModelFromNetwork()
{
	s->wait();
	prevSimulationTime = simulationTime;
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
	//SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);

	bool usingDR = true;
	bool started = false;
	bool quadratic = true;
	
	bool first = true;
	try
	{

		unsigned char* outBuffer;
		int outBufferSize = 0;
		setupNetwork("127.0.0.1", "8888");
		setupCigi();

		for (;;)
		{
			updateModelFromNetwork();
			if (first)
				initializeTimer();			 

			auto deltaSimTime = simulationTime - prevSimulationTime;
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
				dr->correctGhost(1);
				cigi->packData(entity, &outBuffer, outBufferSize);

				auto p = entity.getPosition();
				auto v = entity.getVelocity();
				auto a = entity.getAcceleration();
				auto pg = ghost->getEntity(1).getPosition();
				log << "Correcting Time = " << simulationTime << "; ghost = " << pg.x() << "; model = " << p.x() << "; " << v.x() << ";" << a.x() << "; " << elapsed.count() << std::endl;
				syncWithRealTime();

				boost::system::error_code ignored_error;
				socket.send_to(boost::asio::buffer(outBuffer, outBufferSize),
					receiver_endpoint,
					0,
					ignored_error);

				cigi->freeMessage();
			}
			else
				syncWithRealTime();
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
