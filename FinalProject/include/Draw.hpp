/**
 *  @file   Draw.hpp
 *  @brief  Drawing actions interface.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
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
#include <vector>
// Project header files
#include "Command.hpp"

/*! \brief Anytime we want to implement a new command in our paint tool,
 * we have to inherit from the command class.
 * This forces us to implement an 'execute' and 'undo' command.
 *
 * In our Draw class note that we have also explicitly marked the
 * functions as 'override' to indicate that these functions are
 * overriding the behavior of functions in a base class
 * that are implemented as virtual.
*/
class Draw : public Command {
private:
  // Location to draw center of pixel
  int m_pixelX, m_pixelY;

  // Pixel color
  sf::Color m_color;

  // Old colors used for undoing
  std::vector<std::vector<sf::Color>> m_old_colors;

  // Size of the pixel
  const int m_pen_size;

  // Used for undoing
  bool m_inverted;

  // use IsEqual()
  Draw &operator=(const Draw &) = delete;

  // Stores whether the draw is a component of more commands
  bool m_is_component;


public:
  Draw(int pixelX, int pixelY, const sf::Color &newColor, sf::Image &image, const int pen_size, bool inverted=false, bool is_component=false);
  Draw(int pixelX, int pixelY, const sf::Color &newColor, const std::vector<std::vector<sf::Color>> &oldColors, const int pen_size, bool inverted=false, bool is_component=false);
  Draw(const Draw &obj);
  bool IsEqual(const Command &other) override;
  bool Execute(sf::Image &image) override;
  void Invert() override;
  std::string ToString() const override;
  bool Undo(sf::Image &image) override;
  std::vector<sf::Packet> Serialize() const override;
  bool IsComponent() const override;
};

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream &operator<<(std::ostream &strm, const Draw &dr) {
  return strm << dr.ToString();
}

#endif
