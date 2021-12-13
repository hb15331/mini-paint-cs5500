/**
 *  @file   UDPNetworkClient.cpp
 *  @brief  Client side of UDP implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UDPNetworkClient.hpp"
#include <list>

/*!	\brief Constructor for UDPNetworkClient object, takes username as a parameter.
 * \return UDPNetworkClient object
 */
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

/*!	\brief Destructor.
 */
UDPNetworkClient::~UDPNetworkClient() {
  std::cout << "Client destructor called" << std::endl;
}

/*!	\brief Joins to server if available.
 * \return 1 if connected, 0 if not.
 */
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

/*!	\brief Sends a given packet to the server.
 */
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

/*!	\brief Send a string to the server.
 */
int UDPNetworkClient::sendString(std::string s) {
  sf::Packet stringpacket;
  stringpacket << "String" << s;
  SendPacket(stringpacket);
}

/*!	\brief Receive any information sent from server.
 * \return A packet.
 */
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

/*!	\brief Username getter.
 * \return Username.
 */
std::string UDPNetworkClient::getUsername() { return m_username; }

/*!	\brief Username setter.
 */
int UDPNetworkClient::setUsername(std::string name) { m_username = name; }
