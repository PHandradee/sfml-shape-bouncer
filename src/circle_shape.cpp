#include "circle_shape.hpp"
#include <algorithm>

namespace bouncer {

/**
 * @brief Constructs a CircleShape with specified parameters
 */
CircleShape::CircleShape(const std::string& name,
                         const sf::Vector2f& position,
                         const sf::Vector2f& velocity,
                         const sf::Color& color,
                         float radius,
                         const sf::Font& font,
                         unsigned int fontSize)
  : text_(font)
  , font_(font)
  , radius_(radius)
{
  // Initialize properties
  properties_.name = name;
  properties_.velocity = velocity;
  properties_.fontSize = fontSize;
  properties_.textColor = sf::Color::White;
  properties_.scale = 1.f;
  
  // Configure circle shape
  circle_.setRadius(radius_);
  circle_.setFillColor(color);
  circle_.setPosition(position);
  
  // Configure text label
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  updateText();
}

/**
 * @brief Draws the circle and its text label on the render window
 * @param window SFML render window to draw on
 */
void CircleShape::draw(sf::RenderWindow& window) const {
  if (!properties_.visible) return;
  
  window.draw(circle_);
  window.draw(text_);
}

/**
 * @brief Updates circle position and handles boundary collisions
 * @param deltaTime Time elapsed since last update in seconds
 * @param windowSize Current window dimensions
 */
void CircleShape::update(float deltaTime, const sf::Vector2u& windowSize) {
  if (!properties_.visible) return;
  
  // Update position based on velocity (normalized to 60 FPS)
  sf::Vector2f newPos = circle_.getPosition();
  newPos.x += properties_.velocity.x * deltaTime * 60.f;
  newPos.y += properties_.velocity.y * deltaTime * 60.f;
  
  // Get shape bounds for collision detection
  const sf::FloatRect bounds = getGlobalBounds();
  
  // Check collision with window boundaries and reverse velocity if needed
  // Left boundary
  if (bounds.position.x <= 0.f) {
    newPos.x = -bounds.position.x + circle_.getPosition().x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  // Right boundary
  else if (bounds.position.x + bounds.size.x >= static_cast<float>(windowSize.x)) {
    newPos.x = static_cast<float>(windowSize.x) - bounds.size.x - circle_.getPosition().x + newPos.x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  
  // Top boundary
  if (bounds.position.y <= 0.f) {
    newPos.y = -bounds.position.y + circle_.getPosition().y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  // Bottom boundary
  else if (bounds.position.y + bounds.size.y >= static_cast<float>(windowSize.y)) {
    newPos.y = static_cast<float>(windowSize.y) - bounds.size.y - circle_.getPosition().y + newPos.y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  
  // Apply new position
  circle_.setPosition(newPos);
  
  // Update text position after movement or collision
  updateText();
}

sf::FloatRect CircleShape::getGlobalBounds() const {
  return circle_.getGlobalBounds();
}

sf::Vector2f CircleShape::getPosition() const {
  return circle_.getPosition();
}

void CircleShape::setPosition(const sf::Vector2f& position) {
  circle_.setPosition(position);
  updateText();
}

void CircleShape::setScale(float scale) {
  const float clampedScale = std::clamp(scale, 0.1f, 4.f);
  circle_.setScale(sf::Vector2f{clampedScale, clampedScale});
  properties_.scale = clampedScale;
  updateText();
}

float CircleShape::getScale() const {
  return circle_.getScale().x;
}

void CircleShape::setRadius(float radius) {
  radius_ = radius;
  circle_.setRadius(radius_);
  updateText();
}

/**
 * @brief Updates text font, content, size, color and centers it within the circle
 */
void CircleShape::updateText() {
  text_.setFont(font_);
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  
  // Center text within the circle
  const sf::Vector2f textPos = calculateTextCenterPosition(text_);
  text_.setPosition(textPos);
}

} // namespace bouncer
