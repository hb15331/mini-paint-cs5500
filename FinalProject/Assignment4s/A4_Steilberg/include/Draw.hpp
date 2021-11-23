/** 
 *  @file   Draw.hpp 
 *  @brief  Drawing actions interface. 
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Color.hpp>

// Include standard library C++ libraries.
#include <string>
#include <vector>
#include <iostream>
#include <sstream>

// Project header files
#include "Command.hpp"
#include "App.hpp"

/*! \brief Command to draw a pixel of a given size and color at a given location */
class Draw : public Command{
    public:
    // Create a draw object
    explicit Draw(std::string command_description,
                  sf::Image& canvas,
                  const sf::Vector2i coordinate,
                  const sf::Color color,
                  const int pen_size);

    // Overriding previous execute and undo functions
    bool execute() override;
    bool undo() override;

    // Overriding toString
    std::string toString() const override;

    private:
    // Check equality given safe typecast from base class
    virtual bool isEqual(const Command& other) const;
    // Image that we draw 
    sf::Image& m_canvas;
    // Grid of old colors to use for undoing
    std::vector<std::vector<sf::Color>> m_old_colors;
    // Coordinate for the center of the pixel
    const sf::Vector2i m_coordinate;
    // Color of the pixel to be drawn
    const sf::Color m_new_color;
    // Size of the pixel
    const int m_pen_size;
};

/*! \brief Override to allow simple printing of a Draw object */
inline std::ostream& operator<<(std::ostream &strm, const Draw& dr) {
  return strm << dr.toString();
}

#endif
