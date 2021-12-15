/**
 *  @file   ClearCanvas.hpp
 *  @brief  Command to set entire canvas to one color
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef CLEARCANVAS_HPP
#define CLEARCANVAS_HPP

#include "Command.hpp"
#include "Draw.hpp"

#include <vector>
#include <memory>


/*! \brief Represents a canvas clearing operation */
class ClearCanvas : public Command
{
  public:
    // Constructors
    ClearCanvas(const sf::Color &clearColor, sf::Image &image, bool inverted=false);
    ClearCanvas(const sf::Color &clearColor, std::vector<std::shared_ptr<Draw>> components, bool inverted=false);

    // Check equality with other command
    bool IsEqual(const Command &other) override;

    // Execute the canvas clearing
    bool Execute(sf::Image &image) override;

    // Unclear the canvas
    void Invert() override;

    // Convert the ClearCanvas into a string, usually for debuggin
    std::string ToString() const override;

    // Undo the ClearCanvas
    bool Undo(sf::Image &image) override;

    // Turn the ClearCanvas into a series of packets
    std::vector<sf::Packet> Serialize() const override;

    // Always false, ClearCanvas is not a component
    bool IsComponent() const override;

    // Check equality with another ClearCanvas
    ClearCanvas &operator=(const ClearCanvas &) = delete;

  private:

    // Color to clear the canvas to
    sf::Color m_color;

    // Component draws used for clearing
    std::vector<std::shared_ptr<Draw>> m_components;

    // True if we are unclearing the canvas
    bool m_inverted;

    // pen_size for component draws
    const size_t M_CHUNK_SIZE = 100;
};

#endif // CLEARCANVAS_HPP

/*! \brief Override to allow simple printing of a ClearCanvas object
  \param strm Stream to add text to
  \param dr ClearCanvas to turn into a string and add to stream
*/
inline std::ostream &operator<<(std::ostream &strm, const ClearCanvas &dr) {
  return strm << dr.ToString();
}
