/**
 *  @file   deserialize.hpp
 *  @brief  Deserializer for receiving packets.
 *  @author Alex, Hualin, Jackson and Yufeng
 *  @date   2021-12-13
 ***********************************************/

#ifndef DESERIALIZE_HPP_INCLUDED
#define DESERIALIZE_HPP_INCLUDED

#include "Command.hpp"
#include "UndoRedo.hpp"
#include <exception>

/*! \brief turns a packet back into a command
 * \return Command form of packet
 */
inline std::shared_ptr<Command> Deserialize(sf::Packet packet) {
  std::string cmd_type, username;
  int xcoord, ycoord, pen_size;
  bool inverted;
  uint32_t curr_color, old_color_rows, old_color_cols;
  std::vector<std::vector<sf::Color>> prev_colors;
  if (!(packet >> cmd_type) &&
      packet.getDataSize() > 0) {
    std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
    return nullptr;
  }
  if (cmd_type == "Draw") {
    if (!(packet >> xcoord >> ycoord >> pen_size >> inverted >> curr_color >> old_color_rows >> old_color_cols) &&
      packet.getDataSize() > 0) {
      std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
      return nullptr;
    }
    //std::cout << "COLOR DESERIALIZE\n";
    for(size_t row_idx = 0; row_idx < old_color_rows; ++row_idx) {
      std::vector<sf::Color> row;
      for(size_t col_idx = 0; col_idx < old_color_cols; ++col_idx) {
        uint32_t color_int;
        if (!(packet >> color_int)) {
          std::cout << "Old color deserialization error at row " << row_idx << ", col " << col_idx << std::endl;
          return nullptr;
        }
        row.push_back(sf::Color(color_int));
        //std::cout << color_int << " ";
      }
      //std::cout << std::endl;
      prev_colors.push_back(row);
    }
    packet >> username;

    return (std::make_shared<Draw>(xcoord, ycoord, sf::Color(curr_color),
                                   prev_colors, pen_size, inverted));
  } else if (cmd_type == "UndoRedo") {
    std::string cmd;
    if (!(packet >> cmd >> username)) {
      std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
      return nullptr;
    }
    return (std::make_shared<UndoRedo>(cmd));
  } else if (cmd_type == "String") {
    std::string out;
    std::cout << "String packet\n" << out << std::endl;
    return nullptr;
  } else if (cmd_type == "ClearComponent") {
    std::string trash;
    packet >> trash;
    if (!(packet >> xcoord >> ycoord >> pen_size >> inverted >> curr_color >> old_color_rows >> old_color_cols) &&
      packet.getDataSize() > 0) {
      std::cout << "Error deserializing packet " << packet
              << " size: " << packet.getDataSize() << std::endl;
      return nullptr;
    }
    //std::cout << "COLOR DESERIALIZE\n";
    for(size_t row_idx = 0; row_idx < old_color_rows; ++row_idx) {
      std::vector<sf::Color> row;
      for(size_t col_idx = 0; col_idx < old_color_cols; ++col_idx) {
        uint32_t color_int;
        if (!(packet >> color_int)) {
          std::cout << "Old color deserialization error at row " << row_idx << ", col " << col_idx << std::endl;
          return nullptr;
        }
        row.push_back(sf::Color(color_int));
        //std::cout << color_int << " ";
      }
      //std::cout << std::endl;
      prev_colors.push_back(row);
    }
    packet >> username;

    return (std::make_shared<Draw>(xcoord, ycoord, sf::Color(curr_color),
                                   prev_colors, pen_size, inverted, true));
  } else {
    return nullptr;
  }
}

#endif // DESERIALIZE_HPP_INCLUDED
