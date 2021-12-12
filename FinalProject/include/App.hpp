/**
 *  @file   App.hpp
 *  @brief  App class interface
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/
#ifndef APP_HPP
#define APP_HPP

// The 'C-based' libraries are needed for Nuklear GUI
#include <string.h>
// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Network.hpp>
#include <SFML/OpenGL.hpp>
#include <SFML/Window.hpp>
// Include standard library C++ libraries.
#include "Command.hpp"
#include "UDPNetworkClient.hpp"
#include "deserialize.hpp"
#include "UndoRedo.hpp"
#include <cassert>
#include <memory>
#include <queue>
#include <stack>

// NUKLEAR - for our GUI
#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
// #define NK_IMPLEMENTATION
// #define NK_SFML_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sfml_gl2.h"

#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 400

// Singleton for our Application called 'App'.
class App {
private:
  // Member variables
  // Queue stores the next command to do.
  std::queue<std::shared_ptr<Command>> m_commands;
  // Stack that stores the last action to occur.
  std::stack<std::shared_ptr<Command>> m_undo;
  // Stack that stores the last action to redo.
  std::stack<std::shared_ptr<Command>> m_redo;
  // Color that is selected for painting. Default is red
  sf::Color m_selected_color;
  // Main image
  sf::Image *m_image;
  // Create a sprite that we overlay
  // on top of the texture.
  sf::Sprite *m_sprite;
  // Texture sent to the GPU for rendering
  sf::Texture *m_texture;
  // Our rendering window
  sf::RenderWindow *m_window;
  sf::RenderWindow *m_option_window;

  // Username
  std::string m_uname;

  // The network client
  UDPNetworkClient m_udp_client;
  // Pen size retrieved from GUI setting
  int m_pen_size;

  // Member functions
  // Store the address of our funcion pointer
  // for each of the callback functions.
  void (*m_initFunc)(App &);
  void (*m_updateFunc)(App &);
  void (*m_drawFunc)(App &);

  // prevent implicit definition of copy constructor
  // and copy assignment
  App(const App &) = delete;
  App &operator=(const App &) = delete;

  // Is the app connected to the server?
  bool m_is_online;

public:

  // Keeps track of the previous mouse and current mouse positions
  // 'pmouse' is where the mouse previously was.
  // 'mouse' records where the mouse currently is.
  unsigned int pmouseX, pmouseY, mouseX, mouseY;

  // Member functions
  // Constructor
  App(std::string uname);

  // Destructor
  ~App();

  // Adds a new command to the command queue
  void AddCommand(std::shared_ptr<Command> c);

  // Get the commands needed to be executed
  std::queue<std::shared_ptr<Command>> GetCommandQueue();

  // Execute command at front of queue
  void ExecuteCommand();

  // Get currently selected pen color
  sf::Color &GetSelectedColor();

  // Set currently selected pen color
  void SetSelectedColor(sf::Color color);

  // Undo last action
  void Undo();

  // Undo last undo :)
  void Redo();

  // Get the drawin image/texture data
  sf::Image &GetImage();
  sf::Texture &GetTexture();

  // Get handles to drawing/gui window
  sf::RenderWindow &GetWindow();
  sf::RenderWindow &GetGUIWindow();

  // Function to render our GUI
  void drawLayout(struct nk_context* ctx);
  struct nk_context *m_ctx;


  void Init(void (*initFunction)(App &));
  void UpdateCallback(void (*updateFunction)(App &));
  void DrawCallback(void (*drawFunction)(App &));

  // Main app loop
  void Loop();

  // Set canvas to a given color
  void ClearCanvas(sf::Color color);

  // Attempt to connect to the server. If connected then m_is_online becomes true
  void CreateUDPNetworkClient();

  // Send packet to server
  void SendPacket(sf::Packet packet);

  // Gets the pen size in pixels
  int GetPenSize();

  // Returns true if the app has an active connection to the server, false otherwise
  bool IsOnline();

  // Get the current udp client (Note: You should check that you're online
  //before you use it...)
  UDPNetworkClient &GetUdpClient();

  // Receive command from the server
  std::shared_ptr<Command> ReceiveData();
};

#endif
