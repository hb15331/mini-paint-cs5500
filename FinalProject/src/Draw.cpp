/**
 *  @file   Draw.cpp
 *  @brief  Draw implementation, all drawing actions are commands.
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/

#include "Draw.hpp"

int m_pixelX, m_pixelY;
sf::Color m_color;
sf::Color m_prevColor;

Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor, sf::Image &image)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_prevColor(image.getPixel(pixelX, pixelY)), m_ptrImage(&image) {}

Draw::Draw(const Draw &obj)
    : Command("Draw"), m_pixelX(obj.m_pixelX), m_pixelY(obj.m_pixelX),
      m_color(obj.m_color),
      m_prevColor((*obj.m_ptrImage).getPixel(obj.m_pixelX, obj.m_pixelY)),
      m_ptrImage(obj.m_ptrImage) {}

/*! \brief 	The isEqual method compares the command with another command
 *object. If the command has the same pixel location and color, the bool value
 *true will be returned. Otherwise false is returned.
 */
bool Draw::isEqual(const Command &other) {
  const Draw *another = dynamic_cast<const Draw *>(&other);
  return another != nullptr && m_pixelX == another->m_pixelX &&
         m_pixelY == another->m_pixelY && m_color == (another->m_color);
}

/*! \brief 	Return true if the command is successfully executed. Otherwise
 * return false.
 *
 */
bool Draw::execute() {
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;
  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize) {
    (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_color);
    return true;
  }
  return false;
}

/*! \brief 	Return true if the command is successfully undone. Otherwise
 * return false.
 *
 */
bool Draw::undo() {
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;
  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize) {
    (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_prevColor);
    return true;
  }
  return true;
}

/*! \brief Converts the Draw into a readable string
 * \return String that represents draw location, size and color
 */
std::string Draw::toString() const {
  std::stringstream ss;
  ss << Command::toString() << ": (" << m_pixelX << "," << m_pixelY << ")"
     << ", sz=" << 1;
  return ss.str();
}
