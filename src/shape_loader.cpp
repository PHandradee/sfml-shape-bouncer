#include "shape_loader.hpp"
#include "circle_shape.hpp"
#include "rectangle_shape.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bouncer {

/**
 * @brief Loads shapes from the configuration file
 * @param configManager Configuration manager with loaded font resource
 * @param filePath Path to the configuration file
 * @return Vector of unique pointers to loaded shapes
 */
std::vector<std::unique_ptr<Shape>> ShapeLoader::loadShapes(
  const ConfigManager& configManager,
  const std::string& filePath)
{
  std::vector<std::unique_ptr<Shape>> shapes;
  
  std::ifstream file(filePath);
  
  if (!file.is_open()) {
    std::cerr << "Error: Could not open configuration file: " 
              << filePath << std::endl;
    return shapes;
  }
  
  if (!configManager.isFontLoaded()) {
    std::cerr << "Error: Font not loaded" << std::endl;
    return shapes;
  }
  
  const sf::Font& font = configManager.getFont();
  const unsigned int fontSize = configManager.getFontConfig().size;
  
  std::string line;
  while (std::getline(file, line)) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
      continue;
    }
    
    const std::vector<std::string> tokens = tokenize(line);
    
    if (tokens.empty()) {
      continue;
    }
    
    const std::string& type = tokens[0];
    
    if (type == "Circle") {
      std::unique_ptr<Shape> shape = parseCircle(tokens, font, fontSize);
      if (shape) {
        shapes.push_back(std::move(shape));
      }
    } else if (type == "Rectangle") {
      std::unique_ptr<Shape> shape = parseRectangle(tokens, font, fontSize);
      if (shape) {
        shapes.push_back(std::move(shape));
      }
    }
    // Window and Font are processed by ConfigManager
  }
  
  file.close();
  return shapes;
}

/**
 * @brief Parses a Circle configuration line and creates a CircleShape
 * Format: Circle Name X Y SX SY R G B Radius
 * @param tokens Tokenized line components
 * @param font Reference to SFML font for text rendering
 * @param fontSize Font size in pixels
 * @return Unique pointer to created CircleShape or nullptr on error
 */
std::unique_ptr<Shape> ShapeLoader::parseCircle(
  const std::vector<std::string>& tokens,
  const sf::Font& font,
  unsigned int fontSize)
{
  // Expected format: Circle Name X Y SX SY R G B Radius
  // tokens[0] = "Circle"
  // tokens[1] = name
  // tokens[2] = X position
  // tokens[3] = Y position
  // tokens[4] = velocity X
  // tokens[5] = velocity Y
  // tokens[6] = color red component
  // tokens[7] = color green component
  // tokens[8] = color blue component
  // tokens[9] = radius
  
  if (tokens.size() < 10) {
    std::cerr << "Error: Invalid Circle line (insufficient parameters)" << std::endl;
    return nullptr;
  }
  
  try {
    const std::string name = tokens[1];
    const float x = std::stof(tokens[2]);
    const float y = std::stof(tokens[3]);
    const float sx = std::stof(tokens[4]);
    const float sy = std::stof(tokens[5]);
    const int r = std::stoi(tokens[6]);
    const int g = std::stoi(tokens[7]);
    const int b = std::stoi(tokens[8]);
    const float radius = std::stof(tokens[9]);
    
    const sf::Vector2f position(x, y);
    const sf::Vector2f velocity(sx, sy);
    const std::uint8_t rColor = static_cast<std::uint8_t>(r);
    const std::uint8_t gColor = static_cast<std::uint8_t>(g);
    const std::uint8_t bColor = static_cast<std::uint8_t>(b);
    const sf::Color color(rColor, gColor, bColor);
    
    return std::make_unique<CircleShape>(
      name, position, velocity, color, radius, font, fontSize);
    
  } catch (const std::exception& e) {
    std::cerr << "Error parsing Circle: " << e.what() << std::endl;
    return nullptr;
  }
}

/**
 * @brief Parses a Rectangle configuration line and creates a RectangleShape
 * Format: Rectangle Name X Y SX SY R G B Width Height
 * @param tokens Tokenized line components
 * @param font Reference to SFML font for text rendering
 * @param fontSize Font size in pixels
 * @return Unique pointer to created RectangleShape or nullptr on error
 */
std::unique_ptr<Shape> ShapeLoader::parseRectangle(
  const std::vector<std::string>& tokens,
  const sf::Font& font,
  unsigned int fontSize)
{
  // Expected format: Rectangle Name X Y SX SY R G B Width Height
  // tokens[0] = "Rectangle"
  // tokens[1] = name
  // tokens[2] = X position
  // tokens[3] = Y position
  // tokens[4] = velocity X
  // tokens[5] = velocity Y
  // tokens[6] = color red component
  // tokens[7] = color green component
  // tokens[8] = color blue component
  // tokens[9] = width
  // tokens[10] = height
  
  if (tokens.size() < 11) {
    std::cerr << "Error: Invalid Rectangle line (insufficient parameters)" << std::endl;
    return nullptr;
  }
  
  try {
    const std::string name = tokens[1];
    const float x = std::stof(tokens[2]);
    const float y = std::stof(tokens[3]);
    const float sx = std::stof(tokens[4]);
    const float sy = std::stof(tokens[5]);
    const int r = std::stoi(tokens[6]);
    const int g = std::stoi(tokens[7]);
    const int b = std::stoi(tokens[8]);
    const float w = std::stof(tokens[9]);
    const float h = std::stof(tokens[10]);
    
    const sf::Vector2f position(x, y);
    const sf::Vector2f velocity(sx, sy);
    const std::uint8_t rColor = static_cast<std::uint8_t>(r);
    const std::uint8_t gColor = static_cast<std::uint8_t>(g);
    const std::uint8_t bColor = static_cast<std::uint8_t>(b);
    const sf::Color color(rColor, gColor, bColor);
    
    return std::make_unique<RectangleShape>(
      name, position, velocity, color, w, h, font, fontSize);
    
  } catch (const std::exception& e) {
    std::cerr << "Error parsing Rectangle: " << e.what() << std::endl;
    return nullptr;
  }
}

/**
 * @brief Splits a string into whitespace-delimited tokens
 * @param line The input string to tokenize
 * @return Vector of string tokens
 */
std::vector<std::string> ShapeLoader::tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::string token;
  
  while (iss >> token) {
    tokens.push_back(token);
  }
  
  return tokens;
}

} // namespace bouncer
