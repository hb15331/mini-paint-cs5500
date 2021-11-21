/** 
 *  @file   main.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and Alex Thrush
 *  @date   2021-15-11
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
#include "../include/App.hpp"
#include "../include/Command.hpp"
#include "../include/Draw.hpp"

/*! \brief 	The entry point into our program.
*		
*/

int main(){
	App app;
	app.loop();
	//App app;
	return 0;
}