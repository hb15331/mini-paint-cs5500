/** 
 *  @file   main.cpp 
 *  @brief  Entry point into the program.
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/

// Compile: Use the CMakeLists.txt to generate a build file
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


/*! \brief  Call any initailization functions here.
*       This might be for example setting up any
*       global variables, allocating memory,
*       dynamically loading any libraries, or
*       doing nothing at all.
*
*/
void initialization(App& app){
    std::cout << "Starting the App" << std::endl;
}


/*! \brief  The update function presented can be simplified.
*       I have demonstrated two ways you can handle events,
*       if for example we want to add in an event loop.
*
*/
void update(App& app){
    // Update our canvas
    sf::Event event;
    while(app.GetWindow().pollEvent(event)){
    }

    // We can otherwise handle events normally
    if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
        sf::Vector2i coordinate = sf::Mouse::getPosition(app.GetWindow());
        // Fix bound segfault bug involving moving mouse outside of window while drawing
        coordinate.x = clamp(coordinate.x, 0, WINDOW_WIDTH);
        coordinate.y = clamp(coordinate.y, 0, WINDOW_HEIGHT);

        // Adding the draw command
        // mmmmm smart pointers
        app.AddCommand(make_shared<Draw>("Draw", app.GetImage(),
                                          coordinate,
                                          sf::Color::Red, app.GetPenSize()));
    }

    // Support undoing
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
        app.UndoLastCommand();
    }

    // Support redoing
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
        app.RedoLastCommand();
    }

    // Press minus to decrease pen size
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Hyphen)) {
        app.DecPenSize();
    }

    // Press equals to increase pen size
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Equal)) {
        app.IncPenSize();
    }

    // TODO: Implement screen clearing
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
        // Blank out window color
        //image.create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
        //texture.loadFromImage(image);

    }

    // Exit the program if esc is pressed
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
        exit(EXIT_SUCCESS);
    }

    // Where was the mouse previously before going to the next frame
    app.UpdatePMouse(app.GetMouseX(), app.GetMouseY());

    app.ExecuteCommand();
}


/*! \brief  The draw call */
void draw(App& app){
    // We load into our texture the modified pixels
    // But we only do so every 10 draw calls to reduce latency of transfer
    // between the GPU and CPU.
    // Ask yourself: Could we do better with sf::Clock and refresh once
    //       every 'x' frames?
    if(app.IsTimeForUpdate()){
        app.TextureUpdate();
    }
}

 
/*! \brief  The entry point into our program */
int main(){
    // Call any setup function
    // Passing a function pointer into the 'init' function.
    // of our application.
    App app;
    
    app.Init(&initialization);
    // Setup your keyboard
    app.UpdateCallback(&update);
    // Setup the Draw Function
    app.DrawCallback(&draw);
    // Call the main loop function
    app.Loop();
    return 0;
}

