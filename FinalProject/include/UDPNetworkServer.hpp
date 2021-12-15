/**
 *  @file   UDPNetworkServer.hpp
 *  @brief  Server side of UDP implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef UDP_NETWORK_SERVER_HPP
#define UDP_NETWORK_SERVER_HPP

// Include our Third-Party SFML Header
#include <SFML/Network.hpp>

// Our Command library
#include "Command.hpp"
#include "Draw.hpp"
#include "deserialize.hpp"

// Other standard libraries
#include <string>
#include <vector>
#include <queue>
#include <stack>

/*! \brief Non-blocking UDP server used to distribute command packets.
 * UDP was used to enable easier handling of multiple connections and because
 * the data to be sent is small and numerous
 */
class UDPNetworkServer {
public:
  // Default Constructor
  UDPNetworkServer(std::string name, sf::IpAddress address,
                   unsigned short port);

  // Default Destructor
  ~UDPNetworkServer();

  // Adds a command to the command queue
  void AddCommand(std::shared_ptr<Command> c);

  // Undo last command
  std::shared_ptr<Command> Undo();

  // Redo last command
  std::shared_ptr<Command>Redo();

  // Start the server
  int start();

  // Stops the server from running and removes all clients
  int stop();

  //Bind to port
  bool bindToPort();

  //Receive a packet
  bool receivePacket();

private:
  // Queue stores the next command to do.
  std::queue<std::shared_ptr<Command>> m_commands;

  // Stack that stores the last action to occur.
  std::stack<std::shared_ptr<Command>> m_undo;

  // Stack that stores the last action to redo.
  std::stack<std::shared_ptr<Command>> m_redo;

  // What to do when the client joins the server
  int handleClientJoining(unsigned short clientPort,
                          sf::IpAddress clientIpAddress);
  // What to do when the client leaves the server
  int handleClientLeaving();

  // Name for our server
  std::string m_name;

  // Flag for if the server should stop.
  bool m_start;

  // Ip Address for our UDP Server
  sf::IpAddress m_ipAddress;

  // Port for the server
  unsigned short m_port;

  // A UDP Socket for our server
  sf::UdpSocket m_socket;

  // A data structure to hold all of the clients.
  // The first parameter is the port from which
  // data was received.
  // The second is the IpAddress of the client.
  std::map<unsigned short, sf::IpAddress> m_activeClients;

  // A data structure to hold all of the packets
  std::vector<sf::Packet> m_packetHistory;

  // Packets not yet sent
  std::queue<sf::Packet> m_packets_to_send;

  // Timer to avoid overloading packets
  sf::Clock m_packet_clock;
};

#endif
