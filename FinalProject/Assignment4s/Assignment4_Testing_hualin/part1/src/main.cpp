/** 
 *  @file   main.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and ???? 
 *  @date   yyyy-dd-mm 
 ***********************************************/

// Compile: Use a CMakeLists.txt or you can try compiling with:
// g++ -std=c++17 ./src/*.cpp -I./include/ -o App -lsfml-graphics -lsfml-window -lsfml-system
//
// Note:	If your compiler does not support -std=c++17, 
//		then try -std=c++14 then -std=c++11.
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
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"


// instantiate the non-singleton App class
//App myApp = App();


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

/*! \brief 	The update function presented can be simplified.
*		I have demonstrated two ways you can handle events,
*		if for example we want to add in an event loop.
*		
*/
void update(App& myApp){
	// Update our canvas
	sf::Event event;
	while(myApp.GetWindow().pollEvent(event)){
		if(event.type == sf::Event::MouseMoved){				
			// Modify the pixel
			myApp.mouseX = event.mouseMove.x; 
			myApp.mouseY = event.mouseMove.y; 
			//myApp.GetImage().setPixel(myApp.mouseX,myApp.mouseY,sf::Color::Blue);
		}
	}

	// We can otherwise handle events normally
	if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
		sf::Vector2i coordinate = sf::Mouse::getPosition(myApp.GetWindow());
		std::cout << "Hmm, lots of repeats here: " << coordinate.x << "," << coordinate.y << std::endl;
		// Modify the pixel
		//myApp.GetImage().setPixel(coordinate.x,coordinate.y,sf::Color::Red);
		std::string msg("drawing");
		Command* drawCommand = new Draw(msg);
		myApp.AddCommand(drawCommand);
	
	}
	
	// execute draw commands from command queue
	myApp.ExecuteCommand();

	// Capture any keys that are released
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
		exit(EXIT_SUCCESS);
	}

	// handle the undo key input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		myApp.undo_command();
	}

	// handle the redo key input
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Y)) {
		myApp.redo_command();
	}

	// Where was the mouse previously before going to the next frame
	myApp.pmouseX = myApp.mouseX;
	myApp.pmouseY = myApp.mouseY;
}


/*! \brief 	The draw call 
*		
*/
void draw(App& myApp){
	// Static variable 
	static int refreshRate = 0;
	++refreshRate;	// Increment 


	// We load into our texture the modified pixels	
	// But we only do so every 10 draw calls to reduce latency of transfer
	// between the GPU and CPU.
	// Ask yourself: Could we do better with sf::Clock and refresh once
	// 	 	 every 'x' frames? 
	if(refreshRate>10){
		myApp.GetTexture().loadFromImage(myApp.GetImage());	
		refreshRate =0;
	}
}

 
/*! \brief 	The entry point into our program.
*		
*/
int main(){

	//App myApp = App();
	App myApp;

	// Call any setup function
	// Passing a function pointer into the 'init' function.
	// of our application.
	myApp.Init(&initialization);
	// Setup your keyboard
	myApp.UpdateCallback(&update);
	// Setup the Draw Function
	myApp.DrawCallback(&draw);
	// Call the main loop function
	myApp.Loop();
	// Destroy our app
	//myApp.Destroy();

	return 0;
}

