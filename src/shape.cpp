#include "shape.hpp"
#include <algorithm>

namespace bouncer {

sf::Color Shape::getColor() const {
  // Será implementado nas classes derivadas
  return sf::Color::White;
}

void Shape::setColor(const sf::Color& color) {
  // Será implementado nas classes derivadas
  (void)color;
}

void Shape::setUI_Scale(float scale) {
  properties_.scale = std::clamp(scale, 0.f, 4.f);
  setScale(properties_.scale);
}

sf::Vector2f Shape::calculateTextCenterPosition(const sf::Text& text) const {
  const sf::FloatRect textBounds = text.getLocalBounds();
  const sf::FloatRect shapeBounds = getGlobalBounds();
  
  // Centraliza o texto na forma
  const float textX = shapeBounds.position.x + (shapeBounds.size.x / 2.f) - (textBounds.size.x / 2.f);
  const float textY = shapeBounds.position.y + (shapeBounds.size.y / 2.f) - (textBounds.size.y / 2.f);
  
  return sf::Vector2f(textX, textY);
}

} // namespace bouncer
