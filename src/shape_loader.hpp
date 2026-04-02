#pragma once

#include "shape.hpp"
#include "config_manager.hpp"
#include <memory>
#include <vector>
#include <string>

namespace bouncer {

/**
 * @brief Carregador de formas que lê do arquivo de configuração e cria as formas
 */
class ShapeLoader {
public:
  /**
   * @brief Carrega formas do arquivo de configuração
   * @param configManager Gerenciador de configuração já carregado
   * @param filePath Caminho para o arquivo de configuração
   * @return Vetor de ponteiros únicos para as formas carregadas
   */
  static std::vector<std::unique_ptr<Shape>> loadShapes(
    const ConfigManager& configManager,
    const std::string& filePath);

private:
  /**
   * @brief Parseia uma linha Circle do arquivo de configuração
   */
  static std::unique_ptr<Shape> parseCircle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize);
  
  /**
   * @brief Parseia uma linha Rectangle do arquivo de configuração
   */
  static std::unique_ptr<Shape> parseRectangle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize);
  
  /**
   * @brief Divide uma string em tokens
   */
  [[nodiscard]] static std::vector<std::string> tokenize(const std::string& line);
};

} // namespace bouncer
