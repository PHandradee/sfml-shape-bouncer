#include "shape.hpp"
#include <algorithm>

namespace bouncer {

/**
 * @brief Gets the fill color of the shape (default implementation)
 * @return White color by default, overridden in derived classes
 */
sf::Color Shape::getColor() const {
  return sf::Color::White;
}

/**
 * @brief Sets the fill color of the shape (default implementation)
 * @param color Color to set (unused in base class)
 */
void Shape::setColor(const sf::Color& color) {
  (void)color;
}

/**
 * @brief Sets the UI scale factor with clamping between 0 and 4
 * @param scale Desired scale value
 */
void Shape::setUI_Scale(float scale) {
  properties_.scale = std::clamp(scale, 0.f, 4.f);
  setScale(properties_.scale);
}

/**
 * @brief Calculates centered position for text within the shape
 * @param text The text object to center
 * @return 2D vector with centered position coordinates
 */
sf::Vector2f Shape::calculateTextCenterPosition(const sf::Text& text) const {
  const sf::FloatRect textBounds = text.getLocalBounds();
  const sf::FloatRect shapeBounds = getGlobalBounds();
  
  // Center text horizontally and vertically within the shape bounds
  const float textX = shapeBounds.position.x + (shapeBounds.size.x / 2.f) - (textBounds.size.x / 2.f);
  const float textY = shapeBounds.position.y + (shapeBounds.size.y / 2.f) - (textBounds.size.y / 2.f);
  
  return sf::Vector2f(textX, textY);
}

} // namespace bouncer
