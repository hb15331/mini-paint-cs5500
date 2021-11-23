/** 
 *  @file   Command.hpp 
 *  @brief  Represents an actionable command by the user.
 *  @author Mike and Jack
 *  @date   2021-18-10
 ***********************************************/
#ifndef COMMAND_HPP
#define COMMAND_HPP

// Include standard library C++ libraries.
#include <string>
#include <typeinfo>

/*! \brief Represents a general command that can be issued as a result of user input.
 * All commands can be executed or undone
 */
class Command{
private:
    // Holds description of the command
    std::string m_command_description;
    // Checks equality given that the types match
    virtual bool isEqual(const Command& other) const = 0;

protected:
    // Construct the command with no args (hidden)
    Command();
    // Construct the command with a description
    Command(std::string command_description);

public:
    // Destructor
    virtual ~Command();
    /*! \brief Executes the command */
    virtual bool execute() = 0;
    /*! \brief Undoes the command */
    virtual bool undo() = 0;
    //! Converts the command to a user-readable string
    virtual std::string toString() const;
    //! Checks equality with another command
    bool operator==(const Command& other) const;

};

/*! \brief Allow Commands to be printed with std::cout */
inline std::ostream& operator<<(std::ostream &strm, const Command& cm) {
  return strm << cm.toString();
}

#endif
