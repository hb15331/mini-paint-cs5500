#include "UDPNetworkClient.hpp"
#include <list>

// Default Constructor
// In our constructor, we will want to setup our clients
// 1.) username
// 2.) port
//
// Then we will bind a socket to this port for which we will
// communicate over.
//
// Our UDPNetworkClient will be non-blocking by default
UDPNetworkClient::UDPNetworkClient(std::string username) {
  // Adding in a list of valid ports to automate the binding process.
  int valid_ports[] = {50001, 50002, 50003, 50004, 50005,
                       50006, 50007, 50008, 50009, 50010};
  std::list<int>::iterator it;
  m_username = username;
  // m_port = port;
  // Setup a socket for a UDP connection
  // Multiple computers can have the same port
  // However, the IP addresses would need to be different.
  // Listening to a port
  // m_socket.bind(m_port);

  // Logic for testing ports to see which are available.
  for (int i = 0; i < 10; ++i) {
    std::cout << "Port:" << valid_ports[i] << std::endl;
    sf::Socket::Status status = m_socket.bind(valid_ports[i]);
    if (status == sf::Socket::Status::Done) {
      std::cout << "Status after bind is: Done." << std::endl;
      break;
    } else if (i == 9 and status != sf::Socket::Status::Done) {
      std::cout << "Too many users on the server, bind failed!" << std::endl;
    } else {
      std::cout << "Status after bind is: Failed. Moving to next port."
                << std::endl;
    }
  }

  // By default socket is non-blocking
  m_socket.setBlocking(false);
}

// Default Destructor
UDPNetworkClient::~UDPNetworkClient() {
  std::cout << "Client destructor called" << std::endl;
}

// Join a server
// For this function, we will setup the server for which we will attempt to
// communicate Since this is a UDP network, we will keep things quite simple,
// meaning we will not do any error checking to see if the server even exists.
// We simply try to send messages to a server.
int UDPNetworkClient::joinServer(sf::IpAddress serverAddress,
                                 unsigned short serverPort) {
  std::cout << "UDPClient will attempt to join server: " << std::endl;
  m_serverIpAddress = serverAddress;
  m_serverPort = serverPort;


  sendString("Hello, " + m_username + " is joining!");

  sf::Clock clock;
  clock.restart();
  while(clock.getElapsedTime() < sf::seconds(2)) {
      sf::Packet pack = ReceiveData();
      std::string initString, message;
      if (pack >> initString >> message) {
        if (initString == "String") {
          std::cout << message << std::endl;
          return 1; // Online
        }
      }
  }
  return 0; // Offline

}

// Send data to server
// Now is the fun part, for which we will attempt to send
// some data to the UDPNetworkServer.
//
// In this example, we will try to send over commands.
int UDPNetworkClient::SendPacket(sf::Packet packet) {
  packet << m_username;

  std::cout << "Packet Data:" << packet << std::endl;

  if (m_socket.send(packet, m_serverIpAddress, m_serverPort) !=
      sf::Socket::Done) {
    std::cout << "Client error? Wrong IP?" << std::endl;
  } else {
    std::cout << "Client(" << m_username << ") sending packet" << std::endl;
  }
  return 0;
}

// Send 'string' data to server
// As an alternative, we may want to send string messages to the server.
// This can be a quick way to log, or perhaps 'chat' with the server
int UDPNetworkClient::sendString(std::string s) {
  sf::Packet stringpacket;
  stringpacket << "String" << s;
  SendPacket(stringpacket);
}

sf::Packet UDPNetworkClient::ReceiveData() {
  sf::IpAddress copyAddress = m_serverIpAddress;
  unsigned short copyPort = m_serverPort;
  sf::Packet packet;
  sf::Socket::Status status = m_socket.receive(packet, copyAddress, copyPort);

  if (status == sf::Socket::Done) {
    return packet;
  } else {
    return packet;
  }
}
// Return our username
std::string UDPNetworkClient::getUsername() { return m_username; }

// Set our username
int UDPNetworkClient::setUsername(std::string name) { m_username = name; }
