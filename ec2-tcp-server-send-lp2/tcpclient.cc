//For sending a file to LP2.
//First, act as server, create socket, and accept connection from LP2. Then become client and send files.
//This circumvents LP2's ever-changing public IP.
#include <SFML/Network/Socket.hpp>
#include <SFML/Network/TcpSocket.hpp>
#include <SFML/Network/TcpListener.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

#include "network.h"

using namespace std;

int main(int argc, char *argv[])
{
    sf::TcpSocket socket;
    sf::Socket::Status status;
    sf::TcpListener listener;
    sf::IpAddress remoteAddr;
    unsigned short localPort = 0;
    unsigned short remotePort = 0;
    unsigned long count = 0;
    unsigned long bytes = 0;
    std::size_t received = 0;
    char buffer[PACKET_SIZE];
    std::streamsize bytesRead = 0;
    int offset = 0;
    int i = 0; //for loop clearing buffer
    char textFileName[] = "receivedData.txt"; //Assume already sent from LP1 to EC2
    int retVal = 0;

    streampos fileBeginning;
    streampos fileEnd;
    
    // parse command line arguments, if any
    if (argc < 2) 
      localPort = DATA_PORT; //Should be 5001
    else
    {
        char *s(argv[0]);
        char *tmp;
        try
        {
            localPort = static_cast<unsigned short>(strtol(s,&tmp,10));
        }   
        catch (const std::invalid_argument& e)
        {
            std::cout << "Unable to convert " << s << " to integer" << std::endl;
            localPort = DATA_PORT;
        }   
    }

    std::ifstream fileIn;
    fileIn.open(textFileName, std::ifstream::in);

    if(fileIn.is_open()){ //Verify file was opened successfully
      std::cout << "File successfully opened." << std::endl;
    }
    else{
      std::cout << "File failed to open." << std::endl;
    }
    
    // Wait for incoming connections on port 5001
    listener.listen(localPort);
    std::cout << "Listening for LP2 on port " << localPort << std::endl;
    
    // Wait for a connection, accept connection
    if(listener.accept(socket) == sf::Socket::Done){ //Connected
      std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
      std::cout << "Sending file..." << std::endl;
      
      //continuously read from file and send as packets to LP2
      bytesRead = PACKET_SIZE;
      //fileBeginning = fileIn.tellg();
      while (bytesRead == PACKET_SIZE){ //PACKET_SIZE //bytesRead==1024
	fileIn.seekg(offset); //starts at 0, add's bytes read each loop, may need tellg()
	fileIn.read(buffer, 1024); //PACKET_SIZE
	bytesRead = fileIn.gcount();
	for(i = bytesRead; i<1024; i++){ //cleanup potential garbage in buffer
	  buffer[i] = '\0';
	}
	std::cout << buffer; //Will print garbage symbols from no '\0'
	status = socket.send(buffer, sizeof(buffer)); //status: disconnect, done
	++count; //Increment # of packets send
	offset += bytesRead;
	for(i = 0; i<1024; i++){ //clear buffer
	  buffer[i] = '\0';
	}
      }
    }
    
    else{
    	std::cout << "Client not connected";	
    }

    std::cout << std::endl << "Sent " << count << " packet(s)";
    std::cout << " and " << offset << " bytes" << std::endl;
    fileIn.close();
    return 0;
}
