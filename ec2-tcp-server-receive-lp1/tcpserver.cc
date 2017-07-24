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
    char textFileName[] = "data.txt";
    int retVal = 0;

    // parse command line arguments, if any
    if (argc < 2) 
        localPort = DATA_PORT;
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

    // create file for text data
    std::ofstream fileOut(textFileName, std::ofstream::out);
    if (!fileOut.is_open())
    {
        std::cout << "Error occurred opening file" << std::endl;
        return -1;
    }
    
	// Wait for incoming connections on port 55001
    listener.listen(localPort);
    std::cout << "Listening on port " << localPort << std::endl;
    
    // Wait for a connection, accept connection
    //retVal = listener.accept(socket);
    if(listener.accept(socket) == sf::Socket::Done){ //Connected
    	std::cout << "New client connected: " << socket.getRemoteAddress() << std::endl;
    	
		//continuously receive and save packets	
    	while (true)
    	{
    		status = socket.receive(buffer, sizeof(buffer), received); //status: disconnect, done
        	++count;
        	if (status != sf::Socket::Done){
	            std::cout << "Error occurred receiving packet #" << count << std::endl;
	            break;
	        }
	        else{
	            // save data stream to file
	            fileOut.write(buffer, received);
	            bytes += received;
	        }
    	}
    }
    
    else{
    	std::cout << "Client not connected";	
    }
    
    std::cout << "Received " << count << " packets";
    std::cout << " and " << bytes << " bytes" << std::endl;
    fileOut.close();
    return 0;
}
