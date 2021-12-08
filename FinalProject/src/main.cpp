/**
 *  @file   main.cpp
 *  @brief  Entry point into the program.
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/

// Compile: Use a CMakeLists.txt to generate a build file or you can try
// compiling with: g++ -std=c++17 ./src/*.cpp -I./include/ -o App
// -lsfml-graphics -lsfml-window -lsfml-system
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
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
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
void initialization(App &appObject) {
  appObject.CreateUDPNetworkClient();
  std::cout << "Starting the App" << std::endl;
}

/*! \brief 	The update function for app to handle events.
 * 		This includes mouse and key events for draw, undo, redo
 *		and exit.
 *
 */
void update(App &appObject) {
  // Update our canvas
  sf::Event event;
  while (appObject.GetWindow().pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Y) {
        appObject.Redo();
      } else if (event.key.code == sf::Keyboard::Z) {
        appObject.Undo();
      }
      if (event.key.code == sf::Keyboard::Num1 ||
          event.key.code == sf::Keyboard::Numpad1) {
        appObject.SetSelectedColor(sf::Color::Black);
      }
      if (event.key.code == sf::Keyboard::Num2 ||
          event.key.code == sf::Keyboard::Numpad2) {
        appObject.SetSelectedColor(sf::Color::White);
      }
      if (event.key.code == sf::Keyboard::Num3 ||
          event.key.code == sf::Keyboard::Numpad3) {
        appObject.SetSelectedColor(sf::Color::Red);
      }
      if (event.key.code == sf::Keyboard::Num4 ||
          event.key.code == sf::Keyboard::Numpad4) {
        appObject.SetSelectedColor(sf::Color::Green);
      }
      if (event.key.code == sf::Keyboard::Num5 ||
          event.key.code == sf::Keyboard::Numpad5) {
        appObject.SetSelectedColor(sf::Color::Blue);
      }
      if (event.key.code == sf::Keyboard::Num6 ||
          event.key.code == sf::Keyboard::Numpad6) {
        appObject.SetSelectedColor(sf::Color::Yellow);
      }
      if (event.key.code == sf::Keyboard::Num7 ||
          event.key.code == sf::Keyboard::Numpad7) {
        appObject.SetSelectedColor(sf::Color::Magenta);
      }
      if (event.key.code == sf::Keyboard::Num8 ||
          event.key.code == sf::Keyboard::Numpad8) {
        appObject.SetSelectedColor(sf::Color::Cyan);
      }
    }
  }

  // handle the keyReleased events
  if (event.type == sf::Event::KeyReleased) {
    if (event.key.code == sf::Keyboard::Space) {
      std::cout << "Clear the canvas" << std::endl;
      appObject.ClearCanvas(appObject.GetSelectedColor());
    }
  }

  // We can otherwise handle events normally
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i coordinate = sf::Mouse::getPosition(appObject.GetWindow());
    sf::Image image = appObject.GetImage();
    // assuming the image size does not reach INT_MAX
    int xSize = (int)image.getSize().x;
    int ySize = (int)(image.getSize().y);
    if (coordinate.x >= 0 && coordinate.x < xSize && coordinate.y >= 0 &&
        coordinate.y < ySize) {
      sf::Color current_color = image.getPixel(coordinate.x, coordinate.y);
      appObject.AddCommand(std::make_shared<Draw>(coordinate.x, coordinate.y,
                                                  appObject.GetSelectedColor(), 
                                                  current_color));
      appObject.SendPacket(appObject.GetCommandQueue().front()->Serialize());
      appObject.ExecuteCommand();
    }
  }
  // Capture any keys that are released
  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
    exit(EXIT_SUCCESS);
  }

  std::shared_ptr<Command> received_command;
  received_command = appObject.ReceiveData();

  if(received_command != nullptr){
    std::cout << "Data received from server." << std::endl;
    // Need to update command's stored image here.
    appObject.AddCommand(received_command);
    appObject.ExecuteCommand();
  }

  // Where was the mouse previously before going to the next frame
  appObject.pmouseX = appObject.mouseX;
  appObject.pmouseY = appObject.mouseY;
}

/*! \brief 	The draw call
 *
 */
void draw(App &appObject) {
  // Static variable
  static int refreshRate = 0;
  ++refreshRate; // Increment

  // We load into our texture the modified pixels
  // But we only do so every 10 draw calls to reduce latency of transfer
  // between the GPU and CPU.
  // Ask yourself: Could we do better with sf::Clock and refresh once
  // 	 	 every 'x' frames?
  if (refreshRate > 10) {
    appObject.GetTexture().loadFromImage(appObject.GetImage());
    refreshRate = 0;
  }
}

/*! \brief 	The entry point into our program.
 *
 */
int main() {
  std::string username;
  unsigned short port;
  std::cout << "Enter your username:";
  std::cin >> username;
  App drawApp(username);
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
