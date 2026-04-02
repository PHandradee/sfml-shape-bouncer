#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

namespace bouncer {

/**
 * @brief Estrutura que contém todas as propriedades de uma forma
 * que não estão armazenadas na própria sf::Shape
 */
struct ShapeProperties {
  std::string name{};           ///< Nome da forma
  sf::Vector2f velocity{0.f, 0.f};  ///< Velocidade (SX, SY)
  sf::Color textColor{sf::Color::White};  ///< Cor do texto
  unsigned int fontSize{16u};   ///< Tamanho da fonte
  bool visible{true};           ///< Visibilidade da forma
  float scale{1.f};             ///< Escala da forma (0 a 4)
};

/**
 * @brief Classe base para todas as formas geométricas
 */
class Shape {
public:
  virtual ~Shape() = default;
  
  virtual void draw(sf::RenderWindow& window) const = 0;
  virtual void update(float deltaTime, const sf::Vector2u& windowSize) = 0;
  virtual sf::FloatRect getGlobalBounds() const = 0;
  virtual sf::Vector2f getPosition() const = 0;
  virtual void setPosition(const sf::Vector2f& position) = 0;
  virtual void setScale(float scale) = 0;
  virtual float getScale() const = 0;
  
  // Getters e setters para propriedades
  [[nodiscard]] const std::string& getName() const noexcept { return properties_.name; }
  void setName(const std::string& name) { properties_.name = name; }
  
  [[nodiscard]] sf::Vector2f getVelocity() const noexcept { return properties_.velocity; }
  void setVelocity(const sf::Vector2f& velocity) { properties_.velocity = velocity; }
  
  [[nodiscard]] sf::Color getColor() const;
  void setColor(const sf::Color& color);
  
  [[nodiscard]] sf::Color getTextColor() const noexcept { return properties_.textColor; }
  void setTextColor(const sf::Color& color) { properties_.textColor = color; }
  
  [[nodiscard]] unsigned int getFontSize() const noexcept { return properties_.fontSize; }
  void setFontSize(unsigned int size) { properties_.fontSize = size; }
  
  [[nodiscard]] bool isVisible() const noexcept { return properties_.visible; }
  void setVisible(bool visible) { properties_.visible = visible; }
  
  [[nodiscard]] float getUI_Scale() const noexcept { return properties_.scale; }
  void setUI_Scale(float scale);

protected:
  ShapeProperties properties_;
  
  /**
   * @brief Calcula a posição centralizada para o texto
   */
  [[nodiscard]] sf::Vector2f calculateTextCenterPosition(
    const sf::Text& text) const;
};

} // namespace bouncer
