/**
 *  @file   Command.cpp
 *  @brief  Implementation of Command.hpp
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

// Project header files
#include "Command.hpp"

/*! \brief 	Constructor for new command object, which takes
                a description in string form as a parameter.
 *
 */
Command::Command(std::string command_description)
    : m_command_description(command_description) {}

/*! \brief Converts the command to a string, useful for debugging */
std::string Command::ToString() const { return m_command_description; }

/*! \brief 	Command destructor.
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
