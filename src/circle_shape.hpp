#pragma once

#include "shape.hpp"
#include <SFML/Graphics/CircleShape.hpp>

namespace bouncer {

/**
 * @brief Classe que representa um círculo com propriedades customizadas
 */
class CircleShape : public Shape {
public:
  CircleShape(const std::string& name,
              const sf::Vector2f& position,
              const sf::Vector2f& velocity,
              const sf::Color& color,
              float radius,
              const sf::Font& font,
              unsigned int fontSize = 16u);
  
  void draw(sf::RenderWindow& window) const override;
  void update(float deltaTime, const sf::Vector2u& windowSize) override;
  sf::FloatRect getGlobalBounds() const override;
  sf::Vector2f getPosition() const override;
  void setPosition(const sf::Vector2f& position) override;
  void setScale(float scale) override;
  float getScale() const override;
  
  [[nodiscard]] float getRadius() const noexcept { return radius_; }
  void setRadius(float radius);

private:
  sf::CircleShape circle_;
  sf::Text text_;
  const sf::Font& font_;
  float radius_;
  
  void updateText();
};

} // namespace bouncer
