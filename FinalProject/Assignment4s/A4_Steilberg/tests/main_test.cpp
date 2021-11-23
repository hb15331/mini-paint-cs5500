/**
 *  @file   main_test.cpp
 *  @brief  Unit Tests for our program
 *  @author Mike and ????
 *  @date   yyyy-dd-mm
 ***********************************************/

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

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


void initialization(App& app){
	std::cout << "Starting the App" << std::endl;
}

void draw(App& app) {

}

void update_check_queuesize(App& app) {
  sf::Vector2i coordinate = sf::Vector2i(1,1);
  REQUIRE (app.CommandQueueSize() == 0);
  app.AddCommand(make_shared<Draw>("Draw", app.GetImage(),
                                   coordinate,
                                   sf::Color::Red, app.GetPenSize()));

  REQUIRE (app.CommandQueueSize() == 1);
  app.ExecuteCommand();
  REQUIRE (app.CommandQueueSize() == 0);
}

void update_add_pixel(App& app) {
  sf::Vector2i coordinate = sf::Vector2i(1,1);
  app.AddCommand(make_shared<Draw>("Draw", app.GetImage(),
                                   coordinate,
                                   sf::Color::Red, app.GetPenSize()));
  app.ExecuteCommand();
}


/*! \brief Initialize and destroy the program
*
*/
TEST_CASE("init and destroy"){
  App app;
  app.Init(&initialization);
  // Destroy our app
}


TEST_CASE("Check queue size changes when issuing/executing commands") {
  App app;
  app.Init(&initialization);
  //app.UpdateCallback(&draw);
  //app.DrawCallback(&update);
  update_check_queuesize(app);
}

TEST_CASE("Check drawn pixel actually works") {
  App app;
  app.Init(&initialization);
  REQUIRE (app.GetTexture().copyToImage().getPixel(1,1) == sf::Color::White);
  update_add_pixel(app);
  app.TextureUpdate();
  REQUIRE (app.GetTexture().copyToImage().getPixel(1,1) == sf::Color::Red);
}

TEST_CASE("Check pen size increment results in change in pen size") {
  App app;
  app.Init(&initialization);
  REQUIRE (app.GetPenSize() == 0);
  app.IncPenSize();
  REQUIRE (app.GetPenSize() == 1);
  app.DecPenSize();
  REQUIRE (app.GetPenSize() == 0);
}


TEST_CASE("Check pen size increment results in pixels drawn in increased radius") {
  App app;
  app.Init(&initialization);
  REQUIRE (app.GetPenSize() == 0);
  update_add_pixel(app);
  app.TextureUpdate();
  REQUIRE (app.GetTexture().copyToImage().getPixel(1,1) == sf::Color::Red);
  // Pixel unchanged outside of drawn pixel area
  REQUIRE (app.GetTexture().copyToImage().getPixel(1,2) == sf::Color::White);
  app.IncPenSize();
  update_add_pixel(app);
  app.TextureUpdate();
  // Pixel now red due to expanded pixel area
  REQUIRE (app.GetTexture().copyToImage().getPixel(1,2) == sf::Color::Red);
}
