/** 
 *  @file   main_test.cpp 
 *  @brief  Unit Tests for our program
 *  @author Mike and Yufeng
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
#include <memory>
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"

std::string testLog;

void mock_initialization(void){
	testLog.append("App initialized.");
}

//update and draw method is copied straight from the main method.
//This doesn't seem ideal for later refactoring.
//for now, the logic within update and recall won't be tested.
//instead, the tests will be targeted towards an App object.

/*! \brief 	The update function for app to handle events.
* 		This includes mouse and key events for draw, undo, redo
*		and exit.
*		
*/
void update(App& appObject){
	// Update our canvas
	sf::Event event;
	while(appObject.GetWindow().pollEvent(event)){
		if(event.type == sf::Event::KeyPressed){				
			if(event.key.code==sf::Keyboard::Y) {
				appObject.Redo();
			}
			else if(event.key.code==sf::Keyboard::Z) {
				appObject.Undo();
			}
		}
	}
	// We can otherwise handle events normally
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		sf::Vector2i coordinate = sf::Mouse::getPosition(appObject.GetWindow());
		sf::Image image = appObject.GetImage();
		//assuming the image size does not reach INT_MAX
		int xSize = (int) image.getSize().x;
		int ySize = (int) (image.getSize().y);
		if (coordinate.x >= 0 && coordinate.x < xSize && coordinate.y >=0 && coordinate.y < ySize) {
			appObject.AddCommand(std::make_shared<Draw>(coordinate.x, coordinate.y, sf::Color::Red, appObject.GetImage()));
			appObject.ExecuteCommand();
		}
	}
	// Capture any keys that are released
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}
	
	// Where was the mouse previously before going to the next frame
	appObject.pmouseX = appObject.mouseX;
	appObject.pmouseY = appObject.mouseY;
}


/*! \brief 	The draw call 
*		
*/
void draw(App& appObject){
	// Static variable 
	static int refreshRate = 0;
	++refreshRate;	// Increment 


	// We load into our texture the modified pixels	
	// But we only do so every 10 draw calls to reduce latency of transfer
	// between the GPU and CPU.
	// Ask yourself: Could we do better with sf::Clock and refresh once
	// 	 	 every 'x' frames? 
	if(refreshRate>10){
		appObject.GetTexture().loadFromImage(appObject.GetImage());	
		refreshRate =0;
	}
}

void mock_update(App& appObject) {
    testLog.append("Mock update has been called.");
    appObject.GetWindow().close();
}

void mock_draw(App& appObject) {
    testLog.append("Mock draw has been called.");
    appObject.GetWindow().close();
}

TEST_CASE("init and mock update") {
    App testApp;
    testLog = "";
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&mock_update);
    testApp.DrawCallback(&draw);
    testApp.Loop();
    REQUIRE(testLog == "App initialized.Mock update has been called.");
}

TEST_CASE("init and mock draw") {
    App testApp;
    testLog = "";
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&update);
    testApp.DrawCallback(&mock_draw);
    testApp.Loop();
    REQUIRE(testLog == "App initialized.Mock draw has been called.");
}

TEST_CASE("test draw command") {
    App testApp;
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&mock_update);
    testApp.DrawCallback(&mock_draw);
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.AddCommand(std::make_shared<Draw>(0, 0, sf::Color::Red, testApp.GetImage()));
    testApp.ExecuteCommand();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Red);
}

// invalid draw command location fails
// because draw constructor requires previous color
// TEST_CASE("test invalid draw command") {
//     App testApp;
//     testApp.Init(&mock_initialization);
//     testApp.UpdateCallback(&mock_update);
//     testApp.DrawCallback(&mock_draw);
//     REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
//     Draw* command = new Draw(-1, -1, sf::Color::Red, testApp.GetImage());
//     testApp.AddCommand(command);
//     testApp.ExecuteCommand();
    //make sure the entirely white image stays unmodified
    // int xSize = testApp.GetImage().getSize().x;
    // int ySize = testApp.GetImage().getSize().y;
    // for (unsigned int pixelX = 0; pixelX < xSize; pixelX++) {
    //     for (unsigned int pixelY = 0; pixelY < ySize; pixelY++) {
    //         REQUIRE(testApp.GetImage().getPixel(pixelX,pixelY)==sf::Color::White);
    //     }
    // }
// }

TEST_CASE("test undo on single valid command") {
    App testApp;
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&mock_update);
    testApp.DrawCallback(&mock_draw);
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.AddCommand(std::make_shared<Draw>(0, 0, sf::Color::Red, testApp.GetImage()));
    testApp.ExecuteCommand();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Red);
    testApp.Undo();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
}

TEST_CASE("test redo on single valid command") {
    App testApp;
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&mock_update);
    testApp.DrawCallback(&mock_draw);
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.AddCommand(std::make_shared<Draw>(0, 0, sf::Color::Red, testApp.GetImage()));
    testApp.ExecuteCommand();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Red);
    testApp.Undo();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.Redo();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Red);
}

TEST_CASE("test invalid redo after executing a new command") {
    App testApp;
    testApp.Init(&mock_initialization);
    testApp.UpdateCallback(&mock_update);
    testApp.DrawCallback(&mock_draw);
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.AddCommand(std::make_shared<Draw>(0, 0, sf::Color::Red, testApp.GetImage()));
    testApp.ExecuteCommand();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Red);
    //undo command1
    testApp.Undo();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::White);
    testApp.AddCommand(std::make_shared<Draw>(0, 0, sf::Color::Blue, testApp.GetImage()));
    testApp.ExecuteCommand();
    //Call redo when there are no valid redo commands
    //after setting pixel at (0,0) to blue using new command
    testApp.Redo();
    REQUIRE(testApp.GetImage().getPixel(0,0)==sf::Color::Blue);
}


