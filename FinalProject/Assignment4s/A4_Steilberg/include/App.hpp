/**
 *  @file   App.hpp
 *  @brief  App class interface
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/
#ifndef APP_HPP
#define APP_HPP

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Include standard library C++ libraries.
#include <queue>
#include <stack>
#include <memory>
#include <iostream>

// Project includes
#include "Command.hpp"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

using std::shared_ptr;
using std::make_shared;

/*! \brief Singleton pattern class to hold state of the drawing app */
class App{
private:
// Member variables
    // Keeps track of the previous mouse and current mouse positions
    // 'pmouse' is where the mouse previously was.
    // 'mouse' records where the mouse currently is.
    unsigned int m_pmouseX, m_pmouseY, m_mouseX, m_mouseY;

    // Pen size-- size of drawing square will be 2*pen_size + 1
    int m_pen_size;

    // Keeps track for when it is time to update the app image
    int m_refresh_count;

    // Queue stores the next command to do.
    std::queue<shared_ptr<Command>> m_commands;
    // Stack that stores the last action to occur.
    std::stack<shared_ptr<Command>> m_undo;
    // Stack that stores undone commands for redoing
    std::stack<shared_ptr<Command>> m_redo;
    // Main image
    sf::Image* m_image;
    // Create a sprite that we overaly
    // on top of the texture.
    sf::Sprite* m_sprite;
    // Texture sent to the GPU for rendering
    sf::Texture* m_texture;
    // Our rendering window
    sf::RenderWindow* m_window;
    // Represents the pen size. Size will be (2*pen_size + 1)**2 pixels


// Member functions
    // Store the address of our funcion pointer
    // for each of the callback functions.
    void (*m_initFunc)(App& app);
    void (*m_updateFunc)(App& app);
    void (*m_drawFunc)(App& app);

public:

// Member functions
    // Default constructor which is hidden in the Singleton
    App();
    App(const App& other);
    App& operator=(const App& other);
    void AddCommand(shared_ptr<Command> c);
    void UndoLastCommand();
    void ExecuteCommand();
    void RedoLastCommand();

    void UpdatePMouse(unsigned int mouseX, unsigned int mouseY);
    unsigned int GetMouseX();
    unsigned int GetMouseY();
    bool IsTimeForUpdate();
    void TextureUpdate();
    unsigned int CommandQueueSize();
    unsigned int UndoQueueSize();
    unsigned int RedoQueueSize();

    // Used to change the size of the pixel being drawn
    void IncPenSize();
    void DecPenSize();
    int GetPenSize();

    // Getters
    sf::Image& GetImage();
    sf::Texture& GetTexture();
    sf::RenderWindow& GetWindow();

    ~App();
    void Init(void (*initFunction)(App& app));
    void UpdateCallback(void (*updateFunction)(App& app));
    void DrawCallback(void (*drawFunction)(App& app));
    void Loop();

};

/*! \brief "clamps" a number to be no lower than low and no greater than high
 * Used to maintain bounds
 *
 * \param number Number to clamp
 * \param low minimum value for the number
 * \param high maximum value for the number
 */
inline int clamp(int number, int low, int high) {
  if (number <= low) {
      return low;
  } else if (number >= high) {
    return high;
  } else {
    return number;
  }
}


#endif
