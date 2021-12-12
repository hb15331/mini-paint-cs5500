/**
 *  @file   Draw.cpp
 *  @brief  Draw implementation, all drawing actions are commands.
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/

#include "Draw.hpp"

int m_pixelX, m_pixelY;
sf::Color m_color;
std::vector<std::vector<sf::Color>> m_old_colors;

Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor, sf::Image &image, const int pen_size)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_old_colors(), m_ptrImage(&image), m_inverted(false), m_pen_size(pen_size)
{
  for (int i = -m_pen_size; i <= m_pen_size; ++i)
  {
    // We need to keep track of all the old pixels so that we can revert to them
    std::vector<sf::Color> row;
    for (int j = -m_pen_size; j <= m_pen_size; ++j)
    {
      int x = std::clamp(m_pixelX + i, 0, (int)(*m_ptrImage).getSize().x);
      int y = std::clamp(m_pixelY + j, 0, (int)(*m_ptrImage).getSize().y);
      row.push_back(image.getPixel(x, y));
    }
    // Keep track of the previous pixel color
    m_old_colors.push_back(row);
  }
}

Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor,
           const std::vector<std::vector<sf::Color>> &oldColors, const int pen_size)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_old_colors(oldColors), m_ptrImage(nullptr), m_inverted(false), m_pen_size(pen_size) {}

Draw::Draw(const Draw &obj)
    : Command("Draw"), m_pixelX(obj.m_pixelX), m_pixelY(obj.m_pixelX),
      m_color(obj.m_color),
      m_old_colors(),
      m_ptrImage(obj.m_ptrImage),
      m_pen_size(obj.m_pen_size) {}

/*! \brief 	The isEqual method compares the command with another command
 *object. If the command has the same pixel location and color, the bool value
 *true will be returned. Otherwise false is returned.
 */
bool Draw::IsEqual(const Command &other)
{
  const Draw *another = dynamic_cast<const Draw *>(&other);
  return another != nullptr && m_pixelX == another->m_pixelX &&
         m_pixelY == another->m_pixelY && m_color == (another->m_color) && m_pen_size == another->m_pen_size;
}

/*! \brief 	Return true if the command is successfully executed. Otherwise
 * return false.
 *
 */
bool Draw::Execute()
{
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;

  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize)
  {
    if (m_inverted)
    {
      for (int i = -m_pen_size; i <= m_pen_size; ++i)
      {
        for (int j = -m_pen_size; j <= m_pen_size; ++j)
        {
          int x = std::clamp(m_pixelX + i, 0, (int)(*m_ptrImage).getSize().x);
          int y = std::clamp(m_pixelY + j, 0, (int)(*m_ptrImage).getSize().y);
          (*m_ptrImage).setPixel(x, y, m_old_colors[i + m_pen_size][j + m_pen_size]);
        }
      }
    }
    else
    {
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
bool Draw::Execute(sf::Image &image)
{
  m_ptrImage = &image;
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;
  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize)
  {
    if (m_inverted)
    {
      for (int i = -m_pen_size; i <= m_pen_size; ++i)
      {
        for (int j = -m_pen_size; j <= m_pen_size; ++j)
        {
          int x = std::clamp(m_pixelX + i, 0, (int)(*m_ptrImage).getSize().x);
          int y = std::clamp(m_pixelY + j, 0, (int)(*m_ptrImage).getSize().y);
          (*m_ptrImage).setPixel(x, y, m_old_colors[i + m_pen_size][j + m_pen_size]);
        }
      }
    }
    else
    {
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
bool Draw::Undo(sf::Image &image)
{
  m_ptrImage = &image;
  int xSize = (int)(*m_ptrImage).getSize().x;
  int ySize = (int)(*m_ptrImage).getSize().y;
  if (m_pixelX >= 0 && m_pixelX < xSize && m_pixelY >= 0 && m_pixelY < ySize)
  {
    for (int i = -m_pen_size; i <= m_pen_size; ++i)
      {
        for (int j = -m_pen_size; j <= m_pen_size; ++j)
        {
          int x = std::clamp(m_pixelX + i, 0, (int)(*m_ptrImage).getSize().x);
          int y = std::clamp(m_pixelY + j, 0, (int)(*m_ptrImage).getSize().y);
          (*m_ptrImage).setPixel(x, y, m_old_colors[i + m_pen_size][j + m_pen_size]);
        }
      }
    return true;
  }
  return true;
}

/*! \brief Converts the Draw into a readable string
 * \return String that represents draw location, size and color
 */
std::string Draw::ToString() const
{
  std::stringstream ss;
  ss << Command::ToString() << ": (" << m_pixelX << "," << m_pixelY << ")"
     << ", sz=" << m_pen_size << "Inverted " << m_inverted;
  return ss.str();
}

/*! \brief Converts the draw into a packet
 * \return Packet that represents the draw command
 */
sf::Packet Draw::Serialize() const
{
  std::cout << "Serializing" << std::endl;
  sf::Packet packet;
  if (m_inverted)
  {
    packet << "Draw" << m_pixelX << m_pixelY << 1 << m_prevColor.r << m_prevColor.g
           << m_prevColor.b << m_prevColor.a << m_color.r << m_color.g
           << m_color.b << m_color.a;
  }
  else
  {
    packet << "Draw" << m_pixelX << m_pixelY << 1 << m_color.r << m_color.g
           << m_color.b << m_color.a << m_prevColor.r << m_prevColor.g
           << m_prevColor.b << m_prevColor.a;
  }

  return packet;
}

/*! \brief inverts the command for undoing purposes*/
void Draw::Invert()
{
  m_inverted = !m_inverted;
}
