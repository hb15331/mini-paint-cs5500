/**
 *  @file   Command.cpp
 *  @brief  Implementation of Command.hpp
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/

// Project header files
#include "Command.hpp"

/*! \brief Unimplemented because abstract class */
Command::Command() : m_command_description() {}

/*! \brief Initialize command description. Protected member function. */
Command::Command(std::string command_description) : m_command_description(command_description) {}

/*! \brief Left unimplemented because abstract class */
Command::~Command(){}

/*! \brief Converts the command to a string, useful for debugging */
std::string Command::toString() const {
  return m_command_description;
}

/*! \brief Checks for equality between two commands
 *
 * \param Command to compare this Command to
 * \return true if tye Commands are equal, false if not
 */
bool Command::operator==(const Command& other) const {
    return typeid(*this) == typeid(other) && isEqual(other);
}
