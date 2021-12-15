/**
 *  @file   UDPNetworkServer.cpp
 *  @brief  Server side of UDP implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UDPNetworkServer.hpp"

/*!	\brief Constructor for UDPNetworkServer object, takes name,
 *      IP address, and port as params.
 * \param name Name of the server
 * \param IP address of the server\
 * \param port to accept connections on
 * \return UDPNetworkServer object
 */
UDPNetworkServer::UDPNetworkServer(std::string name, sf::IpAddress address,
                                   unsigned short port) {
  m_name = name;
  m_ipAddress = address;
  m_port = port;
  std::cout << "Server Constructor" << std::endl;
}

/*!	\brief Destructor */
UDPNetworkServer::~UDPNetworkServer() {
  std::cout << "Server Destructor" << std::endl;
}

/*!	\brief Starts server, prepares it to send/receive.
 * \return 0 if successful, nonzero if not
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

  m_packet_clock.restart();

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
        // Attempt to deserialize the packet; will be null if packet was empty/malformed
        std::shared_ptr<Command> command = Deserialize(packet);
        if (command) {
          if (command->m_command_description == "Undo") {
            command = Undo();
            if (command == nullptr) {
              continue;
            }
            // Attempt to undo all components of the command (if there are any)
            while (command->IsComponent() && !m_undo.empty() &&
                   m_undo.top()->IsComponent()) {
              if (command != nullptr) {
                packet = command->Serialize()[0];
                packet << "Server";
                std::cout << "Undoing" << std::endl;
                m_packets_to_send.push(packet);
              }
              command = Undo();
            }
            // Get the last one
            if (command != nullptr) {
              packet = command->Serialize()[0];
              packet << "Server";
              std::cout << "Undoing" << std::endl;
              m_packets_to_send.push(packet);
            }

          } else if (command->m_command_description == "Redo") {
            command = Redo();
            if (command == nullptr) {
              continue;
            }
            // Attempt to redo all components of the command (if there are any)
            while (command->IsComponent() && !m_redo.empty() &&
                   m_redo.top()->IsComponent()) {
              if (command != nullptr) {
                packet = command->Serialize()[0];
                packet << "Server";
                std::cout << "Redoing" << std::endl;
                m_packets_to_send.push(packet);
              }
              command = Redo();
            }
            // Get the last one
            if (command != nullptr) {
              packet = command->Serialize()[0];
              packet << "Server";
              std::cout << "Redoing" << std::endl;
              m_packets_to_send.push(packet);
            }
          } else {
            AddCommand(command);
            m_packets_to_send.push(packet);
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

      std::cout << "total messages: " << m_commands.size() << std::endl;

    } else if (status != 1) {
      std::cout << status << std::endl;
    }

    // This is how we handle not sending all messages at once
    // We keep track of when we last sent a packet and only send a new one if some time
    // has passed. If the packet is very large we have to wait longer
    std::map<unsigned short, sf::IpAddress>::iterator ipIterator;
    float send_time = .0001f;
    if (!m_packets_to_send.empty() &&
        m_packets_to_send.front().getDataSize() > 10000) {
      send_time = .3f;
    }
    if (m_packet_clock.getElapsedTime() > sf::seconds(send_time) &&
        !m_packets_to_send.empty()) {

      m_packet_clock.restart();
      for (ipIterator = m_activeClients.begin();
           ipIterator != m_activeClients.end(); ipIterator++) {

        int send_status = m_socket.send(m_packets_to_send.front(),
                                        ipIterator->second, ipIterator->first);

        if (send_status == sf::Socket::Done) {

        } else if (send_status != 1) {
          std::cout << "Problem: " << send_status << std::endl;
        }
      }
      // Confirm successful sending of packet
      m_packets_to_send.pop();
      m_packetHistory.push_back(packet);
    }

  } // End our server while loop
}

/*!	\brief Stops server, disconnects clients.
 * \return false
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

/*!	\brief Processes joining of client
 * \param clientPort port used by client
 * \param clientIpAddress IP address used by client
 */
int UDPNetworkServer::handleClientJoining(unsigned short clientPort,
                                          sf::IpAddress clientIpAddress) {
  std::cout << "Updating new client" << std::endl;
  sf::Packet stringpacket;
  stringpacket << "String"
               << "Initialize";

  if (m_socket.send(stringpacket, clientIpAddress, clientPort) !=
      sf::Socket::Done) {
    std::cout << "Server error? Wrong IP?" << std::endl;
  } else {
    std::cout << "Server sending init packet" << std::endl;
  }

  // Client catch-up code. Send all packets in packet history to the client
  sf::sleep(sf::seconds(1.f));
  std::cout << m_packetHistory.size() << std::endl;
  for (int i = 0; i < m_packetHistory.size(); i++) {
    std::cout << "Sending newbie packet" << std::endl;
    // Working around udp buffer restrictions
    sf::sleep(sf::seconds(.02f));
    if (i % 10 == 0) {
      sf::sleep(sf::seconds(.3f));
    }
    if (m_packetHistory.at(i).getDataSize() > 10000) {
      sf::sleep(sf::seconds(.15f));
    }
    int send_status =
        m_socket.send(m_packetHistory.at(i), clientIpAddress, clientPort);
    if (send_status == sf::Socket::Done) {

    } else if (send_status != 1) {
      std::cout << "Problem: " << send_status << std::endl;
    }
  }
}

/*! \brief 	Undo the latest command that has executed.
 *		The command that's been undone can be invoked again in redo.
 * \return the undoing version of the command
 */
std::shared_ptr<Command> UDPNetworkServer::Undo() {
  if (!m_undo.empty()) {
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
 * \return The redoing version of the command
 */
std::shared_ptr<Command> UDPNetworkServer::Redo() {
  if (!m_redo.empty()) {
    m_undo.push(m_redo.top());
    auto redo = m_redo.top();
    m_redo.pop();
    redo->Invert();
    return redo;
  }
}

/*! \brief Adds command to relevant queue and stack.
 * \param c Command to add to the queue and undo stack
 */
void UDPNetworkServer::AddCommand(std::shared_ptr<Command> c) {
  if (c && (m_undo.empty() || !(c->IsEqual(*m_undo.top())))) {
    m_commands.push(c);
    m_undo.push(c);
  }
}

/*! \brief Processes departure of client (not in use)
 */
int UDPNetworkServer::handleClientLeaving() { return 0; }
