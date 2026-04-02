#include "config_manager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bouncer {

/**
 * @brief Loads configuration settings from a file
 * @param filePath Path to the configuration file
 * @return true if loaded successfully, false otherwise
 */
bool ConfigManager::loadFromFile(const std::string& filePath) {
  std::ifstream file(filePath);
  
  if (!file.is_open()) {
    std::cerr << "Error: Could not open configuration file: " 
              << filePath << std::endl;
    return false;
  }
  
  std::string line;
  while (std::getline(file, line)) {
    // Skip empty lines and comments
    if (line.empty() || line[0] == '#') {
      continue;
    }
    
    parseLine(line);
  }
  
  file.close();
  return true;
}

/**
 * @brief Parses a single line from the configuration file
 * @param line The line content to parse
 */
void ConfigManager::parseLine(const std::string& line) {
  const std::vector<std::string> tokens = tokenize(line);
  
  if (tokens.empty()) {
    return;
  }
  
  const std::string& type = tokens[0];
  
  if (type == "Window") {
    parseWindow(tokens);
  } else if (type == "Font") {
    parseFont(tokens);
  }
  // Circle and Rectangle are processed by ShapeLoader
}

/**
 * @brief Parses a Window configuration line
 * @param tokens Tokenized line components [Window, width, height]
 */
void ConfigManager::parseWindow(const std::vector<std::string>& tokens) {
  if (tokens.size() < 3) {
    std::cerr << "Error: Invalid Window line" << std::endl;
    return;
  }
  
  try {
    windowConfig_.width = std::stoul(tokens[1]);
    windowConfig_.height = std::stoul(tokens[2]);
  } catch (const std::exception& e) {
    std::cerr << "Error parsing Window: " << e.what() << std::endl;
  }
}

/**
 * @brief Parses a Font configuration line and loads the font
 * @param tokens Tokenized line components [Font, path, size, R, G, B]
 */
void ConfigManager::parseFont(const std::vector<std::string>& tokens) {
  if (tokens.size() < 6) {
    std::cerr << "Error: Invalid Font line" << std::endl;
    return;
  }
  
  try {
    fontConfig_.filePath = tokens[1];
    fontConfig_.size = std::stoul(tokens[2]);
    const std::uint8_t r = static_cast<std::uint8_t>(std::stoi(tokens[3]));
    const std::uint8_t g = static_cast<std::uint8_t>(std::stoi(tokens[4]));
    const std::uint8_t b = static_cast<std::uint8_t>(std::stoi(tokens[5]));
    fontConfig_.color = sf::Color(r, g, b);
    
    // Load the font file
    if (font_.openFromFile(fontConfig_.filePath)) {
      fontLoaded_ = true;
    } else {
      std::cerr << "Error: Could not load font: " 
                << fontConfig_.filePath << std::endl;
      fontLoaded_ = false;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error parsing Font: " << e.what() << std::endl;
    fontLoaded_ = false;
  }
}

/**
 * @brief Splits a string into whitespace-delimited tokens
 * @param line The input string to tokenize
 * @return Vector of string tokens
 */
std::vector<std::string> ConfigManager::tokenize(const std::string& line) {
  std::vector<std::string> tokens;
  std::istringstream iss(line);
  std::string token;
  
  while (iss >> token) {
    tokens.push_back(token);
  }
  
  return tokens;
}

} // namespace bouncer
