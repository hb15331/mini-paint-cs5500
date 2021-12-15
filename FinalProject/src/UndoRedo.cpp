/**
 *  @file   UndoRedo.cpp
 *  @brief  Undo/redo functionality, adjusted to work with network
 *implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UndoRedo.hpp"

/*!	\brief Undo object constructor, takes command as param.
 * \param command_description word describing the Undo/Redo
 * \return Undo object.
 */
UndoRedo::UndoRedo(std::string command_description)
    : Command(command_description) {}

/*!	\brief Destructor
 */
UndoRedo::~UndoRedo() {}

/*! \brief Converts the UndoRedo into a readable string.
 * \return String that represents whether undo or redo.
 */
std::string UndoRedo::ToString() const {
  std::stringstream ss;
  ss << Command::ToString();
  return ss.str();
}

/*! \brief Converts the Undo/Redo into a packet.
 * \return Packet that represents the Undo/Redo command.
 */
std::vector<sf::Packet> UndoRedo::Serialize() const {
  std::cout << "Serializing" << std::endl;
  sf::Packet packet;
  packet << "UndoRedo" << m_command_description;
  std::vector<sf::Packet> vec;
  vec.push_back(packet);
  return vec;
}

/*! Returns false because undos/redos are never part of a larger command
 * \return false
 */
bool UndoRedo::IsComponent() const { return false; }

/*! \brief 	The isEqual method compares the command with another command
 *object.
 * \param other Command to compare to
 * \return true if equal commands, false otherwise
 */
bool UndoRedo::IsEqual(const Command &other) {
  const UndoRedo *another = dynamic_cast<const UndoRedo *>(&other);
  return another != nullptr &&
         m_command_description == another->m_command_description;
}

/*!	\brief Undos aren't executed in this manner so do nothing
 * \return True
 */
bool UndoRedo::Execute(sf::Image &image) { return true; }

/*!	\brief Undos can't be undone. Interesting possible extension is making undos themselves
 * undoable. Emacs does this, and it essentially functions as a redo but prevents you from
 * ever losing the redo stack if you add a new command after undoing.
 * \return True
 */
bool UndoRedo::Undo(sf::Image &image) { return true; }

/*! \brief Unused */
void UndoRedo::Invert() {}
