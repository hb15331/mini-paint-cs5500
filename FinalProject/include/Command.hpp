/**
 *  @file   Command.hpp
 *  @brief  Represents an actionable command by the user.
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>

// The command class
class Command{
private:
	std::string m_commandDescription;
public:
	Command(std::string commandDescription);
	// Destructor for a command
	virtual ~Command();
	// Returns true if two commands are equal, returns false otherwise.
	virtual bool isEqual(const Command& other) = 0;
	// Returns true or false if the command was able to succssfully
	// execute.
	// If it executes successfully, then that command is added to the
	// undo stack.
	// Each parameters also takes in a string name. While this may not be optimal,
	// it allows us to easily debug what each command is doing in a textual form.
	virtual bool execute() = 0;
	virtual bool undo() = 0;
};



#endif