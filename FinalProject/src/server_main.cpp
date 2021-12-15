/**
 *  @file   server_main.cpp
 *  @brief  Main for server app.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#include "UDPNetworkServer.hpp"

/*!	\brief Main for running the server.
 * \return 0 if successful, not 0 otherwise
 */
int main() {
  UDPNetworkServer server("Server Name", sf::IpAddress::getLocalAddress(),
                          50000);
  server.start();
}
