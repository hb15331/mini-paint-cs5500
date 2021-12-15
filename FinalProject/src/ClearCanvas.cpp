#include "ClearCanvas.hpp"

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

/*! \brief Inverts the clearcanvas
 *
 */
void ClearCanvas::Invert() {
  m_inverted = !m_inverted;
  for (auto draw : m_components) {
    draw->Invert();
  }
}

std::string ClearCanvas::ToString() const {
  std::stringstream ss;
  ss << Command::ToString() << ": chunk_size " << M_CHUNK_SIZE
     << ", color: " << m_color.toInteger() << ", inverted: " << m_inverted;
  return ss.str();
}

bool ClearCanvas::Undo(sf::Image &image) {
  for (std::shared_ptr<Draw> component : m_components) {
    if (!(component->Undo(image))) {
      return false;
    }
  }
  return true;
}

bool ClearCanvas::IsComponent() const { return false; }

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
