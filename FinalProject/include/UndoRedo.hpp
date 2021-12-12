#ifndef UNDOREDO_HPP
#define UNDOREDO_HPP

#include <sstream>
#include "Command.hpp"


class UndoRedo : public Command
{
  public:
    UndoRedo(std::string command_description);
    ~UndoRedo();

    std::string ToString() const override;
    sf::Packet Serialize() const override;

    bool Execute() override;
    bool Execute(sf::Image &image) override;
    bool Undo(sf::Image &image) override;
    void Invert() override;

    bool IsEqual(const Command &other) override;


  protected:

  private:
};

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream &operator<<(std::ostream &strm, const UndoRedo &me) {
  return strm << me.ToString();
}


#endif // UNDOREDO_HPP
