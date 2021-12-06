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
#define NK_IMPLEMENTATION
#define NK_SFML_GL2_IMPLEMENTATION
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

public:
  // Member Variables
  // Keeps track of the previous mouse and current mouse positions
  // 'pmouse' is where the mouse previously was.
  // 'mouse' records where the mouse currently is.
  // Note: Occassinally I break my rules of always having a getter/setter
  //	 function for each member variable if there's reason to.
  //   	 There is some overhead associated with calling these functions,
  // 	 and even with 'inline' there is no gaureentee that the functions
  //  	 do get inlined. Other commercial and open source APIs seem
  // 	 to do this on occassion--use your disgression and expression to
  // 	 make an informed choice if exposuing these variables is safe, fast,
  // 	 and going to be more maintainable in the long run. In the end--it's
  // 	 still risky to expose anything as public.
  unsigned int pmouseX, pmouseY, mouseX, mouseY;
  // Member functions
  App(std::string uname);
  ~App();
  void AddCommand(std::shared_ptr<Command> c);
  std::queue<std::shared_ptr<Command>> GetCommandQueue();
  void ExecuteCommand();
  sf::Color &GetSelectedColor();
  void SetSelectedColor(sf::Color color);
  void Undo();
  void Redo();
  sf::Image &GetImage();
  sf::Texture &GetTexture();
  sf::RenderWindow &GetWindow();
  sf::RenderWindow &GetGUIWindow();
  // Function to render our GUI
  void drawLayout(struct nk_context* ctx);
  struct nk_context *m_ctx;
  void Init(void (*initFunction)(App &));
  void UpdateCallback(void (*updateFunction)(App &));
  void DrawCallback(void (*drawFunction)(App &));
  void Loop();
  void ClearCanvas(sf::Color color);
  void CreateUDPNetworkClient();
  void SendPacket(sf::Packet packet);
  UDPNetworkClient &GetUdpClient();
  std::shared_ptr<Command> ReceiveData();
};

#endif
