#pragma once

#include "shape.hpp"
#include "config_manager.hpp"
#include <memory>
#include <vector>
#include <string>

namespace bouncer {

/**
 * @brief Shape loader that reads configuration file and creates shape objects
 */
class ShapeLoader {
public:
  /**
   * @brief Loads shapes from the configuration file
   * @param configManager Configuration manager with loaded font resource
   * @param filePath Path to the configuration file
   * @return Vector of unique pointers to loaded shapes
   */
  static std::vector<std::unique_ptr<Shape>> loadShapes(
    const ConfigManager& configManager,
    const std::string& filePath);

private:
  /**
   * @brief Parses a Circle configuration line and creates a CircleShape
   * @param tokens Tokenized line components
   * @param font Reference to SFML font for text rendering
   * @param fontSize Font size in pixels
   * @return Unique pointer to created CircleShape or nullptr on error
   */
  static std::unique_ptr<Shape> parseCircle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize);
  
  /**
   * @brief Parses a Rectangle configuration line and creates a RectangleShape
   * @param tokens Tokenized line components
   * @param font Reference to SFML font for text rendering
   * @param fontSize Font size in pixels
   * @return Unique pointer to created RectangleShape or nullptr on error
   */
  static std::unique_ptr<Shape> parseRectangle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize);
  
  /**
   * @brief Splits a string into whitespace-delimited tokens
   * @param line The input string to tokenize
   * @return Vector of string tokens
   */
  [[nodiscard]] static std::vector<std::string> tokenize(const std::string& line);
};

} // namespace bouncer
