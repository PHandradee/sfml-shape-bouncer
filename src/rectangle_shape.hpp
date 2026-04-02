#pragma once

#include "shape.hpp"
#include <SFML/Graphics/RectangleShape.hpp>

namespace bouncer {

/**
 * @brief Class representing a rectangle with custom properties
 */
class RectangleShape : public Shape {
public:
  /**
   * @brief Constructs a RectangleShape with specified parameters
   * @param name Name identifier for the shape
   * @param position Initial X,Y position
   * @param velocity Velocity vector (X, Y)
   * @param color Fill color of the rectangle
   * @param width Width of the rectangle
   * @param height Height of the rectangle
   * @param font Reference to SFML font for text rendering
   * @param fontSize Font size in pixels (default: 16)
   */
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
  sf::RectangleShape rectangle_;   ///< SFML rectangle shape object
  sf::Text text_;                  ///< Text label for the shape
  const sf::Font& font_;           ///< Reference to font resource
  sf::Vector2f size_;              ///< Rectangle dimensions (width, height)
  
  /**
   * @brief Updates text position and appearance
   */
  void updateText();
};

} // namespace bouncer
