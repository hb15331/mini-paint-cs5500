/**
 *  @file   UndoRedo.hpp
 *  @brief  Undo/redo functionality, adjusted to work with network implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <sstream>
#include <vector>
#include "Command.hpp"

/*! \brief Represents either an undo or redoing of a command */
class UndoRedo : public Command
{
  public:
    // Constructor with command description
    UndoRedo(std::string command_description);

    // Destructor
    ~UndoRedo();

    // Turns the UndoRedo into a readable string
    std::string ToString() const override;

    // Turns the UndoRedo into a package that can be sent
    std::vector<sf::Packet> Serialize() const override;

    // Executes the UndoRedo
    bool Execute(sf::Image &image) override;

    // Unused!
    bool Undo(sf::Image &image) override;

    // Unused!
    void Invert() override;

    // UndoRedo is never a component
    bool IsComponent() const override;

    // Check equality with another UndoRedo
    bool IsEqual(const Command &other) override;
};

/*! \brief Override to allow simple printing of an UndoRedo object
  \param strm Stream to add text to
  \param dr UndoRedo to turn into a string and add to stream
*/
inline std::ostream &operator<<(std::ostream &strm, const UndoRedo &me) {
  return strm << me.ToString();
}


#endif // UNDOREDO_HPP
