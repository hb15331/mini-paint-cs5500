/**
 *  @file   UndoRedo.cpp
 *  @brief  Undo/redo functionality, adjusted to work with network implementation.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UndoRedo.hpp"

/*!	\brief Undo object constructor, takes command as param.
 * \return Undo object.
 */
UndoRedo::UndoRedo(std::string command_description) : Command(command_description)
{
}

/*!	\brief Destructor
 */
UndoRedo::~UndoRedo()
{
}

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
sf::Packet UndoRedo::Serialize() const {
  std::cout << "Serializing" << std::endl;
  sf::Packet packet;
  packet << "UndoRedo" << m_command_description;
  return packet;
}

/*! \brief 	The isEqual method compares the command with another command
 *object.
 */
bool UndoRedo::IsEqual(const Command &other) {
  const UndoRedo *another = dynamic_cast<const UndoRedo *>(&other);
  return another != nullptr && m_command_description == another->m_command_description;
}

/*!	\brief Executes operation.
 * \return True
 */
bool UndoRedo::Execute(){
  return true;
}

/*!	\brief Executes operation, taking image reference as param.
 * \return True
 */
bool UndoRedo::Execute(sf::Image &image)  {
  return true;
}

/*!	\brief Undoes operation, taking image reference as param.
 * \return True
 */
bool UndoRedo::Undo(sf::Image &image) {
  return true;
}


/*! \brief inverts the command for undoing purposes*/
void UndoRedo::Invert() {
}
