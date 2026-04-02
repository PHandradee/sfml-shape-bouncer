#include "rectangle_shape.hpp"
#include <algorithm>

namespace bouncer {

/**
 * @brief Constructs a RectangleShape with specified parameters
 */
RectangleShape::RectangleShape(const std::string& name,
                               const sf::Vector2f& position,
                               const sf::Vector2f& velocity,
                               const sf::Color& color,
                               float width,
                               float height,
                               const sf::Font& font,
                               unsigned int fontSize)
  : text_(font)
  , font_(font)
  , size_(width, height)
{
  // Initialize properties
  properties_.name = name;
  properties_.velocity = velocity;
  properties_.fontSize = fontSize;
  properties_.textColor = sf::Color::White;
  properties_.scale = 1.f;
  
  // Configure rectangle shape
  rectangle_.setSize(size_);
  rectangle_.setFillColor(color);
  rectangle_.setPosition(position);
  
  // Configure text label
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  updateText();
}

/**
 * @brief Draws the rectangle and its text label on the render window
 * @param window SFML render window to draw on
 */
void RectangleShape::draw(sf::RenderWindow& window) const {
  if (!properties_.visible) return;
  
  window.draw(rectangle_);
  window.draw(text_);
}

/**
 * @brief Updates rectangle position and handles boundary collisions
 * @param deltaTime Time elapsed since last update in seconds
 * @param windowSize Current window dimensions
 */
void RectangleShape::update(float deltaTime, const sf::Vector2u& windowSize) {
  if (!properties_.visible) return;
  
  // Update position based on velocity (normalized to 60 FPS)
  sf::Vector2f newPos = rectangle_.getPosition();
  newPos.x += properties_.velocity.x * deltaTime * 60.f;
  newPos.y += properties_.velocity.y * deltaTime * 60.f;
  
  // Get shape bounds for collision detection
  const sf::FloatRect bounds = getGlobalBounds();
  
  // Check collision with window boundaries and reverse velocity if needed
  // Left boundary
  if (bounds.position.x <= 0.f) {
    newPos.x = -bounds.position.x + rectangle_.getPosition().x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  // Right boundary
  else if (bounds.position.x + bounds.size.x >= static_cast<float>(windowSize.x)) {
    newPos.x = static_cast<float>(windowSize.x) - bounds.size.x - rectangle_.getPosition().x + newPos.x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  
  // Top boundary
  if (bounds.position.y <= 0.f) {
    newPos.y = -bounds.position.y + rectangle_.getPosition().y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  // Bottom boundary
  else if (bounds.position.y + bounds.size.y >= static_cast<float>(windowSize.y)) {
    newPos.y = static_cast<float>(windowSize.y) - bounds.size.y - rectangle_.getPosition().y + newPos.y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  
  // Apply new position
  rectangle_.setPosition(newPos);
  
  // Update text position after movement or collision
  updateText();
}

sf::FloatRect RectangleShape::getGlobalBounds() const {
  return rectangle_.getGlobalBounds();
}

sf::Vector2f RectangleShape::getPosition() const {
  return rectangle_.getPosition();
}

void RectangleShape::setPosition(const sf::Vector2f& position) {
  rectangle_.setPosition(position);
  updateText();
}

void RectangleShape::setScale(float scale) {
  const float clampedScale = std::clamp(scale, 0.1f, 4.f);
  rectangle_.setScale(sf::Vector2f{clampedScale, clampedScale});
  properties_.scale = clampedScale;
  updateText();
}

float RectangleShape::getScale() const {
  return rectangle_.getScale().x;
}

void RectangleShape::setSize(const sf::Vector2f& size) {
  size_ = size;
  rectangle_.setSize(size_);
  updateText();
}

/**
 * @brief Updates text font, content, size, color and centers it within the rectangle
 */
void RectangleShape::updateText() {
  text_.setFont(font_);
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  
  // Center text within the rectangle
  const sf::Vector2f textPos = calculateTextCenterPosition(text_);
  text_.setPosition(textPos);
}

} // namespace bouncer
