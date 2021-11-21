/** 
 *  @file   Command.cpp 
 *  @brief  Implementation of Command.hpp
 *  @author Mike and Yufeng Gao 
 *  @date   2021-18-10 
 ***********************************************/

// Include our Third-Party SFML header
// #include ...
// Include standard library C++ libraries.
#include <string>
// Project header files
#include "Command.hpp"

/*! \brief 	N/A
*		
*/
Command::Command(std::string commandDescription) : m_commandDescription(commandDescription) {
}

/*! \brief 	N/A
*		
*/
Command::~Command(){
}
