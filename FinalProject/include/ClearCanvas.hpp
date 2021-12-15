#ifndef CLEARCANVAS_HPP
#define CLEARCANVAS_HPP

#include "Command.hpp"
#include "Draw.hpp"

#include <vector>
#include <memory>


class ClearCanvas : public Command
{
  public:
    ClearCanvas(const sf::Color &clearColor, sf::Image &image, bool inverted=false);
    ClearCanvas(const sf::Color &clearColor, std::vector<std::shared_ptr<Draw>> components, bool inverted=false);

    bool IsEqual(const Command &other) override;
    bool Execute(sf::Image &image) override;
    void Invert() override;
    std::string ToString() const override;
    bool Undo(sf::Image &image) override;
    std::vector<sf::Packet> Serialize() const override;
    bool IsComponent() const override;


    ClearCanvas &operator=(const ClearCanvas &) = delete;

  protected:

  private:
    sf::Color m_color;
    std::vector<std::shared_ptr<Draw>> m_components;
    bool m_inverted;
    const size_t M_CHUNK_SIZE = 100;
};

#endif // CLEARCANVAS_HPP

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream &operator<<(std::ostream &strm, const ClearCanvas &dr) {
  return strm << dr.ToString();
}
