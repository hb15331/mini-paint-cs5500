#include "Serializer.hpp"


sf::Packet Serializer::Serialize (Command command) {
  return command.Serialize();
}

sf::Packet Serializer::Serialize (std::string command_str) {
  sf::Packet packet;
  switch (command_str) {
  case "Undo":
    packet << "Undo";
    break
  case "Redo":
    packet << "Redo";
    break;
  default:
    throw std::runtime_error ("Unknown command string '" + command_str + "'");
  }
  return packet;
}
