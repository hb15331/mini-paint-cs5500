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
  if (!(packet >> cmd_type >> xcoord >> ycoord >> sz >> r >> g >> b >> a >>
        p_r >> p_g >> p_b >> p_a >> username)) {
    std::cout << "Error deserializing packet " << packet << std::endl;
  }
  if (cmd_type == "Draw") {

    return (std::make_shared<Draw>(xcoord, ycoord, sf::Color(r, g, b, a),
                                   sf::Color(p_r, p_g, p_b, p_a)));
  } else {
    throw std::runtime_error("Unknown command");
  }
}

#endif // DESERIALIZE_HPP_INCLUDED
