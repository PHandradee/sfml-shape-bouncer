#include "config_manager.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bouncer {

bool ConfigManager::loadFromFile(const std::string& filePath) {
  std::ifstream file(filePath);
  
  if (!file.is_open()) {
    std::cerr << "Erro: Não foi possível abrir o arquivo de configuração: " 
              << filePath << std::endl;
    return false;
  }
  
  std::string line;
  while (std::getline(file, line)) {
    // Ignora linhas vazias ou comentários
    if (line.empty() || line[0] == '#') {
      continue;
    }
    
    parseLine(line);
  }
  
  file.close();
  return true;
}

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
  // Circle e Rectangle são processados pelo ShapeLoader
}

void ConfigManager::parseWindow(const std::vector<std::string>& tokens) {
  if (tokens.size() < 3) {
    std::cerr << "Erro: Linha Window inválida" << std::endl;
    return;
  }
  
  try {
    windowConfig_.width = std::stoul(tokens[1]);
    windowConfig_.height = std::stoul(tokens[2]);
  } catch (const std::exception& e) {
    std::cerr << "Erro ao parsear Window: " << e.what() << std::endl;
  }
}

void ConfigManager::parseFont(const std::vector<std::string>& tokens) {
  if (tokens.size() < 6) {
    std::cerr << "Erro: Linha Font inválida" << std::endl;
    return;
  }
  
  try {
    fontConfig_.filePath = tokens[1];
    fontConfig_.size = std::stoul(tokens[2]);
    const std::uint8_t r = static_cast<std::uint8_t>(std::stoi(tokens[3]));
    const std::uint8_t g = static_cast<std::uint8_t>(std::stoi(tokens[4]));
    const std::uint8_t b = static_cast<std::uint8_t>(std::stoi(tokens[5]));
    fontConfig_.color = sf::Color(r, g, b);
    
    // Carrega a fonte
    if (font_.openFromFile(fontConfig_.filePath)) {
      fontLoaded_ = true;
    } else {
      std::cerr << "Erro: Não foi possível carregar a fonte: " 
                << fontConfig_.filePath << std::endl;
      fontLoaded_ = false;
    }
  } catch (const std::exception& e) {
    std::cerr << "Erro ao parsear Font: " << e.what() << std::endl;
    fontLoaded_ = false;
  }
}

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
