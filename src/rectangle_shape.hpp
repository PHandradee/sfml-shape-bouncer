#pragma once

#include "shape.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace bouncer {

/**
 * @brief Classe que representa um retângulo com propriedades customizadas
 */
class RectangleShape : public Shape {
public:
  RectangleShape(const std::string& name,
                 const sf::Vector2f& position,
                 const sf::Vector2f& velocity,
                 const sf::Color& color,
                 float width,
                 float height,
                 const sf::Font& font,
                 unsigned int fontSize = 16u);
  
  void draw(sf::RenderWindow& window) const override;
  void update(float deltaTime, const sf::Vector2u& windowSize) override;
  sf::FloatRect getGlobalBounds() const override;
  sf::Vector2f getPosition() const override;
  void setPosition(const sf::Vector2f& position) override;
  void setScale(float scale) override;
  float getScale() const override;
  
  [[nodiscard]] sf::Vector2f getSize() const noexcept { return size_; }
  void setSize(const sf::Vector2f& size);

private:
  sf::RectangleShape rectangle_;
  sf::Text text_;
  const sf::Font& font_;
  sf::Vector2f size_;
  
  void updateText();
};

} // namespace bouncer
