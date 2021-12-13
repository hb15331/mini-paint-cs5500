/**
 *  @file   UDPNetworkServer.cpp
 *  @brief  Server side of UDP implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UDPNetworkServer.hpp"
#include "Draw.hpp"

#include <iostream>

/*!	\brief Constructor for UDPNetworkServer object, takes name,
 *      IP address, and port as params.
 * \return UDPNetworkServer object
 */
UDPNetworkServer::UDPNetworkServer(std::string name, sf::IpAddress address,
                                   unsigned short port) {
  m_name = name;
  m_ipAddress = address;
  m_port = port;
  std::cout << "Server Constructor" << std::endl;
}
/*!	\brief Destructor.
 */
UDPNetworkServer::~UDPNetworkServer() {
  std::cout << "Server Destructor" << std::endl;
}

/*!	\brief Starts server, prepares it to send/receive.
 */
int UDPNetworkServer::start() {
  std::cout << "Starting UDP Network server" << std::endl;
  std::cout << "Server's IP address is: " << m_ipAddress << std::endl;
  // Attempt to bind to a port
  int status;
  if (status = (m_socket.bind(m_port) != sf::Socket::Done)) {
    std::cout << "Unable to bind -- Error: " << status << std::endl;
    return status;
  }

  // Start the server
  // TODO In the future you may add reason to stop or pause the server
  // as other possible states.
  m_start = true;

  // We are going to create a non-blocking UDP server..
  m_socket.setBlocking(false);

  // Start a server that will run forever
  while (m_start) {

    // When a connection is made to our server, we get some amount of bytes
    // We store the clients ip and port that is connected
    // FIXME for now '128' is a magic number we have 'guessed' is big enough
    // char in[128];
    sf::Packet packet;
    sf::IpAddress senderIp;
    size_t received;
    unsigned short senderPort;
    // If the server receives a message
    // then we want to broadcast that message out to
    // all potential folks who have joined our server.

    sf::Socket::Status status =
        m_socket.receive(packet, m_ipAddress, senderPort);

    if (status == sf::Socket::Done) {

      try {
        std::shared_ptr<Command> command = Deserialize(packet);
        if (command) {
          if (command->m_command_description == "Undo") {
            command = Undo();
            packet = command->Serialize();
            packet << "Server";
            std::cout << "Undoing" << std::endl;

          } else if (command->m_command_description == "Redo") {
            command = Redo();
            packet = command->Serialize();
            packet << "Server";
          } else {
            AddCommand(command);

          }
          if (command) {
            std::cout << *command << std::endl;
          }
        }
      } catch (std::runtime_error e) {
        std::cout << e.what() << std::endl;
      }

      // Check if this is the first message sent by the client
      // by iterating through all of our current clients.
      // If we reach the end of the map, then we know this is a
      // new client.
      std::map<unsigned short, sf::IpAddress>::iterator clientIterator;
      clientIterator = m_activeClients.find(senderPort);
      if (clientIterator == m_activeClients.end()) {
        std::cout << "First time joiner!" << std::endl;
        handleClientJoining(senderPort, senderIp);
        // Add client to our activeClients map so we can send them more
        // messages that come in
        m_activeClients[senderPort] = senderIp;
      }
      // TODO: What happens if lots of messages are sent all at once?
      // Could this be a bottleneck?
      // One such fix, is to have each client joined as a separate thread
      // for the server which receives messages. 1 socket thus per connection
      // and then we may then also have a lock on any shared data structures.
      // m_sentHistory.push_back(in);
      std::cout << "total messages: " << m_commands.size() << std::endl;
      // We create an iterator that looks through our map
      // For each of our clients we are going to send to them
      // (including the client that just joined) the message that
      // was just received.
      std::map<unsigned short, sf::IpAddress>::iterator ipIterator;
      for (ipIterator = m_activeClients.begin();
           ipIterator != m_activeClients.end(); ipIterator++) {
        m_socket.send(packet, ipIterator->second, ipIterator->first);
      }
    } else if (status != 1) {
      std::cout << status << std::endl;
    }

  } // End our server while loop
}

/*!	\brief Stops server, disconnects clients.
 */
int UDPNetworkServer::stop() { m_start = false; }

/*!	\brief Binds server to a given port.
 * \return True or false  depending on whether bind is successful.
 */
bool UDPNetworkServer::bindToPort() {
  int status;
  status = m_socket.bind(m_port);
  if (status != sf::Socket::Done) {
    return false;
  }
  return true;
}

/*!	\brief Receives a packet if possible.
 * \return True or false depending on whether a packet is received.
 */
bool UDPNetworkServer::receivePacket() {
  m_socket.setBlocking(false);
  sf::Packet packet;
  sf::IpAddress senderIp;
  size_t received;
  unsigned short senderPort;
  sf::Socket::Status status = m_socket.receive(packet, m_ipAddress, senderPort);
  if (status == sf::Socket::Done) {
    return true;
  }
  return false;
}

/*!	\brief Processes joining of client.
 */
int UDPNetworkServer::handleClientJoining(unsigned short clientPort,
                                          sf::IpAddress clientIpAddress) {
  std::cout << "Updating new client" << std::endl;
  sf::Packet stringpacket;
  stringpacket << "String" << "Initialize";

  if (m_socket.send(stringpacket, clientIpAddress, clientPort) !=
      sf::Socket::Done) {
    std::cout << "Server error? Wrong IP?" << std::endl;
  } else {
    std::cout << "Server sending init packet" << std::endl;
  }

  sf::sleep(sf::seconds(3.f));

  // Iterate through every message sent and send it to the client.
  for (int i = 0; i < m_sentHistory.size(); i++) {
    char in[128];
    m_socket.send(m_sentHistory[i].c_str(), m_sentHistory[i].length() + 1,
                  clientIpAddress, clientPort);
  }
}

/*! \brief 	Undo the latest command that has executed.
 *		The command that's been undone can be invoked again in redo.
 */
std::shared_ptr<Command> UDPNetworkServer::Undo()
{
  if (!m_undo.empty())
  {
    std::shared_ptr<Command> undo = m_undo.top();
    undo->Invert();
    m_redo.push(undo);
    m_undo.pop();
    return undo;
  } else {
    return nullptr;
  }
}

/*! \brief 	Redo the latest command that has undone.
 *		The command that's been re-done can be invoked again in undo.
 */
std::shared_ptr<Command> UDPNetworkServer::Redo()
{
  if (!m_redo.empty())
  {
    m_undo.push(m_redo.top());
    auto redo =  m_redo.top();
    m_redo.pop();
    redo->Invert();
    return redo;
  }
}

/*! \brief Adds command to relevant queue and stack.
 */
void UDPNetworkServer::AddCommand(std::shared_ptr<Command> c) {
  if(c) {
    m_commands.push(c);
    m_undo.push(c);
  }
}

/*! \brief Processes departure of client (not in use)
 */
int UDPNetworkServer::handleClientLeaving() {}
