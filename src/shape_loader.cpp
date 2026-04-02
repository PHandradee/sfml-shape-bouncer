#include "shape_loader.hpp"
#include "circle_shape.hpp"
#include "rectangle_shape.hpp"
#include <fstream>
#include <iostream>
#include <sstream>

namespace bouncer {

std::vector<std::unique_ptr<Shape>> ShapeLoader::loadShapes(
  const ConfigManager& configManager,
  const std::string& filePath)
{
  std::vector<std::unique_ptr<Shape>> shapes;
  
  std::ifstream file(filePath);
  
  if (!file.is_open()) {
    std::cerr << "Erro: Não foi possível abrir o arquivo de configuração: " 
              << filePath << std::endl;
    return shapes;
  }
  
  if (!configManager.isFontLoaded()) {
    std::cerr << "Erro: Fonte não carregada" << std::endl;
    return shapes;
  }
  
  const sf::Font& font = configManager.getFont();
  const unsigned int fontSize = configManager.getFontConfig().size;
  
  std::string line;
  while (std::getline(file, line)) {
    // Ignora linhas vazias ou comentários
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
    // Window e Font são processados pelo ConfigManager
  }
  
  file.close();
  return shapes;
}

std::unique_ptr<Shape> ShapeLoader::parseCircle(
  const std::vector<std::string>& tokens,
  const sf::Font& font,
  unsigned int fontSize)
{
  // Formato: Circle N X Y SX SY R G B Radius
  // tokens[0] = "Circle"
  // tokens[1] = name
  // tokens[2] = X
  // tokens[3] = Y
  // tokens[4] = SX (velocidade X)
  // tokens[5] = SY (velocidade Y)
  // tokens[6] = R (cor vermelha)
  // tokens[7] = G (cor verde)
  // tokens[8] = B (cor azul)
  // tokens[9] = Radius
  
  if (tokens.size() < 10) {
    std::cerr << "Erro: Linha Circle inválida (número insuficiente de parâmetros)" << std::endl;
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
    std::cerr << "Erro ao parsear Circle: " << e.what() << std::endl;
    return nullptr;
  }
}

std::unique_ptr<Shape> ShapeLoader::parseRectangle(
  const std::vector<std::string>& tokens,
  const sf::Font& font,
  unsigned int fontSize)
{
  // Formato: Rectangle N X Y SX SY R G B W H
  // tokens[0] = "Rectangle"
  // tokens[1] = name
  // tokens[2] = X
  // tokens[3] = Y
  // tokens[4] = SX (velocidade X)
  // tokens[5] = SY (velocidade Y)
  // tokens[6] = R (cor vermelha)
  // tokens[7] = G (cor verde)
  // tokens[8] = B (cor azul)
  // tokens[9] = W (largura)
  // tokens[10] = H (altura)
  
  if (tokens.size() < 11) {
    std::cerr << "Erro: Linha Rectangle inválida (número insuficiente de parâmetros)" << std::endl;
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
    std::cerr << "Erro ao parsear Rectangle: " << e.what() << std::endl;
    return nullptr;
  }
}

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
