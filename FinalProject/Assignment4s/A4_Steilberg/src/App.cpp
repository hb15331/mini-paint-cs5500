/**
 *  @file   App.cpp
 *  @brief  Main class for program
 *  @author Mike and Jack Steilberg
 *  @date   2021-18-10
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>

// Include standard library C++ libraries.
#include <cassert>

// Project header files
#include "App.hpp"

App::App() :
    m_pmouseX(0),
    m_pmouseY(0),
    m_mouseX(0),
    m_mouseY(0),
    m_pen_size(0),
    m_refresh_count(0),
    m_commands(),
    m_undo(),
    m_redo(),
    m_image(new sf::Image),
    m_sprite(new sf::Sprite),
    m_texture(new sf::Texture),
    m_window(new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT),
                                  "Mini-Paint alpha 0.0.2",
                                  sf::Style::Titlebar)),
    m_initFunc(nullptr),
    m_updateFunc(nullptr),
    m_drawFunc(nullptr) {

}

App::App(const App& other) :
    m_pmouseX(other.m_pmouseX),
    m_pmouseY(other.m_pmouseY),
    m_mouseX(other.m_mouseX),
    m_mouseY(other.m_mouseY),
    m_pen_size(other.m_pen_size),
    m_refresh_count(other.m_refresh_count),
    m_commands(other.m_commands),
    m_undo(other.m_undo),
    m_redo(other.m_redo),
    m_image(other.m_image),
    m_sprite(other.m_sprite),
    m_texture(other.m_texture),
    m_window(other.m_window),
    m_initFunc(other.m_initFunc),
    m_updateFunc(other.m_updateFunc),
    m_drawFunc(other.m_drawFunc) {

}

App& App::operator=(const App& other) {
    this->m_pmouseX = other.m_pmouseX,
    this->m_pmouseY = other.m_pmouseY;
    this->m_mouseX = other.m_mouseX;
    this->m_mouseY = other.m_mouseY;
    this->m_commands = other.m_commands;
    this->m_undo = other.m_undo;
    this->m_redo = other.m_redo;
    this->m_initFunc = other.m_initFunc;
    this->m_updateFunc = other.m_updateFunc;
    this->m_drawFunc = other.m_drawFunc;
    this->m_pen_size = other.m_pen_size;
    return *this;
}


/*! \brief Increases the size of the pen by 1 */
void App::IncPenSize() {
    m_pen_size++;
    std::cout << "Pen size: " << m_pen_size << std::endl;
}

/*! \brief Reduces the size of the pen by 1 */
void App::DecPenSize() {
    // Bounds check
    if(m_pen_size > 0) {
        m_pen_size--;
        std::cout << "Pen size: " << m_pen_size << std::endl;
    }
}

/*! \brief Gets the size of the drawing pen.
 * \return Size of the pen in pixels (square is 2*pen_size + 1 squared)
 */
int App::GetPenSize() {
    return m_pen_size;
}


/*! \brief Undoes the last command that was executed */
void App::UndoLastCommand() {
    // Check we still have commands to undo
    if (!m_undo.empty()) {
        // Undo the command
        m_undo.top()->undo();
        // Make the command redoable
        m_redo.push(m_undo.top());
        // Make the command only undoable once
        m_undo.pop();
        std::cout << "Undo!" << std::endl;
    } else {
        std::cout << "Undo stack empty!" << std::endl;
    }
}

/*! \brief Redoes the last command that was undone */
void App::RedoLastCommand() {
    // Check we still have commands to redo
    if (!m_redo.empty()) {
        // Execute the redo command
        m_redo.top()->execute();
        // Make it undoable again
        m_undo.push(m_redo.top());
        // Make it only redoable once
        m_redo.pop();
        std::cout << "Redo!" << std::endl;
    } else {
        std::cout << "Redo stack empty!" << std::endl;
    }
}

/*! \brief Adds a command to the command queue
 *
 * \param c Pointer to the command
 */
void App::AddCommand(shared_ptr<Command> c){
    // Check if command is same as previous command
    if (m_undo.empty() || !(*m_undo.top() == *c)) {
        m_commands.push(c);

        // Add to the undo stack
        m_undo.push(c);

        // Since we have added more commands redoing makes no sense
        // So we empty the stack
        while(!m_redo.empty()) {
            m_redo.pop();
        }
    } else {
        std::cout << "Dupe suppressed!" << std::endl;
    }
}

/*! \brief Returns the size of the command queue
 * \return Size of the command queue
 */
unsigned int App::CommandQueueSize() {
    return m_commands.size();
}

/*! \brief Returns the size of the undo queue
 * \return Size of the undo queue
 */
unsigned int App::UndoQueueSize() {
    return m_undo.size();
}

/*! \brief Returns the size of the redo queue
 * \return Size of the redo queue
 */
unsigned int App::RedoQueueSize() {
    return m_redo.size();
}

/*! \brief Loads the image into the app texture
 *
 */
void App::TextureUpdate() {
    this->GetTexture().loadFromImage(this->GetImage());
    m_refresh_count = 0;
}

/*! \brief Executes all commands in the command queue */
void App::ExecuteCommand(){
    if (!m_commands.empty()) {
        m_commands.front()->execute();
        m_commands.pop();
    }
}

/*! \brief  Return a reference to our m_image, so that
 * we do not have to publicly expose it.
 *
 * \return The image
 */
sf::Image& App::GetImage(){
    return *m_image;
}

/*! \brief  Return a reference to our m_Texture so that
 * we do not have to publicly expose it.
 *
 * \return The texture
 */
sf::Texture& App::GetTexture(){
    return *m_texture;
}

/*! \brief  Return a reference to our m_window so that we
 * do not have to publicly expose it.
 *
 * \return The render window
 */
sf::RenderWindow& App::GetWindow(){
    return *m_window;
}

/*! \brief Returns true if the app is ready to refresh its image
 *
 * \return True if app is ready to refresh image, false otherwise
 */
bool App::IsTimeForUpdate() {
    return (m_refresh_count > 3);
}

/*! \brief  Destroy we manually call at end of our program. */
App::~App(){
    delete m_image;
    delete m_sprite;
    delete m_texture;
}

/*! \brief  Initializes the App and sets up the main
 * rendering window(i.e. our canvas.)
 *
 * \param initFunction Callback function
*/
void App::Init(void (*initFunction)(App& app)){
    // Create our window
    m_window = new sf::RenderWindow(sf::VideoMode(WINDOW_WIDTH,WINDOW_HEIGHT),"Mini-Paint alpha 0.0.2",sf::Style::Titlebar);
    m_window->setVerticalSyncEnabled(true);

    // Create an image which stores the pixels we will update
    m_image->create(WINDOW_WIDTH,WINDOW_HEIGHT,sf::Color::White);
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

/*! \brief  Set a callback function which will be called
 * each iteration of the main loop before drawing.
 *
 * \param updateFunction Callback function
 */
void App::UpdateCallback(void (*updateFunction)(App& app)){
    m_updateFunc = updateFunction;
}

/*! \brief  Set a callback function which will be called
 * each iteration of the main loop after update.
 *
 * \param drawFunction Callback function
 */
void App::DrawCallback(void (*drawFunction)(App& app)){
    m_drawFunc = drawFunction;
}

/*! \brief update the previous mouse coordinates
 *
 * \param mouseX x mouse coordinate
 * \param mouseY y mouse coordinate
 */
void App::UpdatePMouse(unsigned int mouseX, unsigned int mouseY) {
    m_pmouseX = mouseX;
    m_pmouseY = mouseY;
}

/*! \brief Gets the mouse x coordinate
 *
 * \returns Mouse x coordinate
 */
unsigned int App::GetMouseX() {
    return m_mouseX;
}

/*! \brief Gets the mouse y coordinate
 *
 * \returns Mouse y coordinate
 */
unsigned int App::GetMouseY() {
    return m_mouseY;
}

/*! \brief  The main loop function which handles initialization
 * and will be executed until the main window is closed.
 * Within the loop function the update and draw callback
 * functions will be called.
*/
void App::Loop(){
    // Call the init function
    m_initFunc(*this);

    // Start the main rendering loop
    while(m_window->isOpen()){
        m_refresh_count++;
        // Clear the window
        m_window->clear();
        // Updates specified by the user
        m_updateFunc(*this);
        // Additional drawing specified by user
        m_drawFunc(*this);
        // Draw to the canvas
        m_window->draw(*m_sprite);
        // Display the canvas
        m_window->display();
    }
}


