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

#include <SFML/Network.hpp>
#include <SFML/Graphics/Image.hpp>

// The command class
class Command {
public:
  const std::string m_command_description;

  Command(std::string command_description);
  // Destructor for a command
  virtual ~Command();
  // Returns true if two commands are equal, returns false otherwise.
  virtual bool IsEqual(const Command &other) = 0;
  // Returns true or false if the command was able to succssfully
  // execute.
  // If it executes successfully, then that command is added to the
  // undo stack.
  // Each parameters also takes in a string name. While this may not be optimal,
  // it allows us to easily debug what each command is doing in a textual form.
  virtual bool Execute() = 0;
  virtual bool Execute(sf::Image &image) = 0;
  virtual bool Undo(sf::Image &image) = 0;

  // Inverts the command
  virtual void Invert() = 0;

  //! Converts the command to a user-readable string
  virtual std::string ToString() const;

  virtual sf::Packet Serialize() const;
};

/*! \brief Allow Commands to be printed with std::cout */
inline std::ostream &operator<<(std::ostream &strm, const Command &cm) {
  return strm << cm.ToString();
}

#endif
