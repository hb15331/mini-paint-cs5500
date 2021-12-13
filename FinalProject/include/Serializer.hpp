/**
 *  @file   Serializer.hpp
 *  @brief  Serializer for sending packets.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <SFML/Network>

#include <string>
#include <exception>
#include <iostream>

#include "Command.h"

/*! \brief Singleton class used to serialize packets (yay singleton!)
*
*/
class Serializer
{
  public:
    Serializer() = delete;
    static sf::Packet Serialize(Command command);
    static sf::Packet Serialize(std::string command_str);
};

#endif // SERIALIZE_H
