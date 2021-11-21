/** 
 *  @file   main_test.cpp 
 *  @brief  Unit Tests for our program
 *  @author Mike and Alex Thrush
 *  @date   2021-17-11
 ***********************************************/

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <string>
// Project header files
#include "../include/App.hpp"
#include "Command.hpp"
#include "Draw.hpp"


void initialization(void){
	std::cout << "Starting the App" << std::endl;
}
 
/*! \brief 	Confirm redo stack is emptied.
*		
*/
TEST_CASE("Empty redo stack"){
	App app;	
	pixel test_pixel;
	test_pixel.color = sf::Color::Black;
	test_pixel.x = 100;
	test_pixel.y = 100;
	for (int i = 0; i < 5; i++) {
  		app.redo_stack.push(test_pixel);
	}
	app.clearRedo();
	REQUIRE( app.undo_stack.empty() == true );
}

/*! \brief  Confirm undo stack is emptied.
*		
*/
TEST_CASE("Empty undo stack"){
	App app;	
	pixel test_pixel;
	test_pixel.color = sf::Color::Black;
	test_pixel.x = 100;
	test_pixel.y = 100;
	for (int i = 0; i < 5; i++) {
  		app.undo_stack.push(test_pixel);
	}
	app.clearUndo();
	REQUIRE( app.redo_stack.empty() == true );
}

/*! \brief  Confirm app closes.
*		
*/
TEST_CASE("Close app"){
	App app;	
	app.initializeCanvas();
	//app.close();
	app.window.close();
	REQUIRE( !app.window.isOpen() );
}

/*! \brief  Paint pixel.
*		
*/
TEST_CASE("Paint pixel"){
	sf::Color previous_color;
	sf::Color current_color;
	App app;	
	app.initializeCanvas();
	pixel pixel;
	pixel.x = 10;
	pixel.y = 10;
	previous_color = app.image.getPixel(pixel.x, pixel.y);
	pixel.color = sf::Color::Black;
	app.paintPixel(pixel);
	current_color = app.image.getPixel(pixel.x, pixel.y);
	REQUIRE( previous_color == sf::Color::White );
}