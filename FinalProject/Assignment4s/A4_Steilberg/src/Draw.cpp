/**
 *  @file   Draw.cpp
 *  @brief  Draw implementation, all drawing actions are commands.
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/

#include "Draw.hpp"

/*! \brief Creates a Draw that will draw a pixel of given size and color
 * to a given location in the given image
 *
 * \param command_description Description of the draw
 * \param canvas Image to draw to
 * \param coordinate Location to draw the pixel
 * \param color Color of the pixel
 * \param pen_size Size of the pixel (pixel is a square pen_size*2 + 1 pixels squared)
 */
Draw::Draw(std::string command_description,
           sf::Image& canvas,
           const sf::Vector2i coordinate,
           const sf::Color color,
           const int pen_size) : Command(command_description),
                                 m_canvas(canvas),
                                 m_old_colors(),
                                 m_coordinate(coordinate),
                                 m_new_color(color),
                                 m_pen_size(pen_size){
    for(int i = -m_pen_size; i <= m_pen_size; ++i) {
        // We need to keep track of all the old pixels so that we can revert to them
        std::vector<sf::Color> row;
        for(int j = -m_pen_size; j <= m_pen_size; ++j) {
            int x = clamp(m_coordinate.x+i, 0, m_canvas.getSize().x);
            int y = clamp(m_coordinate.y+j, 0, m_canvas.getSize().y);
            row.push_back(canvas.getPixel(x, y));
        }
        // Keep track of the previous pixel color
        m_old_colors.push_back(row);
    }
}

/*! \brief Executes the given command, drawing it to the sf::Image.
*
* \return true if the execution succeeded
*/
bool Draw::execute(){
    std::cout  << *this << std::endl;
    for(int i = -m_pen_size; i <= m_pen_size; ++i) {
        for(int j = -m_pen_size; j <= m_pen_size; ++j) {
            int x = clamp(m_coordinate.x+i, 0, m_canvas.getSize().x);
            int y = clamp(m_coordinate.y+j, 0, m_canvas.getSize().y);
            m_canvas.setPixel(x, y, m_new_color);
        }
    }
    return true;
}

/*! \brief Undo the given command.
 * Loops through all pixels set and reverts them to their previous values
 *
 * \return true if the undo succeeded
 */
bool Draw::undo(){
    // This is how we draw pixels of different sizes
    for(int i = -m_pen_size; i <= m_pen_size; ++i) {
        for(int j = -m_pen_size; j <= m_pen_size; ++j) {
            int x = clamp(m_coordinate.x+i, 0, m_canvas.getSize().x);
            int y = clamp(m_coordinate.y+j, 0, m_canvas.getSize().y);
            // Change the pixel back to its old color
            m_canvas.setPixel(x, y, m_old_colors[i+m_pen_size][j+m_pen_size]);
        }
    }
    return true;
}

/*! \brief Checks that this Draw is equal to the given Command
 * \param Command to compare to
 *
 * \return true if the given Command (casted to Draw) is equal to this Draw
 */
bool Draw::isEqual(const Command& other) const {
    // Cast is safe because Command's == overload type checks
    const Draw& other_dr = static_cast<const Draw&>(other);
    // Check important members
    return m_coordinate == other_dr.m_coordinate
        && m_new_color == other_dr.m_new_color
        && m_pen_size == other_dr.m_pen_size;
}

/*! \brief Converts the Draw into a readable string
 * \return String that represents draw location, size and color
 */
std::string Draw::toString() const {
    std::stringstream ss;
    ss << Command::toString() << ": (" << m_coordinate.x << "," << m_coordinate.y << ")" << ", sz=" << m_pen_size;
    return ss.str();
}
