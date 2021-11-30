/** 
 *  @file   App.hpp 
 *  @brief  App class interface
 *  @author Mike and Yufeng Gao 
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
#include "Command.hpp"
// Project header files
// #include ...

// Singleton for our Application called 'App'.
class App{
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
	sf::Image* m_image;
	// Create a sprite that we overaly
	// on top of the texture.
	sf::Sprite* m_sprite;
	// Texture sent to the GPU for rendering
	sf::Texture* m_texture;
	// Our rendering window
    sf::RenderWindow* m_window;

// Member functions
	// Store the address of our funcion pointer
	// for each of the callback functions.	
	void (*m_initFunc)(void);
	void (*m_updateFunc)(App&);
	void (*m_drawFunc)(App&);
	//prevent implicit definition of copy constructor
	//and copy assignment
	App (const App&) = delete;
	App& operator= (const App&) = delete;

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
	App();
	~App();
	void AddCommand(std::shared_ptr<Command> c);
	void ExecuteCommand();
	sf::Color& GetSelectedColor();
	void SetSelectedColor(sf::Color color);
	void Undo();
	void Redo();
	sf::Image& GetImage();
	sf::Texture& GetTexture();
	sf::RenderWindow& GetWindow();
	void Init(void (*initFunction)(void));
	void UpdateCallback(void (*updateFunction)(App&));
	void DrawCallback(void (*drawFunction)(App&));
	void Loop();

};


#endif
