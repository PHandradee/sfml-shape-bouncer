#include "circle_shape.hpp"
#include <algorithm>

namespace bouncer {

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
  // Configura propriedades
  properties_.name = name;
  properties_.velocity = velocity;
  properties_.fontSize = fontSize;
  properties_.textColor = sf::Color::White;
  properties_.scale = 1.f;
  
  // Configura o círculo
  circle_.setRadius(radius_);
  circle_.setFillColor(color);
  circle_.setPosition(position);
  
  // Configura o texto
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  updateText();
}

void CircleShape::draw(sf::RenderWindow& window) const {
  if (!properties_.visible) return;
  
  window.draw(circle_);
  window.draw(text_);
}

void CircleShape::update(float deltaTime, const sf::Vector2u& windowSize) {
  if (!properties_.visible) return;
  
  // Atualiza posição baseada na velocidade
  sf::Vector2f newPos = circle_.getPosition();
  newPos.x += properties_.velocity.x * deltaTime * 60.f; // Normalizado para 60 FPS
  newPos.y += properties_.velocity.y * deltaTime * 60.f;
  
  // Obtém bounds da forma
  const sf::FloatRect bounds = getGlobalBounds();
  
  // Verifica colisão com as bordas e inverte velocidade se necessário
  // Borda esquerda
  if (bounds.position.x <= 0.f) {
    newPos.x = -bounds.position.x + circle_.getPosition().x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  // Borda direita
  else if (bounds.position.x + bounds.size.x >= static_cast<float>(windowSize.x)) {
    newPos.x = static_cast<float>(windowSize.x) - bounds.size.x - circle_.getPosition().x + newPos.x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  
  // Borda superior
  if (bounds.position.y <= 0.f) {
    newPos.y = -bounds.position.y + circle_.getPosition().y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  // Borda inferior
  else if (bounds.position.y + bounds.size.y >= static_cast<float>(windowSize.y)) {
    newPos.y = static_cast<float>(windowSize.y) - bounds.size.y - circle_.getPosition().y + newPos.y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  
  // Aplica nova posição
  circle_.setPosition(newPos);
  
  // Atualiza posição do texto se houve colisão ou mudança de posição
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

void CircleShape::updateText() {
  text_.setFont(font_);
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  
  // Centraliza o texto no círculo
  const sf::Vector2f textPos = calculateTextCenterPosition(text_);
  text_.setPosition(textPos);
}

} // namespace bouncer
