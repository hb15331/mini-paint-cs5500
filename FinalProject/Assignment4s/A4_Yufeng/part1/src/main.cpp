/** 
 *  @file   main.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and Yufeng Gao 
 *  @date   2021-18-10 
 ***********************************************/

// Compile: Use a CMakeLists.txt to generate a build file or you can try compiling with:
// g++ -std=c++17 ./src/*.cpp -I./include/ -o App -lsfml-graphics -lsfml-window -lsfml-system
//
// Note:	If your compiler does not support -std=c++17, 
//		    then try -std=c++14 then -std=c++11.
//		
// HOW TO RUN
//
// ./App

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


/*! \brief 	Call any initailization functions here.
*		This might be for example setting up any
*		global variables, allocating memory,
*		dynamically loading any libraries, or
*		doing nothing at all.
*		
*/
void initialization(void){
	std::cout << "Starting the App" << std::endl;
}

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

 
/*! \brief 	The entry point into our program.
*		
*/
int main(){
	App drawApp;
	// Call any setup function
	// Passing a function pointer into the 'init' function.
	// of our application.
	drawApp.Init(&initialization);
	// Setup your keyboard
	drawApp.UpdateCallback(&update);
	// Setup the Draw Function
	drawApp.DrawCallback(&draw);
	// Call the main loop function
	drawApp.Loop();

	return 0;
}

