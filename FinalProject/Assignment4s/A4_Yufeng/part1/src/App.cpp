/** 
 *  @file   App.cpp 
 *  @brief  Main class for program
 *  @author Mike and Yufeng Gao 
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
App::App()
: m_commands(std::queue<Command*>()),
m_undo(std::stack<Command*>()),
m_redo(std::stack<Command*>()),
m_image(new sf::Image),
m_sprite(new sf::Sprite),
m_texture(new sf::Texture),
m_window(nullptr),
m_initFunc(nullptr),
m_updateFunc(nullptr),
m_drawFunc(nullptr),
pmouseX(0),
pmouseY(0),
mouseX(0),
mouseY(0)
{
}

/*! \brief 	Destruct the App object.
*		
*/
App::~App(){
	delete m_image;
	delete m_sprite;
	delete m_texture;
	while(!m_redo.empty()) {
		Command* disposed = m_redo.top();
		m_redo.pop();
		delete disposed;
	}
	while(!m_undo.empty()) {
		Command* disposed = m_undo.top();
		m_undo.pop();
		delete disposed;
	}
	while(!m_commands.empty()) {
		Command* disposed = m_commands.front();
		m_commands.pop();
		delete disposed;
	}
}

/*! \brief 	When we draw, we would prefer to add
*		a command to a data structure. 
*		
*/
void App::AddCommand(Command* c){
	if(c != nullptr){
		m_commands.push(c);
	}
}

/*! \brief 	The command(s) that were added is executed if it is 
*		not the same as the last executed command(excluding re-dos).
*		Once a new command is executed, the redo stack is cleared.
*/
void App::ExecuteCommand(){
	while(!m_commands.empty()){
		Command* item = m_commands.front();
		if (m_undo.empty() || !m_undo.top()->isEqual(*item)) {
			if (item->execute()) {
				m_undo.push(item);
			}
			while(!m_redo.empty()) {
				Command* disposed = m_redo.top();
				//destruct the Command pointer
				m_redo.pop();
				//delete the Command object from memory
				delete disposed;
			}
			m_commands.pop();
		}
		else {
			m_commands.pop();
			delete item;
		}
	}
}

/*! \brief 	Undo the latest command that has executed.
*		The command that's been undone can be invoked again in redo
*/
void App::Undo(){
	if (!m_undo.empty()) {
		Command* item = m_undo.top();
		if (item->undo()){
			m_redo.push(item);
		};
		m_undo.pop();
	}
};

/*! \brief 	Redo the latest command that has undone.
*		The command that's been re-done can be invoked again in undo
*/
void App::Redo(){
	if (!m_redo.empty()){
		Command* item = m_redo.top();
		if (item->execute()){
			m_undo.push(item);
		};
		m_redo.pop();
	}
};

/*! \brief 	Return a reference to our m_image, so that
*		we do not have to publicly expose it.
*		
*/
sf::Image& App::GetImage(){
	return *m_image;
}

/*! \brief 	Return a reference to our m_Texture so that
*		we do not have to publicly expose it.
*		
*/
sf::Texture& App::GetTexture(){
	return *m_texture;
}

/*! \brief 	Return a reference to our m_window so that we
*		do not have to publicly expose it.
*		
*/
sf::RenderWindow& App::GetWindow(){
	return *m_window;
}


/*! \brief 	Initializes the App and sets up the main
*		rendering window(i.e. our canvas.)
*/
void App::Init(void (*initFunction)(void)){
	// Create our window
	m_window = new sf::RenderWindow(sf::VideoMode(600,400),"Mini-Paint alpha 0.0.2",sf::Style::Titlebar);
	m_window->setVerticalSyncEnabled(true);
	// Create an image which stores the pixels we will update
	m_image->create(600,400,sf::Color::White);
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
void App::UpdateCallback(void (*updateFunction)(App&)){
	m_updateFunc = updateFunction;
}

/*! \brief 	Set a callback function which will be called
		each iteration of the main loop after update.
*		
*/
void App::DrawCallback(void (*drawFunction)(App&)){
	m_drawFunc = drawFunction;
}

/*! \brief 	The main loop function which handles initialization
		and will be executed until the main window is closed.
		Within the loop function the update and draw callback
		functions will be called.
*		
*/
void App::Loop(){
	// Call the init function
	m_initFunc();

	// Start the main rendering loop
	while(m_window->isOpen()){
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


