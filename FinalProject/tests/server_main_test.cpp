/**
 *  @file   main_test.cpp
 *  @brief  Unit Tests for our program
 *  @author Mike and Yufeng
 *  @date   2021-17-11
 ***********************************************/

#define CATCH_CONFIG_MAIN
#include "catch_amalgamated.hpp"

// Include our Third-Party SFML header
#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Network.hpp>
// Include standard library C++ libraries.
#include <iostream>
#include <memory>
#include <string>
// Project header files
#include "App.hpp"
#include "Command.hpp"
#include "Draw.hpp"
#include "UDPNetworkClient.hpp"
#include "UDPNetworkServer.hpp"
#include "Packet.hpp"
#include "deserialize.hpp"

TEST_CASE("Create server object, bind to port") {
    UDPNetworkServer server("Server Name", sf::IpAddress::getLocalAddress(), 50000);
    bool status;
    status = server.bindToPort();
    REQUIRE( status == true );
}

// TEST_CASE("Receive packet") {
//     sf::Packet packet;
//     UDPNetworkServer server("Server Name", sf::IpAddress::getLocalAddress(), 50000);
//     server.bindToPort();
//     while(m_socket.receive(packet, sf::IpAddress::getLocalAddress(), 50001) != sf::Socket::Done) {
//         //Send test packet
//     }
// }