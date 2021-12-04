/**
 *  @file   Draw.hpp
 *  @brief  Drawing actions interface.
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Network.hpp>
// Include standard library C++ libraries.
#include <sstream>
#include <iostream>
#include <string>
// Project header files
#include "Command.hpp"

// Anytime we want to implement a new command in our paint tool,
// we have to inherit from the command class.
// This forces us to implement an 'execute' and 'undo' command.
//
// In our Draw class note that we have also explicitly marked the
// functions as 'override' to indicate that these functions are
// overriding the behavior of functions in a base class
// that are implemented as virtual.
class Draw : public Command {
private:
  int m_pixelX, m_pixelY;
  sf::Color m_color;
  sf::Color m_prevColor;
  // pointer to the image
  sf::Image *m_ptrImage;
  Draw &operator=(const Draw &) = delete;

public:
  Draw(int pixelX, int pixelY, const sf::Color &newColor, sf::Image &image);
  Draw(int pixelX, int pixelY, const sf::Color &newColor, const sf::Color &prevColor);
  Draw(const Draw &obj);
  bool IsEqual(const Command &other) override;
  bool Execute() override;
  std::string ToString() const;
  bool Undo() override;
  sf::Packet Serialize() const;
};

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream &operator<<(std::ostream &strm, const Draw &dr) {
  return strm << dr.ToString();
}

#endif
