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
      m_prevColor(image.getPixel(pixelX, pixelY)), m_ptrImage(&image), m_inverted(false) {}

Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor,
           const sf::Color &prevColor)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_prevColor(prevColor), m_ptrImage(nullptr), m_inverted(false) {}

Draw::Draw(const Draw &obj)
    : Command("Draw"), m_pixelX(obj.m_pixelX), m_pixelY(obj.m_pixelX),
      m_color(obj.m_color),
      m_prevColor((*obj.m_ptrImage).getPixel(obj.m_pixelX, obj.m_pixelY)),
      m_ptrImage(obj.m_ptrImage) {}

/*! \brief 	The isEqual method compares the command with another command
 *object. If the command has the same pixel location and color, the bool value
 *true will be returned. Otherwise false is returned.
 */
bool Draw::IsEqual(const Command &other) {
  const Draw *another = dynamic_cast<const Draw *>(&other);
  return another != nullptr && m_pixelX == another->m_pixelX &&
         m_pixelY == another->m_pixelY && m_color == (another->m_color);
}

/*! \brief 	Return true if the command is successfully executed. Otherwise
 * return false.
 *
 */
bool Draw::Execute() {
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;

  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize) {
    if (m_inverted) {
      (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_prevColor);
    } else {
      (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_color);
    }
    return true;
  }
  return false;
}

/*! \brief 	Return true if the command is successfully executed. Otherwise
 * return false.
 *
 */
bool Draw::Execute(sf::Image &image){
  m_ptrImage = &image;
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;
  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize) {
    if (m_inverted) {
      (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_prevColor);
    } else {
      (*m_ptrImage).setPixel(m_pixelX, m_pixelY, m_color);
    }
    return true;
  }
  return false;
}

/*! \brief 	Return true if the command is successfully undone. Otherwise
 * return false.
 *
 */
bool Draw::Undo(sf::Image &image) {
  m_ptrImage = &image;
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
std::string Draw::ToString() const {
  std::stringstream ss;
  ss << Command::ToString() << ": (" << m_pixelX << "," << m_pixelY << ")"
     << ", sz=" << 1 << "Inverted " << m_inverted;
  return ss.str();
}

/*! \brief Converts the draw into a packet
 * \return Packet that represents the draw command
 */
sf::Packet Draw::Serialize() const {
  std::cout << "Serializing" << std::endl;
  sf::Packet packet;
  if (m_inverted) {
    packet << "Draw" << m_pixelX << m_pixelY << 1 << m_prevColor.r << m_prevColor.g
         << m_prevColor.b << m_prevColor.a << m_color.r << m_color.g
         << m_color.b << m_color.a;
  } else {
    packet << "Draw" << m_pixelX << m_pixelY << 1 << m_color.r << m_color.g
         << m_color.b << m_color.a << m_prevColor.r << m_prevColor.g
         << m_prevColor.b << m_prevColor.a;
  }

  return packet;
}


/*! \brief inverts the command for undoing purposes*/
void Draw::Invert() {
  m_inverted = !m_inverted;
}
