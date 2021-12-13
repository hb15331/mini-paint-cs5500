/**
 *  @file   Serializer.cpp
 *  @brief  Serializer for sending packets.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "Serializer.hpp"

/*!	\brief Constructor for a Serializer object, takes command as param.
 * \return Return Serializer object.
 */
sf::Packet Serializer::Serialize (Command command) {
  return command.Serialize();
}

/*!	\brief Serialize a command and create a packet.
 * \return Packet.
 */
sf::Packet Serializer::Serialize (std::string command_str) {
  sf::Packet packet;
  switch (command_str) {
  case "Undo":
    packet << "Undo";
    break
  case "Redo":
    packet << "Redo";
    break;
  default:
    throw std::runtime_error ("Unknown command string '" + command_str + "'");
  }
  return packet;
}
