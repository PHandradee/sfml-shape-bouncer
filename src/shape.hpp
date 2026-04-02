#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <tuple>

namespace bouncer {

/**
 * @brief Structure containing all shape properties not stored in sf::Shape itself
 */
struct ShapeProperties {
  std::string name{};           ///< Shape name
  sf::Vector2f velocity{0.f, 0.f};  ///< Velocity (X, Y)
  sf::Color textColor{sf::Color::White};  ///< Text color
  unsigned int fontSize{16u};   ///< Font size
  bool visible{true};           ///< Shape visibility
  float scale{1.f};             ///< Shape scale (0 to 4)
};

/**
 * @brief Base class for all geometric shapes
 */
class Shape {
public:
  virtual ~Shape() = default;
  
  /**
   * @brief Draws the shape on the render window
   * @param window The SFML render window to draw on
   */
  virtual void draw(sf::RenderWindow& window) const = 0;
  
  /**
   * @brief Updates the shape's position and handles boundary collisions
   * @param deltaTime Time elapsed since last update in seconds
   * @param windowSize Current window dimensions
   */
  virtual void update(float deltaTime, const sf::Vector2u& windowSize) = 0;
  
  /**
   * @brief Gets the global bounding rectangle of the shape
   * @return SFML floating-point rectangle representing bounds
   */
  virtual sf::FloatRect getGlobalBounds() const = 0;
  
  /**
   * @brief Gets the current position of the shape
   * @return 2D vector with X and Y coordinates
   */
  virtual sf::Vector2f getPosition() const = 0;
  
  /**
   * @brief Sets the position of the shape
   * @param position New 2D position vector
   */
  virtual void setPosition(const sf::Vector2f& position) = 0;
  
  /**
   * @brief Sets the scale factor of the shape
   * @param scale Scale multiplier
   */
  virtual void setScale(float scale) = 0;
  
  /**
   * @brief Gets the current scale factor of the shape
   * @return Current scale value
   */
  virtual float getScale() const = 0;
  
  // Getters and setters for properties
  [[nodiscard]] const std::string& getName() const noexcept { return properties_.name; }
  void setName(const std::string& name) { properties_.name = name; }
  
  [[nodiscard]] sf::Vector2f getVelocity() const noexcept { return properties_.velocity; }
  void setVelocity(const sf::Vector2f& velocity) { properties_.velocity = velocity; }
  
  /**
   * @brief Gets the fill color of the shape
   * @return SFML color object
   */
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
   * @brief Calculates centered position for text within the shape
   * @param text The text object to center
   * @return 2D vector with centered position coordinates
   */
  [[nodiscard]] sf::Vector2f calculateTextCenterPosition(
    const sf::Text& text) const;
};

} // namespace bouncer
