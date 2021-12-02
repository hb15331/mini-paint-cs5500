#include "UDPNetworkServer.hpp"

int main() {
  UDPNetworkServer server("Server Name",sf::IpAddress::getLocalAddress(),50000);
  server.start();
}
