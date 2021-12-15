/**
 *  @file   Command.hpp
 *  @brief  Represents an actionable command by the user.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Network.hpp>
#include <SFML/Graphics/Image.hpp>

/*! \brief Represents a command that can be executed to serve some function, and sent
 * to/from the server */
class Command {
public:
  // Textual description of the command
  const std::string m_command_description;

  // Constructor
  Command(std::string command_description);

  // Destructor for a command
  virtual ~Command();

  // Returns true if two commands are equal, returns false otherwise.
  virtual bool IsEqual(const Command &other) = 0;

  // Returns true or false if the command was able to succssfully
  // execute.
  virtual bool Execute(sf::Image &image) = 0;
  virtual bool Undo(sf::Image &image) = 0;

  // Inverts the command
  virtual void Invert() = 0;

  // Converts the command to a user-readable string
  virtual std::string ToString() const;

  // Turns the Command into a packet for sending to/from server
  virtual std::vector<sf::Packet> Serialize() const;

  // Is the command part of a larger set of subsequent/precequent commands
  virtual bool IsComponent() const = 0;
};

/*! \brief Allow Commands to be printed with std::cout
  \param strm Stream to add text to
  \param cm Command to turn into a string and add to stream
*/
inline std::ostream &operator<<(std::ostream &strm, const Command &cm) {
  return strm << cm.ToString();
}

#endif
