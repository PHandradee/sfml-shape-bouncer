#pragma once

#include "shape.hpp"
#include <memory>
#include <string>
#include <vector>
#include <SFML/Graphics/Font.hpp>

namespace bouncer {

/**
 * @brief Estrutura para armazenar configurações da janela
 */
struct WindowConfig {
  unsigned int width{800u};
  unsigned int height{600u};
};

/**
 * @brief Estrutura para armazenar configurações da fonte
 */
struct FontConfig {
  std::string filePath{};
  unsigned int size{16u};
  sf::Color color{sf::Color::White};
};

/**
 * @brief Gerenciador de configuração que lê e parseia o arquivo config.txt
 */
class ConfigManager {
public:
  ConfigManager() = default;
  
  /**
   * @brief Carrega as configurações do arquivo
   * @param filePath Caminho para o arquivo de configuração
   * @return true se carregou com sucesso, false caso contrário
   */
  bool loadFromFile(const std::string& filePath);
  
  /**
   * @brief Obtém a configuração da janela
   */
  [[nodiscard]] const WindowConfig& getWindowConfig() const noexcept { return windowConfig_; }
  
  /**
   * @brief Obtém a configuração da fonte
   */
  [[nodiscard]] const FontConfig& getFontConfig() const noexcept { return fontConfig_; }
  
  /**
   * @brief Obtém a fonte carregada
   */
  [[nodiscard]] const sf::Font& getFont() const noexcept { return font_; }
  
  /**
   * @brief Verifica se a fonte foi carregada com sucesso
   */
  [[nodiscard]] bool isFontLoaded() const noexcept { return fontLoaded_; }

private:
  WindowConfig windowConfig_;
  FontConfig fontConfig_;
  sf::Font font_;
  bool fontLoaded_{false};
  
  /**
   * @brief Parseia uma linha do arquivo de configuração
   */
  void parseLine(const std::string& line);
  
  /**
   * @brief Parseia uma linha Window
   */
  void parseWindow(const std::vector<std::string>& tokens);
  
  /**
   * @brief Parseia uma linha Font
   */
  void parseFont(const std::vector<std::string>& tokens);
  
  /**
   * @brief Divide uma string em tokens
   */
  [[nodiscard]] static std::vector<std::string> tokenize(const std::string& line);
};

} // namespace bouncer
