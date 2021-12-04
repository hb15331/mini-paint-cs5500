/**
 *  @file   Command.cpp
 *  @brief  Implementation of Command.hpp
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/

// Project header files
#include "Command.hpp"

/*! \brief 	N/A
 *
 */
Command::Command(std::string command_description)
    : m_command_description(command_description) {}

/*! \brief Converts the command to a string, useful for debugging */
std::string Command::ToString() const { return m_command_description; }

/*! \brief 	N/A
 *
 */
Command::~Command() {}

/*! \brief Converts the Command into a packet
 * \return Packet form of Command
 */
sf::Packet Command::Serialize() const {
  sf::Packet packet;
  packet << "Command";
  return packet;
}
