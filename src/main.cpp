/////////////////////////////////////////////////////////////////
//															   //
//      Main Client UDP for receiving data from Leica TotalStation                         //
//															   //
//				Author: Manuel P. J. (aka. manuoso)			   //
//															   //
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include "LogManager.h"

struct data_m{
        float x;
        float y;
        float z;
	      uint64_t timestamp;
    };

    struct data_ma{
        float x;
        float y;
        float z;
	      uint64_t timestamp;
        float hz;
        float v;
	float slopeDist;
    };

using boost::asio::ip::udp;

int main(int argc, char* argv[]) {

	LogManager::init("ReceivedClientUDP_" + std::to_string(time(NULL)));

	try {
		boost::asio::io_service io_service;
	
		LogManager::get()->status("Enter IP of server to connect: ", true);
		std::string ipServerUDP;
		std::cin >> ipServerUDP;

		LogManager::get()->status("Enter PORT of server to connect: ", true);	
		int portServerUDP;
		std::cin >> portServerUDP;

		std::cout << "You want to receive measure only?" << std::endl;
		std::cout << "-1 measure only" << std::endl;
		std::cout << "-2 measure with angles" << std::endl;
		int measureMode;		
		std::cin >> measureMode; 

		boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string(ipServerUDP), portServerUDP);

		udp::socket socket(io_service);
		socket.open(udp::v4());

		boost::array<char, 1> send_buf = { { 0 } };
		socket.send_to(boost::asio::buffer(send_buf), endpoint);

		
		LogManager::get()->status("Started to receive data", true);

		if(measureMode == 1){
			while(true){
				boost::array<char, sizeof(data_m)> recv_buf;
				udp::endpoint sender_endpoint;
				size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

				data_m data_recv;
				memcpy(&data_recv, &recv_buf[0], sizeof(data_m));
		
				std::cout << "length received: " << len << std::endl;
				std::cout << "x: " << data_recv.x << ", y: " << data_recv.y << ", z: " << data_recv.z << ", timestamp: " << data_recv.timestamp << std::endl;

				LogManager::get()->message(std::to_string(len), "LENGTH", false);
				LogManager::get()->message(std::to_string(data_recv.x) + "|" + std::to_string(data_recv.y) + "|" + std::to_string(data_recv.z) + "|" + std::to_string(data_recv.timestamp), "RECEIVED", false);
			}
		}else if(measureMode == 2){
			while(true){
				boost::array<char, sizeof(data_ma)> recv_buf;
				udp::endpoint sender_endpoint;
				size_t len = socket.receive_from(boost::asio::buffer(recv_buf), sender_endpoint);

				data_ma data_recv;
				memcpy(&data_recv, &recv_buf[0], sizeof(data_ma));
		
				std::cout << "length received: " << len << std::endl;
				std::cout << "x: " << data_recv.x << ", y: " << data_recv.y << ", z: " << data_recv.z << ", timestamp: " << data_recv.timestamp << ", HZ: " << data_recv.hz << ", V: " << data_recv.v << ", SlopeDist: " << data_recv.slopeDist << std::endl;

				LogManager::get()->message(std::to_string(len), "LENGTH", false);
				LogManager::get()->message(std::to_string(data_recv.x) + "|" + std::to_string(data_recv.y) + "|" + std::to_string(data_recv.z) + "|" + std::to_string(data_recv.timestamp) + "|" + std::to_string(data_recv.hz) + "|" + std::to_string(data_recv.v) + "|" + std::to_string(data_recv.slopeDist), "RECEIVED", false);
			}
		}else{
			LogManager::get()->error("Unrecognized measure mode", true);
		}
			
	}
	catch (std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}

	LogManager::close();

	return 0;
}
