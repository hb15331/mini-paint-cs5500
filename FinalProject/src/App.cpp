/**
 *  @file   App.cpp
 *  @brief  Main class for program
 *  @author Mike and Yufeng Gao
 *  @date   2021-18-10
 ***********************************************/
// Project header files
#include "App.hpp"

// All static members of a Singleton need to initialized to some value.
// This is so that when the program is compiled, memory is reserved
// for each of these static values.
// Potentially handy data structures for building an undo system.
// TODO: Decide, modify, add or remove any data structures you need.

// Function pointers
// void (*m_initFunc)(void) = nullptr;
// void (*m_updateFunc)(App&) = nullptr;
// void (*m_drawFunc)(App&) = nullptr;

/*!	\brief Construct the App object
 *
 */
App::App(std::string uname)
    : m_commands(std::queue<std::shared_ptr<Command>>()),
      m_undo(std::stack<std::shared_ptr<Command>>()),
      m_redo(std::stack<std::shared_ptr<Command>>()),
      m_selected_color(sf::Color::Red), m_image(new sf::Image),
      m_sprite(new sf::Sprite), m_texture(new sf::Texture), m_window(nullptr),
      m_initFunc(nullptr), m_updateFunc(nullptr), m_drawFunc(nullptr),
      pmouseX(0), pmouseY(0), mouseX(0), mouseY(0), m_uname(uname),
      m_udp_client(uname) {}

/*! \brief 	Destruct the App object.
 *
 */
App::~App() {
  delete m_image;
  delete m_sprite;
  delete m_texture;
  while (!m_redo.empty()) {
    m_redo.pop();
  }
  while (!m_undo.empty()) {
    m_undo.pop();
  }
  while (!m_commands.empty()) {
    m_commands.pop();
  }
}

/*! \brief 	When we draw, we would prefer to add
 *		a command to a data structure.
 *
 */
void App::AddCommand(std::shared_ptr<Command> c) {
  if (c != nullptr) {
    m_commands.push(c);
  }
}

/*! \brief 	The command(s) that were added is executed if it is
 *		not the same as the last executed command(excluding re-dos).
 *		Once a new command is executed, the redo stack is cleared.
 */
void App::ExecuteCommand() {
  while (!m_commands.empty()) {
    if (m_undo.empty() || !m_undo.top()->IsEqual(*m_commands.front())) {
      if (m_commands.front()->Execute()) {
        m_undo.push(m_commands.front());
      }
      while (!m_redo.empty()) {
        // destruct the Command pointer
        m_redo.pop();
      }
      m_commands.pop();
    } else {
      m_commands.pop();
    }
  }
}

/*! \brief 	Get the color used by client for painting. Default color is red.
 */
sf::Color &App::GetSelectedColor() { return m_selected_color; }

/*! \brief 	Set the color used by client for painting.
 */
void App::SetSelectedColor(sf::Color color) { m_selected_color = color; }

/*! \brief 	Undo the latest command that has executed.
 *		The command that's been undone can be invoked again in redo
 */
void App::Undo() {
  if (!m_undo.empty()) {
    if (m_undo.top()->Undo()) {
      m_redo.push(m_undo.top());
    };
    m_undo.pop();
  }
};

/*! \brief 	Redo the latest command that has undone.
 *		The command that's been re-done can be invoked again in undo
 */
void App::Redo() {
  if (!m_redo.empty()) {
    if (m_redo.top()->Execute()) {
      m_undo.push(m_redo.top());
    };
    m_redo.pop();
  }
};

/*! \brief 	Return a reference to our m_image, so that
 *		we do not have to publicly expose it.
 *
 */
sf::Image &App::GetImage() { return *m_image; }

/*! \brief 	Return a reference to our m_Texture so that
 *		we do not have to publicly expose it.
 *
 */
sf::Texture &App::GetTexture() { return *m_texture; }

/*! \brief 	Return a reference to our m_window so that we
 *		do not have to publicly expose it.
 *
 */
sf::RenderWindow &App::GetWindow() { return *m_window; }

void App::ClearCanvas(sf::Color color) {
  m_image->create(600, 400, color);
  m_texture->loadFromImage(*m_image);
  m_sprite->setTexture(*m_texture);
}

/*! \brief 	Initializes the App and sets up the main
 *		rendering window(i.e. our canvas.)
 */
void App::Init(void (*initFunction)(App &app)) {
  // Create our window
  m_window = new sf::RenderWindow(
      sf::VideoMode(600, 400), "Mini-Paint alpha 0.0.2", sf::Style::Titlebar);
  m_window->setVerticalSyncEnabled(true);
  // Create an image which stores the pixels we will update
  m_image->create(600, 400, sf::Color::White);
  assert(m_image != nullptr && "m_image != nullptr");
  // Create a texture which lives in the GPU and will render our image
  m_texture->loadFromImage(*m_image);
  assert(m_texture != nullptr && "m_texture != nullptr");
  // Create a sprite which is the entity that can be textured
  m_sprite->setTexture(*m_texture);
  assert(m_sprite != nullptr && "m_sprite != nullptr");
  // Set our initialization function to perform any user
  // initialization
  m_initFunc = initFunction;
}

/*! \brief 	Set a callback function which will be called
                each iteration of the main loop before drawing.
*
*/
void App::UpdateCallback(void (*updateFunction)(App &)) {
  m_updateFunc = updateFunction;
}

/*! \brief 	Set a callback function which will be called
                each iteration of the main loop after update.
*
*/
void App::DrawCallback(void (*drawFunction)(App &)) {
  m_drawFunc = drawFunction;
}

/*! \brief 	The main loop function which handles initialization
                and will be executed until the main window is closed.
                Within the loop function the update and draw callback
                functions will be called.
*
*/
void App::Loop() {
  // Call the init function
  m_initFunc(*this);

  // Start the main rendering loop
  while (m_window->isOpen()) {

    // Clear the window
    m_window->clear();
    // Updates specified by the user
    m_updateFunc(*this);
    // Additional drawing specified by user
    m_drawFunc(*this);
    // Update the texture
    // Note: This can be done in the 'draw call'
    // Draw to the canvas
    m_window->draw(*m_sprite);
    // Display the canvas
    m_window->display();
  }
}

/*!
 *
 */
void App::CreateUDPNetworkClient() {
  // FIXME Ideally in a real world application, a client would
  // never have to 'guess' which ports are open on a server.
  // One could improve this code by communicating with the server
  // what ports are 'open' for connection in the 'joinServer' function.
  // For now, we will create clients that can simply join however!
  m_udp_client.joinServer(sf::IpAddress::getLocalAddress(), 50000);
  m_udp_client.setUsername(m_uname);
  m_udp_client.sendString("Hello, " + m_uname + " is joining!");
}

/*!
 *
 */
UDPNetworkClient &App::GetUdpClient() { return m_udp_client; }

std::queue<std::shared_ptr<Command>> App::GetCommandQueue() {
  return m_commands;
}

/*!
 *
 */
void App::SendPacket(sf::Packet packet) {
  m_udp_client.SendPacket(packet);
}
