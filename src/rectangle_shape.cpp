#include "rectangle_shape.hpp"
#include <algorithm>

namespace bouncer {

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
  // Configura propriedades
  properties_.name = name;
  properties_.velocity = velocity;
  properties_.fontSize = fontSize;
  properties_.textColor = sf::Color::White;
  properties_.scale = 1.f;
  
  // Configura o retângulo
  rectangle_.setSize(size_);
  rectangle_.setFillColor(color);
  rectangle_.setPosition(position);
  
  // Configura o texto
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  updateText();
}

void RectangleShape::draw(sf::RenderWindow& window) const {
  if (!properties_.visible) return;
  
  window.draw(rectangle_);
  window.draw(text_);
}

void RectangleShape::update(float deltaTime, const sf::Vector2u& windowSize) {
  if (!properties_.visible) return;
  
  // Atualiza posição baseada na velocidade
  sf::Vector2f newPos = rectangle_.getPosition();
  newPos.x += properties_.velocity.x * deltaTime * 60.f; // Normalizado para 60 FPS
  newPos.y += properties_.velocity.y * deltaTime * 60.f;
  
  // Obtém bounds da forma
  const sf::FloatRect bounds = getGlobalBounds();
  
  // Verifica colisão com as bordas e inverte velocidade se necessário
  // Borda esquerda
  if (bounds.position.x <= 0.f) {
    newPos.x = -bounds.position.x + rectangle_.getPosition().x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  // Borda direita
  else if (bounds.position.x + bounds.size.x >= static_cast<float>(windowSize.x)) {
    newPos.x = static_cast<float>(windowSize.x) - bounds.size.x - rectangle_.getPosition().x + newPos.x;
    properties_.velocity.x = -properties_.velocity.x;
  }
  
  // Borda superior
  if (bounds.position.y <= 0.f) {
    newPos.y = -bounds.position.y + rectangle_.getPosition().y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  // Borda inferior
  else if (bounds.position.y + bounds.size.y >= static_cast<float>(windowSize.y)) {
    newPos.y = static_cast<float>(windowSize.y) - bounds.size.y - rectangle_.getPosition().y + newPos.y;
    properties_.velocity.y = -properties_.velocity.y;
  }
  
  // Aplica nova posição
  rectangle_.setPosition(newPos);
  
  // Atualiza posição do texto se houve colisão ou mudança de posição
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

void RectangleShape::updateText() {
  text_.setFont(font_);
  text_.setString(properties_.name);
  text_.setCharacterSize(properties_.fontSize);
  text_.setFillColor(properties_.textColor);
  
  // Centraliza o texto no retângulo
  const sf::Vector2f textPos = calculateTextCenterPosition(text_);
  text_.setPosition(textPos);
}

} // namespace bouncer
