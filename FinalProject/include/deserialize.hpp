#ifndef DESERIALIZE_HPP_INCLUDED
#define DESERIALIZE_HPP_INCLUDED

#include <exception>

/*! \brief turns a packet back into a command
 * \return Command form of packet
 */
inline std::shared_ptr<Command> Deserialize(sf::Packet packet) {
  std::string cmd_type, username;
  int xcoord, ycoord, sz;
  uint8_t r, g, b, a, p_r, p_g, p_b, p_a;
  if (!(packet >> cmd_type) &&
      packet.getDataSize() > 0) {
    std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
    return nullptr;
  }
  if (cmd_type == "Draw") {
    if (!(packet >> xcoord >> ycoord >> sz >> r >> g >> b >> a >>
        p_r >> p_g >> p_b >> p_a >> username) &&
      packet.getDataSize() > 0) {
      std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
      return nullptr;
    }
    return (std::make_shared<Draw>(xcoord, ycoord, sf::Color(r, g, b, a),
                                   sf::Color(p_r, p_g, p_b, p_a)));
  } else if (cmd_type == "String") {
    std::string out;
    std::cout << "String packet\n" << out << std::endl;
    return nullptr;
  } else {
    throw std::runtime_error("Unknown command");
  }
}

#endif // DESERIALIZE_HPP_INCLUDED
