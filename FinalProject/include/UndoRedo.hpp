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


class UndoRedo : public Command
{
  public:
    UndoRedo(std::string command_description);
    ~UndoRedo();

    std::string ToString() const override;
    std::vector<sf::Packet> Serialize() const override;

    bool Execute(sf::Image &image) override;
    bool Undo(sf::Image &image) override;
    void Invert() override;
    bool IsComponent() const override;

    bool IsEqual(const Command &other) override;

};

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream &operator<<(std::ostream &strm, const UndoRedo &me) {
  return strm << me.ToString();
}


#endif // UNDOREDO_HPP
