/**
 *  @file   App.cpp
 *  @brief  Main class for program
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/


// Project header files
#include "App.hpp"
#define NK_IMPLEMENTATION
#define NK_SFML_GL2_IMPLEMENTATION
#include "nuklear.h"
#include "nuklear_sfml_gl2.h"

/*!	\brief Construct the App object
 * \return new app object
 */
App::App(std::string uname, sf::IpAddress ip_address)
    : m_commands(std::queue<std::shared_ptr<Command>>()),
      m_undo(std::stack<std::shared_ptr<Command>>()),
      m_redo(std::stack<std::shared_ptr<Command>>()),
      m_selected_color(sf::Color::Red), m_image(new sf::Image),
      m_sprite(new sf::Sprite), m_texture(new sf::Texture), m_window(nullptr),
      m_option_window(nullptr), m_initFunc(nullptr), m_updateFunc(nullptr),
      m_drawFunc(nullptr), m_is_online(false), pmouseX(0), pmouseY(0),
      mouseX(0), mouseY(0), m_uname(uname), m_ip_address(ip_address),
      m_udp_client(uname), m_ctx(new struct nk_context), m_pen_size(1),
      m_packet_queue(), m_packet_send_clock() {
  m_packet_send_clock.restart();
}

/*! \brief 	Destruct the App object */
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
      // if (m_commands.front()->Execute()) {
      if (m_commands.front()->Execute(GetImage())) {
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
  if (IsOnline()) {
    UndoRedo cmd = UndoRedo("Undo");
    // Only one component packet
    AddPacket(cmd.Serialize()[0]);
  } else {
    if (!m_undo.empty()) {
      if (m_undo.top()->Undo(GetImage())) {
        m_redo.push(m_undo.top());
      };
      m_undo.pop();
    }
  }
};

/*! \brief 	Redo the latest command that has undone.
 *		The command that's been re-done can be invoked again in undo
 */
void App::Redo() {
  if (IsOnline()) {
    UndoRedo cmd = UndoRedo("Redo");
    // Only one component packet
    AddPacket(cmd.Serialize()[0]);
  } else {
    if (!m_redo.empty()) {
      if (m_redo.top()->Execute(*m_image)) {
        m_undo.push(m_redo.top());
      } else {
        throw std::runtime_error("Error redoing " + m_redo.top()->ToString());
      }
      m_redo.pop();
    }
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

/*! \brief 	Return a reference to our m_option_window so that we
 *		do not have to publicly expose it.
 *
 */
sf::RenderWindow &App::GetGUIWindow() { return *m_option_window; }

/*!	\brief Create and set a new image/canvas for the app object,
 *    taking the a color to set the canvas to as a parameter.
 */
void App::ClearTheCanvas(const sf::Color color) {
  std::cout << "Clear the canvas" << std::endl;
  if (IsOnline()) {
    ClearCanvas clear_canvas(color, *m_image, false);
    for (sf::Packet pack : clear_canvas.Serialize()) {
      AddPacket(pack);
    }
  } else {
    auto ptr = std::make_shared<ClearCanvas>(color, *m_image, false);
    AddCommand(ptr);
    ExecuteCommand();
  }
}

/*! \brief 	Initializes the App and sets up the main
 *		rendering window(i.e. our canvas.)
 */
void App::Init(void (*initFunction)(App &app)) {
  // Setup the context
  sf::ContextSettings settings(24, 8, 4, 2, 2);
  // Create our window
  m_window =
      new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                           "Mini-Paint alpha 0.0.2", sf::Style::Titlebar);
  m_window->setVerticalSyncEnabled(true);
  /// Create a GUI window to draw to
  m_option_window =
      new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                           "GUI Window", sf::Style::Default, settings);
  m_option_window->setVerticalSyncEnabled(true);
  m_option_window->setActive(true);
  glViewport(0, 0, m_option_window->getSize().x, m_option_window->getSize().y);
  m_ctx = nk_sfml_init(m_option_window);
  // Load Fonts: if none of these are loaded a default font will be used
  struct nk_font_atlas *atlas;
  nk_sfml_font_stash_begin(&atlas);
  nk_sfml_font_stash_end();
  // // Setup a color for the nuklear gui
  // struct nk_colorf bg;
  // Create an image which stores the pixels we will update
  m_image->create(WINDOW_WIDTH, WINDOW_HEIGHT, sf::Color::White);
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
 *               each iteration of the main loop after update.
 *
 */
void App::DrawCallback(void (*drawFunction)(App &)) {
  m_drawFunc = drawFunction;
}

/*! \brief 	The main loop function which handles initialization
 *               and will be executed until the main window is closed.
 *               Within the loop function the update and draw callback
 *               functions will be called.
 *
 */
void App::Loop() {
  // Call the init function
  m_initFunc(*this);

  // Start the main rendering loop
  while (m_window->isOpen() && m_option_window->isOpen()) {
    m_option_window->setActive(true);
    m_option_window->clear();
    glClearColor(m_selected_color.r, m_selected_color.g, m_selected_color.b,
                 m_selected_color.a);
    glClear(GL_COLOR_BUFFER_BIT);
    nk_sfml_render(NK_ANTI_ALIASING_ON);
    m_option_window->display();
    // Clear the window
    m_window->setActive(true);
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

/*! \brief Creates and saves the udp client */
void App::CreateUDPNetworkClient() {
  // FIXME Ideally in a real world application, a client would
  // never have to 'guess' which ports are open on a server.
  // One could improve this code by communicating with the server
  // what ports are 'open' for connection in the 'joinServer' function.
  // For now, we will create clients that can simply join however!
  m_udp_client.setUsername(m_uname);
  int result = m_udp_client.joinServer(m_ip_address, 50000);
  // Commenting below out to allow for user-entered ip to be connected to.
  // int result = m_udp_client.joinServer(sf::IpAddress::getLocalAddress(),
  // 50000);
  if (result == 1) {
    std::cout << "Online mode!" << std::endl;
    m_is_online = true;
  } else {
    std::cout << "Offline mode!" << std::endl;
    m_is_online = false;
  }
}

/*! \brief Gets the udp client
 * \return The udp client
 */
UDPNetworkClient &App::GetUdpClient() { return m_udp_client; }

std::queue<std::shared_ptr<Command>> App::GetCommandQueue() {
  return m_commands;
}

/*! \brief sends first packet in queue to the server
 * \return true if successful
 */
bool App::SendPacket() {
  float wait_time = .1f;
  if (!m_packet_queue.empty() && m_packet_queue.front().getDataSize() > 10000) {
    wait_time = .3f;
  }
  if (!m_packet_queue.empty() &&
      m_packet_send_clock.getElapsedTime() > sf::seconds(wait_time)) {
    sf::Packet packet = m_packet_queue.front();
    m_packet_queue.pop();
    m_udp_client.SendPacket(packet);
    m_packet_send_clock.restart();
  } else {
    return false;
  }
}

/*! \brief Attempts to receive a command from the server
 * \return Pointer to a command if one was obtained; nullptr if not
 */
std::shared_ptr<Command> App::ReceiveData() {
  sf::Packet packet = m_udp_client.ReceiveData();
  try {
    std::shared_ptr<Command> command = Deserialize(packet);
    if (command == nullptr) {
      return nullptr;
    } else {
      return command;
    }
  } catch (std::runtime_error e) {
    // std::cout << e.what() << std::endl;
    return nullptr;
  }
}

/*! \brief Returns online status of the app
 * \return true if the app is online, false otherwise
 */
bool App::IsOnline() { return m_is_online; }

/*! \brief Draws the GUI
 * \param nk_context Nuklear GUI context pointer
 */
void App::drawLayout(struct nk_context *ctx) {
  /* GUI */
  if (nk_begin(ctx, "Demo", nk_rect(50, 50, 300, 300),
               NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                   NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
    static int property = 20;
    nk_layout_row_static(ctx, 30, 80, 1);
    // Commenting out the "Button" button.
    // if (nk_button_label(ctx, "button"))
    //     fprintf(stdout, "button pressed\n");

    nk_layout_row_dynamic(ctx, 30, 2);
    if (nk_option_label(ctx, "red", m_selected_color == sf::Color::Red)) {
      m_selected_color = sf::Color::Red;
    }
    if (nk_option_label(ctx, "black", m_selected_color == sf::Color::Black)) {
      m_selected_color = sf::Color::Black;
    }
    if (nk_option_label(ctx, "white/erase",
                        m_selected_color == sf::Color::White)) {
      m_selected_color = sf::Color::White;
    }
    if (nk_option_label(ctx, "green", m_selected_color == sf::Color::Green)) {
      m_selected_color = sf::Color::Green;
    }
    if (nk_option_label(ctx, "blue", m_selected_color == sf::Color::Blue)) {
      m_selected_color = sf::Color::Blue;
    }
    if (nk_option_label(ctx, "yellow", m_selected_color == sf::Color::Yellow)) {
      m_selected_color = sf::Color::Yellow;
    }
    if (nk_option_label(ctx, "magenta",
                        m_selected_color == sf::Color::Magenta)) {
      m_selected_color = sf::Color::Magenta;
    }
    if (nk_option_label(ctx, "cyan", m_selected_color == sf::Color::Cyan)) {
      m_selected_color = sf::Color::Cyan;
    }

    nk_layout_row_dynamic(ctx, 30, 3);
    nk_label(ctx, "Pen size:", NK_TEXT_LEFT);
    nk_slider_int(ctx, 1, &m_pen_size, 40, 1);
    std::string s = std::to_string(m_pen_size);
    char const *pen_char = s.c_str(); // use char const* as target type
    nk_label(ctx, pen_char, NK_TEXT_RIGHT);
    // nk_layout_row_dynamic(ctx, 25, 1);
    // //Commenting out the "Compression" button.
    // // nk_property_int(ctx, "Compression:", 0, &property, 100, 10, 1);

    // nk_layout_row_dynamic(ctx, 20, 1);
    // nk_label(ctx, "background:", NK_TEXT_LEFT);
    // nk_layout_row_dynamic(ctx, 25, 1);
    // if (nk_combo_begin_color(ctx, nk_rgb_cf(bg),
    // nk_vec2(nk_widget_width(ctx),400))) {
    //     nk_layout_row_dynamic(ctx, 120, 1);
    //     bg = nk_color_picker(ctx, bg, NK_RGBA);
    //     nk_layout_row_dynamic(ctx, 25, 1);
    //     bg.r = nk_propertyf(ctx, "#R:", 0, bg.r, 1.0f, 0.01f,0.005f);
    //     bg.g = nk_propertyf(ctx, "#G:", 0, bg.g, 1.0f, 0.01f,0.005f);
    //     bg.b = nk_propertyf(ctx, "#B:", 0, bg.b, 1.0f, 0.01f,0.005f);
    //     bg.a = nk_propertyf(ctx, "#A:", 0, bg.a, 1.0f, 0.01f,0.005f);
    //     nk_combo_end(ctx);
    // }
  }
  nk_end(ctx);
}

/*! \brief Adds a packet to the send queue
 * \param packet The packet to send
 */
void App::AddPacket(sf::Packet packet) { m_packet_queue.push(packet); }

/*! Returns the pen size in pixels
 * \return Pen size in pixels
 */
int App::GetPenSize() { return m_pen_size; }
