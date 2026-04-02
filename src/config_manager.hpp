#pragma once

#include "shape.hpp"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics/Font.hpp>

namespace bouncer {

/**
 * @brief Structure for storing window configuration settings
 */
struct WindowConfig {
  unsigned int width{800u};    ///< Window width in pixels
  unsigned int height{600u};   ///< Window height in pixels
};

/**
 * @brief Structure for storing font configuration settings
 */
struct FontConfig {
  std::string filePath{};      ///< Path to the font file
  unsigned int size{16u};      ///< Font size in pixels
  sf::Color color{sf::Color::White};  ///< Default text color
};

/**
 * @brief Configuration manager that reads and parses config.txt file
 */
class ConfigManager {
public:
  ConfigManager() = default;
  
  /**
   * @brief Loads configuration settings from a file
   * @param filePath Path to the configuration file
   * @return true if loaded successfully, false otherwise
   */
  bool loadFromFile(const std::string& filePath);
  
  /**
   * @brief Gets the window configuration settings
   * @return Const reference to WindowConfig structure
   */
  [[nodiscard]] const WindowConfig& getWindowConfig() const noexcept { return windowConfig_; }
  
  /**
   * @brief Gets the font configuration settings
   * @return Const reference to FontConfig structure
   */
  [[nodiscard]] const FontConfig& getFontConfig() const noexcept { return fontConfig_; }
  
  /**
   * @brief Gets the loaded SFML font object
   * @return Const reference to the SFML Font
   */
  [[nodiscard]] const sf::Font& getFont() const noexcept { return font_; }
  
  /**
   * @brief Checks if the font was loaded successfully
   * @return true if font is loaded, false otherwise
   */
  [[nodiscard]] bool isFontLoaded() const noexcept { return fontLoaded_; }

private:
  WindowConfig windowConfig_;   ///< Window configuration settings
  FontConfig fontConfig_;       ///< Font configuration settings
  sf::Font font_;               ///< Loaded SFML font resource
  bool fontLoaded_{false};      ///< Flag indicating if font loaded successfully
  
  /**
   * @brief Parses a single line from the configuration file
   * @param line The line content to parse
   */
  void parseLine(const std::string& line);
  
  /**
   * @brief Parses a Window configuration line
   * @param tokens Tokenized line components
   */
  void parseWindow(const std::vector<std::string>& tokens);
  
  /**
   * @brief Parses a Font configuration line
   * @param tokens Tokenized line components
   */
  void parseFont(const std::vector<std::string>& tokens);
  
  /**
   * @brief Splits a string into whitespace-delimited tokens
   * @param line The input string to tokenize
   * @return Vector of string tokens
   */
  [[nodiscard]] static std::vector<std::string> tokenize(const std::string& line);
};

} // namespace bouncer
