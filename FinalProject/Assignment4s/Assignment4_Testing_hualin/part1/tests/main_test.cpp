/** 
 *  @file   main_test.cpp 
 *  @brief  Unit Tests for our program
 *  @author Hualin 
 *  @date   yyyy-dd-mm 
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
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"


void initialization(void){
	std::cout << "Starting the App" << std::endl;
}

/*! \brief 	Initialize and destroy the program
*
*/
//TEST_CASE("init and destroy"){
//	App myApp = App();
//	myApp.Init(&initialization);
//}


/*! \brief      Test 1: drawing command at position (100, 100)
*
*/
TEST_CASE("draw at (100, 100)"){

	App myApp = App();
	myApp.Init(&initialization);
	sf::Image& m_image = myApp.GetImage();
	// drawing the pixel
	Draw drawCommand = Draw("drawing");
	drawCommand.execute(&m_image, 100, 100);

	REQUIRE(m_image.getPixel(100, 100) == sf::Color::Red);

}


/*! \brief      Test 2: undo command at position (100, 100)
*
*/
TEST_CASE("undo the drawing at (100, 100)"){

	App myApp = App();
	myApp.Init(&initialization);
	sf::Image& m_image = myApp.GetImage();
	// drawing the pixel
	Draw drawCommand = Draw("drawing");
	drawCommand.execute(&m_image, 100, 100);
	// undo the drawing
	drawCommand.undo(&m_image);

	REQUIRE(m_image.getPixel(100, 100) == sf::Color::White);
	
}


/*! \brief      Test 3: redo command at position (100, 100)
*
*/
TEST_CASE("redo the drawing at (100, 100)"){

	App myApp = App();
        myApp.Init(&initialization);
        sf::Image& m_image = myApp.GetImage();
        // drawing the pixel
        Draw drawCommand = Draw("drawing");
        drawCommand.execute(&m_image, 100, 100);
        // undo the drawing
        drawCommand.undo(&m_image);
	// redo the drawing
	drawCommand.redo(&m_image);
	
	REQUIRE(m_image.getPixel(100, 100) == sf::Color::Red);

}


/*! \brief      Test 4: check if two commands are equal
*
*/
TEST_CASE("compare that two draw commands are equal"){
	App myApp = App();
        myApp.Init(&initialization);
        sf::Image& m_image = myApp.GetImage();

	Draw drawCommand1 = Draw("drawing1");
	drawCommand1.execute(&m_image, 100, 100);
	Draw drawCommand2 = Draw("drawing2");
	drawCommand2.execute(&m_image, 100, 100);

	REQUIRE(drawCommand1.isEqual(drawCommand2) == true);

}
