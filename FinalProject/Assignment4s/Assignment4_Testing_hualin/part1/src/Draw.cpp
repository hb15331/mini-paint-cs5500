/** 
 *  @file   Draw.cpp 
 *  @brief  Draw implementation, all drawing actions are commands. 
 *  @author Mike and ???? 
 *  @date   yyyy-dd-mm 
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics/Color.hpp>
// Include standard library C++ libraries.
// #include ...
// Project header files
#include "App.hpp"
#include "Draw.hpp"


/*! \brief      N/A
*               
*/
Draw::Draw(std::string commandDescription) : Command(commandDescription){
}


/*! \brief 	N/A
*		
*/
bool Draw::execute(sf::Image* m_image, int mouseX, int mouseY){
	Draw::mouse_x = mouseX;
	Draw::mouse_y = mouseY;
	m_image -> setPixel(mouse_x, mouse_y, sf::Color::Red);
	return true;
}

/*! \brief 	N/A
*		
*/
bool Draw::undo(sf::Image* m_image){
	m_image -> setPixel(mouse_x, mouse_y, sf::Color::White);
	return true;
}


/*! \brief      N/A
*
*/
bool Draw::redo(sf::Image* m_image){
	m_image -> setPixel(mouse_x, mouse_y, sf::Color::Red);
	return true;
}


/*! \brief      N/A
*               
*/
bool Draw::isEqual(const Command& other){
	const Draw* otherPtr = dynamic_cast<const Draw*> (&other);
	bool status = (otherPtr != nullptr && 
			otherPtr -> mouse_x == mouse_x && 
			otherPtr -> mouse_y == mouse_y);
	return status;
}

