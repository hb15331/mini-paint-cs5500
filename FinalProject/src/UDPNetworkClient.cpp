/**
 *  @file   UDPNetworkClient.cpp
 *  @brief  Client side of UDP implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UDPNetworkClient.hpp"

/*!	\brief Constructor for UDPNetworkClient object, takes username as a
 * parameter.
 * \param username the username to log into the server with
 * \return UDPNetworkClient object
 */
UDPNetworkClient::UDPNetworkClient(std::string username)
    : m_username(username), m_port(), m_ipAddress(), m_serverPort(),
      m_serverIpAddress(), m_socket() {
  // Adding in a list of valid ports to automate the binding process.
  int valid_ports[] = {50001, 50002, 50003, 50004, 50005,
                       50006, 50007, 50008, 50009, 50010};
  std::list<int>::iterator it;
  m_username = username;

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
 * \param serverAddress the server to connect to
 * \param serverPort the port to connect to
 * \return 1 if connected, 0 if not.
 */
int UDPNetworkClient::joinServer(sf::IpAddress serverAddress,
                                 unsigned short serverPort) {
  std::cout << "UDPClient will attempt to join server: " << std::endl;
  m_serverIpAddress = serverAddress;
  m_serverPort = serverPort;

  // Lets the server know we're joining
  sendString("Hello, " + m_username + " is joining!");

  /*
  * We continually wait for a reply from the server, if we get none then we go into
  * offline mode
  */
  sf::Clock clock;
  clock.restart();
  while (clock.getElapsedTime() < sf::seconds(1.5f)) {
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
 * \param The packet to send
 * \return 0 if successfully sent the packet, not 0 if we didn't
 */
int UDPNetworkClient::SendPacket(sf::Packet packet) {
  packet << m_username;
  if (m_socket.send(packet, m_serverIpAddress, m_serverPort) !=
      sf::Socket::Done) {
    std::cout << "Client error? Wrong IP?" << std::endl;
    return -1;
  } else {
    std::cout << "Client(" << m_username << ") sending packet" << std::endl;
  }
  return 0;
}

/*!	\brief Send a string to the server.
 * \param s The string to send to the server
 * \return 0 if successful, nonzero if not
 */
int UDPNetworkClient::sendString(std::string s) {
  sf::Packet stringpacket;
  stringpacket << "String" << s;
  return SendPacket(stringpacket);
}

/*!	\brief Receive any information sent from server.
 * \return A packet if any are received, otherwise an empty packet
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
 * \return Username
 */
std::string UDPNetworkClient::getUsername() { return m_username; }

/*!	\brief Username setter
 * \param name new username
 */
int UDPNetworkClient::setUsername(std::string name) {
  m_username = name;
  return 1;
}
