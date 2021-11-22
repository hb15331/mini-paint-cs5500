/** 
 *  @file   Draw.hpp 
 *  @brief  Drawing actions interface. 
 *  @author Mike and ???? 
 *  @date   yyyy-dd-mm 
 ***********************************************/
#ifndef DRAW_H
#define DRAW_H

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "Command.hpp"

// Anytime we want to implement a new command in our paint tool,
// we have to inherit from the command class.
// This forces us to implement an 'execute' and 'undo' command.
// 
// In our Draw class note that we have also explicitly marked the
// functions as 'override' to indicate that these functions are
// overriding the behavior of functions in a base class 
// that are implemented as virtual.
class Draw : public Command{
	private: 
		int mouse_x;
		int mouse_y;
	public:
		Draw(std::string commandDescription);
		bool execute(sf::Image* m_image, int mouseX, int mouseY) override;
		bool undo(sf::Image* m_image) override;
		bool redo(sf::Image* m_image) override;
		bool isEqual(const Command& other) override;
};

#endif
