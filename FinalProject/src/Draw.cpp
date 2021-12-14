/**
 *  @file   Draw.cpp
 *  @brief  Draw implementation, all drawing actions are commands.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "Draw.hpp"

/*!	\brief Constructor for a Draw object.
 * \return New Draw object.
 */
Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor, sf::Image &image, const int pen_size, bool inverted)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_old_colors(), m_ptrImage(&image), m_pen_size(pen_size), m_inverted(inverted)
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

/*!	\brief Constructor for a Draw object. Used for transmission of commands, as it
 *            does not include a reference to an image.
 * \return New Draw object.
 */
Draw::Draw(int pixelX, int pixelY, const sf::Color &newColor,
           const std::vector<std::vector<sf::Color>> &oldColors, const int pen_size, bool inverted)
    : Command("Draw"), m_pixelX(pixelX), m_pixelY(pixelY), m_color(newColor),
      m_old_colors(oldColors), m_ptrImage(nullptr), m_inverted(inverted), m_pen_size(pen_size) {}

/*!	\brief Constructor for a Draw object.
 * \return New Draw object.
 */
Draw::Draw(const Draw &obj)
    : Command("Draw"), m_pixelX(obj.m_pixelX), m_pixelY(obj.m_pixelX),
      m_color(obj.m_color),
      m_old_colors(),
      m_ptrImage(obj.m_ptrImage),
      m_pen_size(obj.m_pen_size),
      m_inverted(false) {}

/*! \brief 	The isEqual method compares the command with another command
 *object. If the command has the same pixel location and color, the bool value
 *true will be returned. Otherwise false is returned.
 */
bool Draw::IsEqual(const Command &other)
{
  const Draw *another = dynamic_cast<const Draw *>(&other);
  return another != nullptr && m_pixelX == another->m_pixelX && m_inverted == another->m_inverted &&
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
      for (int i = -m_pen_size; i <= m_pen_size; ++i)
      {
        for (int j = -m_pen_size; j <= m_pen_size; ++j)
        {
          int x = std::clamp(m_pixelX + i, 0, (int)(*m_ptrImage).getSize().x);
          int y = std::clamp(m_pixelY + j, 0, (int)(*m_ptrImage).getSize().y);
          (*m_ptrImage).setPixel(x, y, m_color);
        }
      }
    }
    return true;
  }
  return false;
}

/*! \brief 	Executes given command.
 *  \return Return a bool reflecting success or failure of operation.
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
          int x = std::clamp(m_pixelX + i, 0, xSize);
          int y = std::clamp(m_pixelY + j, 0, ySize);
          (*m_ptrImage).setPixel(x, y, m_old_colors[i + m_pen_size][j + m_pen_size]);
        }
      }
    }
    else
    {
      for (int i = -m_pen_size; i <= m_pen_size; ++i)
      {
        for (int j = -m_pen_size; j <= m_pen_size; ++j)
        {
          int x = std::clamp(m_pixelX + i, 0, xSize);
          int y = std::clamp(m_pixelY + j, 0, ySize);
          (*m_ptrImage).setPixel(x, y, m_color);
        }
      }
    }
    return true;
  }
  return false;
}

/*! \brief 	Undoes last operation/command.
 * \return Return a bool value reflecting success or failure of operation.
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
        int x = std::clamp(m_pixelX + i, 0, xSize);
        int y = std::clamp(m_pixelY + j, 0, ySize);
        if (i + m_pen_size >= m_old_colors.size()) {
          continue;
        } else if (j + m_pen_size >= m_old_colors[0].size()) {
          continue;
        }
        (*m_ptrImage).setPixel(x, y, m_old_colors[i + m_pen_size][j + m_pen_size]);
      }
    }
    return true;
  }
  return true;
}

/*! \brief Converts the Draw into a readable string.
 * \return String that represents draw location, size and color.
 */
std::string Draw::ToString() const
{
  std::stringstream ss;
  ss << Command::ToString() << ": (" << m_pixelX << "," << m_pixelY << ")"
     << ", sz=" << m_pen_size << ", Inverted " << m_inverted;
  return ss.str();
}

/*! \brief Converts the draw into a packet.
 * \return Packet that represents the draw command.
 */
sf::Packet Draw::Serialize() const
{
  std::cout << "Serializing " << this->ToString() <<  std::endl;
  sf::Packet packet;

  uint32_t col_size = (uint32_t)m_old_colors.size();
  uint32_t row_size;
  if(col_size > 0) {
    row_size = (unsigned int)m_old_colors.at(0).size();
  } else {
    row_size = 0;
  }

  packet << "Draw" << m_pixelX << m_pixelY << m_pen_size
    << m_inverted << m_color.toInteger()
    << col_size << row_size;
  for(auto row : m_old_colors) {
    for (auto color: row) {
      packet << color.toInteger();
    }
  }


  return packet;
}

/*! \brief inverts the command for undoing purposes*/
void Draw::Invert()
{
  m_inverted = !m_inverted;
}
