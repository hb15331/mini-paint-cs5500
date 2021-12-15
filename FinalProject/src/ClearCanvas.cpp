/**
 *  @file   ClearCanvas.cpp
 *  @brief  Implementation of Command.hpp
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/


#include "ClearCanvas.hpp"

/*! \brief Creates a new ClearCanvas using given image to obtain previous colors
 * \param clearColor the color to clear to
 * \param image The image to use for obtaining old colors
 * \param inverted Runs the command in reverse (un-clears)
 */
ClearCanvas::ClearCanvas(const sf::Color &clearColor, sf::Image &image,
                         bool inverted)
    : Command("ClearCanvas"), m_color(clearColor), m_components(),
      m_inverted(inverted) {
  uint32_t width = image.getSize().x;
  uint32_t height = image.getSize().y;
  size_t half_chunk = M_CHUNK_SIZE / 2;

  for (size_t row = 0 + half_chunk; row <= width - half_chunk;
       row += M_CHUNK_SIZE) {
    for (size_t col = 0 + half_chunk; col <= height - half_chunk;
         col += M_CHUNK_SIZE) {
      m_components.push_back(std::make_shared<Draw>(row, col, m_color, image,
                                                    M_CHUNK_SIZE / 2, false));
    }
  }
}

/*! \brief Creates a new ClearCanvas using given image to obtain previous colors
 * \param clearColor the color to clear to
 * \param components Directly provide the old colors rather than finding them with the image
 * \param inverted Runs the command in reverse (un-clears)
 */
ClearCanvas::ClearCanvas(const sf::Color &clearColor,
                         std::vector<std::shared_ptr<Draw>> components,
                         bool inverted)
    : Command("ClearCanvas"), m_color(clearColor), m_components(components),
      m_inverted(inverted) {}

bool ClearCanvas::IsEqual(const Command &other) {
  const ClearCanvas *another = dynamic_cast<const ClearCanvas *>(&other);
  if (another == nullptr ||
      m_components.size() != another->m_components.size() ||
      m_inverted != another->m_inverted || m_color != another->m_color) {
    return false;
  }
  return true;
}

/*! \brief Clears the canvas given in image
 * \param image The image to clear
 * \return true if successful, false otherwise
 */
bool ClearCanvas::Execute(sf::Image &image) {
  for (std::shared_ptr<Draw> component : m_components) {
    if (!component->Execute(image)) {
      return false;
    }
  }
  return true;
}

/*! \brief Inverts the clearcanvas, making it unclear the canvas when executed
 */
void ClearCanvas::Invert() {
  m_inverted = !m_inverted;
  for (auto draw : m_components) {
    // Invert components
    draw->Invert();
  }
}

/*! \brief Returns a string representing the command
 *  \return string representing the command
 */
std::string ClearCanvas::ToString() const {
  std::stringstream ss;
  ss << Command::ToString() << ": chunk_size " << M_CHUNK_SIZE
     << ", color: " << m_color.toInteger() << ", inverted: " << m_inverted;
  return ss.str();
}

/*! \brief Undoes the clearcanvas command (used for offline drawing)
 * \param image Image used for drawing the undone commands
 * \return true if successful, false otherwise
 */
bool ClearCanvas::Undo(sf::Image &image) {
  for (std::shared_ptr<Draw> component : m_components) {
    if (!(component->Undo(image))) {
      return false;
    }
  }
  return true;
}

/*! \brief Returns false because the ClearCanvas is not itself ever part of a larger command
 * \return false
 */
bool ClearCanvas::IsComponent() const { return false; }

/*! \brief Turns the ClearCanvas into several packets (which are themselves IsComponent()==true)
 *  \returns a vector of packets representing the ClearCanvas as several Draws
 */
std::vector<sf::Packet> ClearCanvas::Serialize() const {
  std::vector<sf::Packet> vec;
  for (auto component : m_components) {
    sf::Packet pack_component;
    pack_component << "ClearComponent";
    sf::Packet part = component->Serialize()[0];
    pack_component.append(part.getData(), part.getDataSize());
    vec.push_back(pack_component);
  }
  return vec;
}
