/**
 *  @file   main.cpp
 *  @brief  Entry point into the program.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
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
 *  \param appObject the App to initialize
 */
void initialization(App &appObject) {
  appObject.CreateUDPNetworkClient();
  std::cout << "Starting the App" << std::endl;
}

/*! \brief 	The update function for app to handle events.
 * 		This includes mouse and key events for draw, undo, redo
 *		and exit.
 *  \param appObject the App to update
 */
void update(App &appObject) {
  // Update our canvas
  sf::Event event;
  // Checking key events
  while (appObject.GetWindow().pollEvent(event)) {
    if (event.type == sf::Event::KeyPressed) {
      if (event.key.code == sf::Keyboard::Escape) {
        exit(EXIT_SUCCESS);
      } else if (event.key.code == sf::Keyboard::Y) {
        appObject.Redo();
      } else if (event.key.code == sf::Keyboard::Z) {
        appObject.Undo();
      } else if (event.key.code == sf::Keyboard::Num1 ||
                 event.key.code == sf::Keyboard::Numpad1) {
        appObject.SetSelectedColor(sf::Color::Black);
      } else if (event.key.code == sf::Keyboard::Num2 ||
                 event.key.code == sf::Keyboard::Numpad2) {
        appObject.SetSelectedColor(sf::Color::White);
      } else if (event.key.code == sf::Keyboard::Num3 ||
                 event.key.code == sf::Keyboard::Numpad3) {
        appObject.SetSelectedColor(sf::Color::Red);
      } else if (event.key.code == sf::Keyboard::Num4 ||
                 event.key.code == sf::Keyboard::Numpad4) {
        appObject.SetSelectedColor(sf::Color::Green);
      } else if (event.key.code == sf::Keyboard::Num5 ||
                 event.key.code == sf::Keyboard::Numpad5) {
        appObject.SetSelectedColor(sf::Color::Blue);
      } else if (event.key.code == sf::Keyboard::Num6 ||
                 event.key.code == sf::Keyboard::Numpad6) {
        appObject.SetSelectedColor(sf::Color::Yellow);
      } else if (event.key.code == sf::Keyboard::Num7 ||
                 event.key.code == sf::Keyboard::Numpad7) {
        appObject.SetSelectedColor(sf::Color::Magenta);
      } else if (event.key.code == sf::Keyboard::Num8 ||
                 event.key.code == sf::Keyboard::Numpad8) {
        appObject.SetSelectedColor(sf::Color::Cyan);
      }
    }

    // handle the keyReleased events
    else if (event.type == sf::Event::KeyReleased &&
             event.key.code == sf::Keyboard::Space) {
      appObject.ClearTheCanvas(appObject.GetSelectedColor());
    }
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    sf::Vector2i coordinate = sf::Mouse::getPosition(appObject.GetWindow());
    sf::Image image = appObject.GetImage();
    // assuming the image size does not reach INT_MAX
    int xSize = (int)image.getSize().x;
    int ySize = (int)(image.getSize().y);
    // Bounds check
    if (coordinate.x >= 0 && coordinate.x < xSize && coordinate.y >= 0 &&
        coordinate.y < ySize) {
      std::vector<std::vector<sf::Color>> current_colors;
      for (int i = -appObject.GetPenSize(); i <= appObject.GetPenSize(); ++i) {
        // We need to keep track of all the old pixels so that we can revert to
        // them
        std::vector<sf::Color> row;
        for (int j = -appObject.GetPenSize(); j <= appObject.GetPenSize();
             ++j) {
          int x = std::clamp(coordinate.x + i, 0, xSize);
          int y = std::clamp(coordinate.y + j, 0, ySize);
          row.push_back(image.getPixel(x, y));
        }
        // Keep track of the previous pixel color
        current_colors.push_back(row);
      }
      if (!appObject.IsOnline()) {
        // If offline we just directly add the command
        appObject.AddCommand(std::make_shared<Draw>(
            coordinate.x, coordinate.y, appObject.GetSelectedColor(),
            current_colors, appObject.GetPenSize()));
      } else {
        // If online we serialize, send the command and wait to get it back before
        // actually putting it in the execution queue
        auto command = std::make_shared<Draw>(
            coordinate.x, coordinate.y, appObject.GetSelectedColor(),
            current_colors, appObject.GetPenSize());
        // Serialize and add the packet to queue of packets to be send
        for (auto packet : command->Serialize()) {
          appObject.AddPacket(packet);
        }
      }
      appObject.ExecuteCommand();
    }
  }

  // Attempt to send a packet to the server
  appObject.SendPacket();

  // Capture input from the nuklear GUI
  nk_input_begin(appObject.m_ctx);
  while (appObject.GetGUIWindow().pollEvent(event)) {
    // Our close event.
    // Note: We only have a 'minimize' button
    //       in our window right now, so this event is not
    //       going to fire.
    if (event.type == sf::Event::Closed) {
      nk_sfml_shutdown();
      appObject.GetGUIWindow().close();
      exit(EXIT_SUCCESS);
    }

    // Capture any keys that are released
    else if (event.type == sf::Event::KeyReleased) {
      std::cout << "Key Pressed" << std::endl;
      // Check if the escape key is pressed.
      if (event.key.code == sf::Keyboard::Escape) {
        nk_sfml_shutdown();
        appObject.GetGUIWindow().close();
        exit(EXIT_SUCCESS);
      }
    }
    // else if(event.type == sf::Event::Resized){
    //    glViewport(0, 0, event.size.width, event.size.height);
    //}
    nk_sfml_handle_event(&event);
  }

  // Complete input from nuklear GUI
  nk_input_end(appObject.m_ctx);

  // Draw our GUI
  appObject.drawLayout(appObject.m_ctx);

  if (appObject.IsOnline()) {
    std::shared_ptr<Command> received_command;
    received_command = appObject.ReceiveData();

    if (received_command != nullptr) {
      std::cout << "Data received from server: " << received_command->ToString()
                << std::endl;
      // Need to update command's stored image here.
      appObject.AddCommand(received_command);
      appObject.ExecuteCommand();
    }
  }

  // Where was the mouse previously before going to the next frame
  appObject.pmouseX = appObject.mouseX;
  appObject.pmouseY = appObject.mouseY;
}

/*! \brief Actually draws the image to the screen
 * \param appObject the object to draw to
 */
void draw(App &appObject) {
  // Static variable
  static int refreshRate = 0;
  ++refreshRate; // Increment

  if (refreshRate > 10) {
    appObject.GetTexture().loadFromImage(appObject.GetImage());
    refreshRate = 0;
  }
}

/*! \brief 	The entry point into our program.
 * \return 0 if successful, not 0 otherwise
 */
int main() {
  std::string username;
  sf::IpAddress ip_address;
  std::cout << "Enter your username:";
  std::cin >> username;
  std::cout << "Enter the ipv4 address of the server:";
  std::cin >> ip_address;
  App drawApp(username, ip_address);
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
