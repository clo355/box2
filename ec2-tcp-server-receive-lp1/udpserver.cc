#include <SFML/Network/Socket.hpp>
#include <SFML/Network/UdpSocket.hpp>
#include <SFML/Network/Packet.hpp>
#include <SFML/Network/IpAddress.hpp>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <fstream>

#include "network.h"

int main(int argc, char *argv[])
{
    sf::UdpSocket socket;
    sf::Socket::Status status;
    sf::IpAddress remoteAddr;
    unsigned short localPort = 0;
    unsigned short remotePort = 0;
    unsigned long count = 0;
    unsigned long bytes = 0;
    std::size_t received = 0;
    char buffer[PACKET_SIZE];
    char audioFileName[] = "message.pcm";

    // parse command line arguments, if any
    if (argc < 2) 
        localPort = AUDIO_PORT;
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
            localPort = AUDIO_PORT;
        }   
    }

    // create file for audio data
    std::ofstream audioOut(audioFileName, std::ofstream::out);
    if (!audioOut.is_open())
    {
        std::cout << "Error occurred opening file" << std::endl;
        return -1;
    }

    // bind the listener to a port
    if (socket.bind(localPort) != sf::Socket::Done)
    {
        std::cout << "Error occurred binding port" << std::endl;
        return -2;
    }
    std::cout << "Listening on port " << localPort << std::endl;

    // continuously get packets
    while (true)
    {
        status = socket.receive(buffer, sizeof(buffer), received, remoteAddr, remotePort);
        ++count;
        if (status != sf::Socket::Done)
        {
            std::cout << "Error occurred receiving packet #" << count << std::endl;
        }
        else
        {
            // save audio stream to file
            audioOut.write(buffer, received);
            bytes += received;
        }
    }
    std::cout << "Received " << count << " packets";
    std::cout << " and " << bytes << " bytes" << std::endl;
    audioOut.close();
    return 0;
}
