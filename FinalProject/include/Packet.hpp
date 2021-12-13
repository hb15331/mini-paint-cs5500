/**
 *  @file   packet.hpp
 *  @brief  Packet implementation (based on SFML).
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef PACKET_HPP
#define PACKET_HPP

// Include our Third-Party SFML Header
#include <SFML/Network.hpp>

// Our Command library
#include "Command.hpp"
#include "Draw.hpp"

// Other standard libraries
#include <memory>
#include <string>
#include <vector>

// We can inhert from the SFML Packet and create our
// own custom packets that have our information in them.
// This gives us all of the functionality that we need
// when we want to send some custom information.
//
// The pieces of information we want to send are the
// client who last performed the action, and the
// command which they did. Then when the server receives
// these commands, it can store them, and broadcast this
// information to all of the other clients who have
// previously connected.
class myPacket : public sf::Packet {
public:
  std::string m_client;
  std::string m_command_string;

  myPacket() { m_client = "whichever client I am"; }

  ~myPacket() {
  }
};

#endif
