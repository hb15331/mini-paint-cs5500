/** 
 *  @file   App.cpp 
 *  @brief  Main class for program
 *  @author Mike and ???? 
 *  @date   yyyy-dd-mm 
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <cassert>
#include <iostream>
// Project header files
//#include "App.hpp"
#include "../include/App.hpp"

// All static members of a Singleton need to initialized to some value.
// This is so that when the program is compiled, memory is reserved
// for each of these static values.
// Potentially handy data structures for building an undo system.
// TODO: Decide, modify, add or remove any data structures you need.

void App::clearUndo(){
	while(undo_stack.empty() == false){
		undo_stack.pop();
	}
}

void App::clearRedo(){
	while(redo_stack.empty() == false){
		redo_stack.pop();
	}
}

void App::paintPixel(pixel pixel){
	image.setPixel(pixel.x,pixel.y,pixel.color);
	texture.loadFromImage(image);
}

void App::initializeCanvas(){
	window.setVerticalSyncEnabled(true);
	image.create(600,400, sf::Color::White);
	texture.loadFromImage(image);
	sprite.setTexture(texture);
}

void App::close(){
	if(window.isOpen()){
		window.close();
	}
	exit(EXIT_SUCCESS);
}

void App::loop(){
	sf::RenderWindow window(sf::VideoMode(600,400), "Assignment 4: Testing",sf::Style::Titlebar);
	initializeCanvas();
	while(window.isOpen()){	
		sf::Event event;
		while(window.pollEvent(event)){
			if(event.type == sf::Event::Closed){
				//window.close();
				//exit(EXIT_SUCCESS);
				close();
			}

			//Managing key presses.
			if(event.type == sf::Event::KeyReleased){
				if(event.key.code == sf::Keyboard::Escape){
					std::cout << "Escape Pressed" << std::endl;
					window.close();
					exit(EXIT_SUCCESS);
				}
				if(event.key.code == sf::Keyboard::Z){
					std::cout << "Undo key pressed" <<std::endl;
					if (undo_stack.empty() == false) {
						//Take pixel from undo stack.
						pixel undoPixel = undo_stack.top();
						undo_stack.pop();
						//Initialize redo pixel, same coordinates as undo, but with current color.
						pixel redoPixel;
						redoPixel.x = undoPixel.x;
						redoPixel.y = undoPixel.y;
						//Capture current color.
						redoPixel.color = image.getPixel(redoPixel.x, redoPixel.y);
						redo_stack.push(redoPixel);
						//Paint undo pixel.
						// image.setPixel(undoPixel.x,undoPixel.y,undoPixel.color);
						// texture.loadFromImage(image);	
						paintPixel(undoPixel);

						//Set last action as undo.
						last_action = undo;				
						std::cout << "Pixel undone." << std::endl;
					}
					else {
						std::cout << "Undo stack is empty." << std::endl;
					}
				}
				if(event.key.code == sf::Keyboard::Y){
					std::cout << "Redo key pressed" <<std::endl;
					if (redo_stack.empty() == false) {
						//Clear undo stack if last action before redo was undo.
						if (last_action == undo){
							clearUndo();
						}
						pixel redoPixel;
						redoPixel = redo_stack.top();
						redo_stack.pop();
						// image.setPixel(redoPixel.x,redoPixel.y,redoPixel.color);
						// texture.loadFromImage(image);
						paintPixel(redoPixel);
						//Set last action as redo.
						last_action = redo;	
						std::cout << "Pixel redone." << std::endl;
					}
					else {
						std::cout << "Redo stack is empty." << std::endl;
					}
				}
			}
		}

		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			sf::Vector2i coords = sf::Mouse::getPosition(window);
			//Make sure drawing only happens on the canvas.
			if (coords.x <= 600 and coords.x >= 0 and coords.y <= 400 and coords.y >= 0){
				//Clear redo stack if last action was redo.
				if (last_action == redo or last_action == undo){
					clearUndo();
					clearRedo();
				}
				pixel oldPixel;
				pixel drawPixel;
				//Get current pixel location and color.
				oldPixel.x = coords.x;
				oldPixel.y = coords.y;
				oldPixel.color = image.getPixel(coords.x, coords.y);
				drawPixel.x = coords.x;
				drawPixel.y = coords.y;
				drawPixel.color == sf::Color::Black;
				undo_stack.push(oldPixel);
				//Paint pixel.
				// image.setPixel(coords.x,coords.y,sf::Color::Black);
				// texture.loadFromImage(image);	
				paintPixel(drawPixel);
				//Set last action as draw.
				last_action = draw1;	
				
			}else{
				std::cout << "Draw unsuccessful, out of bounds." <<std::endl;
				std::cout << "X: " << coords.x << " Y: " << coords.y <<std::endl;
			}
			
		}

		window.clear();
		window.draw(sprite);
		window.display();
	}
}

App::App(){
	
}

// App::App(){
// 	sf::RenderWindow window(sf::VideoMode(600,400), "Assignment 4: Testing",sf::Style::Titlebar);

// 	//Initialize canvas.
// 	initializeCanvas();
// 	while(window.isOpen()){		
// 		sf::Event event;
// 		while(window.pollEvent(event)){
// 			if(event.type == sf::Event::Closed){
// 				//window.close();
// 				//exit(EXIT_SUCCESS);
// 				close();
// 			}

// 			//Managing key presses.
// 			if(event.type == sf::Event::KeyReleased){
// 				if(event.key.code == sf::Keyboard::Escape){
// 					std::cout << "Escape Pressed" << std::endl;
// 					window.close();
// 					exit(EXIT_SUCCESS);
// 				}
// 				if(event.key.code == sf::Keyboard::Z){
// 					std::cout << "Undo key pressed" <<std::endl;
// 					if (undo_stack.empty() == false) {
// 						//Take pixel from undo stack.
// 						pixel undoPixel = undo_stack.top();
// 						undo_stack.pop();
// 						//Initialize redo pixel, same coordinates as undo, but with current color.
// 						pixel redoPixel;
// 						redoPixel.x = undoPixel.x;
// 						redoPixel.y = undoPixel.y;
// 						//Capture current color.
// 						redoPixel.color = image.getPixel(redoPixel.x, redoPixel.y);
// 						redo_stack.push(redoPixel);
// 						//Paint undo pixel.
// 						// image.setPixel(undoPixel.x,undoPixel.y,undoPixel.color);
// 						// texture.loadFromImage(image);	
// 						paintPixel(undoPixel);

// 						//Set last action as undo.
// 						last_action = undo;				
// 						std::cout << "Pixel undone." << std::endl;
// 					}
// 					else {
// 						std::cout << "Undo stack is empty." << std::endl;
// 					}
// 				}
// 				if(event.key.code == sf::Keyboard::Y){
// 					std::cout << "Redo key pressed" <<std::endl;
// 					if (redo_stack.empty() == false) {
// 						//Clear undo stack if last action before redo was undo.
// 						if (last_action == undo){
// 							clearUndo();
// 						}
// 						pixel redoPixel;
// 						redoPixel = redo_stack.top();
// 						redo_stack.pop();
// 						// image.setPixel(redoPixel.x,redoPixel.y,redoPixel.color);
// 						// texture.loadFromImage(image);
// 						paintPixel(redoPixel);
// 						//Set last action as redo.
// 						last_action = redo;	
// 						std::cout << "Pixel redone." << std::endl;
// 					}
// 					else {
// 						std::cout << "Redo stack is empty." << std::endl;
// 					}
// 				}
// 			}
// 		}

// 		if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
// 			sf::Vector2i coords = sf::Mouse::getPosition(window);
// 			//Make sure drawing only happens on the canvas.
// 			if (coords.x <= 600 and coords.x >= 0 and coords.y <= 400 and coords.y >= 0){
// 				//Clear redo stack if last action was redo.
// 				if (last_action == redo or last_action == undo){
// 					clearUndo();
// 					clearRedo();
// 				}
// 				pixel oldPixel;
// 				pixel drawPixel;
// 				//Get current pixel location and color.
// 				oldPixel.x = coords.x;
// 				oldPixel.y = coords.y;
// 				oldPixel.color = image.getPixel(coords.x, coords.y);
// 				drawPixel.x = coords.x;
// 				drawPixel.y = coords.y;
// 				drawPixel.color == sf::Color::Black;
// 				undo_stack.push(oldPixel);
// 				//Paint pixel.
// 				// image.setPixel(coords.x,coords.y,sf::Color::Black);
// 				// texture.loadFromImage(image);	
// 				paintPixel(drawPixel);
// 				//Set last action as draw.
// 				last_action = draw1;	
				
// 			}else{
// 				std::cout << "Draw unsuccessful, out of bounds." <<std::endl;
// 				std::cout << "X: " << coords.x << " Y: " << coords.y <<std::endl;
// 			}
			
// 		}

// 		window.clear();
// 		window.draw(sprite);
// 		window.display();
// 	}

// }


