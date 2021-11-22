/** 
 *  @file   App.cpp 
 *  @brief  Main class for program
 *  @author Mike and ???? 
 *  @date   yyyy-dd-mm 
 ***********************************************/

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
// Include standard library C++ libraries.
#include <cassert>
#include <iostream>
// Project header files
#include "App.hpp"

// All static members of a Singleton need to initialized to some value.
// This is so that when the program is compiled, memory is reserved
// for each of these static values.
// Potentially handy data structures for building an undo system.
// TODO: Decide, modify, add or remove any data structures you need.
std::queue<Command*> m_commands;
std::stack<Command*> m_undo;
std::stack<Command*> m_redo;

// Function pointers
//void (*m_initFunc)(void) = nullptr;
//void (*m_updateFunc)(void) = nullptr;
//void (*m_drawFunc)(void) = nullptr;
// Mouse variables
//unsigned int pmouseX=0;
//unsigned int pmouseY=0;
//unsigned int mouseX=0;
//unsigned int mouseY=0;
// Canvas variables
//sf::RenderWindow* m_window = nullptr;
//sf::Image* m_image = new sf::Image;
//sf::Sprite* m_sprite = new sf::Sprite;
//sf::Texture* m_texture = new sf::Texture;



/*! \brief      N/A
*               
*               
*/
App::App() {
	std::cout << "App created once" << std::endl;
	// initialize canvas variables
	m_image = new sf::Image;
	m_texture = new sf::Texture;
	m_sprite = new sf::Sprite;
	// initialize mouse variables
	pmouseX = 0;
	pmouseY = 0;
	mouseX = 0;
	mouseY = 0;
	// initialize function pointers
	void (*m_initFunc)(void) = nullptr;
	void (*m_updateFunc)(App&) = nullptr;
	void (*m_drawFunc)(App&) = nullptr;
}


/*! \brief      N/A
*               
*               
*/
App::~App(){
	std::cout << "Destroyed!\n";
	delete m_image;
	delete m_sprite;
	delete m_texture;
}


/*! \brief 	When we draw, we would prefer to add
*		a command to a data structure.
*		
*/
void App::AddCommand(Command* c){
//	 TODO: You may refactor as needed
	m_commands.push(c);

}


/*! \brief      N/A
*               
*/
void App::undo_command(){
	if(!m_undo.empty()) {
		Command* c = m_undo.top();
		c -> undo(m_image);
		m_redo.push(c);
		m_undo.pop();
	} else {
		std::cout << "undo stack is empty" << std::endl;
	}

}


/*! \brief      N/A
*               
*/
void App::redo_command(){
	if (!m_redo.empty()) {
		Command* c = m_redo.top();
		c -> redo(m_image);
		m_redo.pop();
	} else {
		std::cout << "redo stack is empty" << std::endl;
	}

}



/*! \brief 	We should execute commands in a data structure
*		Perhaps we will have to modify the logic in our
*		loop!
*		
*/
void App::ExecuteCommand(){
//	 TODO: You may refactor as needed
	while (!m_commands.empty()) {
		Command* c = m_commands.front();
		// undo stck records the last command executed
		Command* prev = nullptr;
		if (!m_undo.empty()) {
			prev = m_undo.top();
		}
		// avoid executing same command twice in a row
		if (m_undo.empty() || !c -> isEqual(*prev)) {
			c -> execute(m_image, mouseX, mouseY);
			m_undo.push(c);
			m_commands.pop();
		}
	
	}

}

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

/*! \brief 	Destroy we manually call at end of our program.
*		
*/
//void App::Destroy(){
//	delete m_image;
//	delete m_sprite;
//	delete m_texture;
//}

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


