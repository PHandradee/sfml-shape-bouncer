# SFML Shape Bouncer - Project Documentation

## Overview

SFML Shape Bouncer is a C++ application that creates an interactive simulation of bouncing geometric shapes (circles and rectangles) within a window. The application features real-time physics simulation, collision detection with window boundaries, and an ImGui-based editor for modifying shape properties at runtime.

## Architecture

The project follows an object-oriented design pattern with a base `Shape` class and derived classes for specific geometric forms. Configuration is loaded from text files, and the rendering is handled by SFML (Simple and Fast Multimedia Library) with Dear ImGui for the user interface.

---

## Classes and Structures

### Namespace: `bouncer`

All core classes are encapsulated within the `bouncer` namespace.

---

## Core Classes

### 1. `ShapeProperties` (Structure)

**File:** `shape.hpp`

A structure containing all properties of a shape that are not stored in the SFML shape itself.

#### Members:
- `std::string name` - Name identifier of the shape
- `sf::Vector2f velocity` - Velocity vector (X, Y components)
- `sf::Color textColor` - Color of the displayed text
- `unsigned int fontSize` - Font size for the shape's label
- `bool visible` - Visibility flag
- `float scale` - Scale factor (range: 0 to 4)

---

### 2. `Shape` (Abstract Base Class)

**File:** `shape.hpp`, `shape.cpp`

Base class for all geometric shapes. Defines the common interface for all shape types.

#### Public Methods:

**Constructor/Destructor:**
- `virtual ~Shape() = default` - Virtual destructor for proper cleanup of derived classes

**Pure Virtual Methods (Interface):**
- `virtual void draw(sf::RenderWindow& window) const = 0` - Renders the shape to the window
- `virtual void update(float deltaTime, const sf::Vector2u& windowSize) = 0` - Updates shape position and handles collisions
- `virtual sf::FloatRect getGlobalBounds() const = 0` - Returns the bounding rectangle of the shape
- `virtual sf::Vector2f getPosition() const = 0` - Gets the current position
- `virtual void setPosition(const sf::Vector2f& position) = 0` - Sets the position
- `virtual void setScale(float scale) = 0` - Sets the scale factor
- `virtual float getScale() const = 0` - Gets the current scale factor

**Property Getters/Setters:**
- `const std::string& getName() const noexcept` - Returns the shape's name
- `void setName(const std::string& name)` - Sets the shape's name
- `sf::Vector2f getVelocity() const noexcept` - Returns the velocity vector
- `void setVelocity(const sf::Vector2f& velocity)` - Sets the velocity vector
- `sf::Color getColor() const` - Returns the shape's fill color (implemented in derived classes)
- `void setColor(const sf::Color& color)` - Sets the shape's fill color (implemented in derived classes)
- `sf::Color getTextColor() const noexcept` - Returns the text color
- `void setTextColor(const sf::Color& color)` - Sets the text color
- `unsigned int getFontSize() const noexcept` - Returns the font size
- `void setFontSize(unsigned int size)` - Sets the font size
- `bool isVisible() const noexcept` - Returns visibility status
- `void setVisible(bool visible)` - Sets visibility
- `float getUI_Scale() const noexcept` - Returns the UI scale value
- `void setUI_Scale(float scale)` - Sets scale with clamping (0 to 4)

#### Protected Methods:
- `sf::Vector2f calculateTextCenterPosition(const sf::Text& text) const` - Calculates centered position for text within the shape

#### Protected Members:
- `ShapeProperties properties_` - Contains all shape properties

---

### 3. `CircleShape` (Class)

**File:** `circle_shape.hpp`, `circle_shape.cpp`

Represents a circle with customizable properties. Inherits from `Shape`.

#### Constructor:
```cpp
CircleShape(
    const std::string& name,
    const sf::Vector2f& position,
    const sf::Vector2f& velocity,
    const sf::Color& color,
    float radius,
    const sf::Font& font,
    unsigned int fontSize = 16u
)
```

**Parameters:**
- `name` - Display name of the circle
- `position` - Initial position (x, y coordinates)
- `velocity` - Initial velocity vector
- `color` - Fill color of the circle
- `radius` - Circle radius
- `font` - Reference to the font for text rendering
- `fontSize` - Font size (default: 16)

#### Public Methods:

**Override Methods:**
- `void draw(sf::RenderWindow& window) const override` - Draws the circle and its label
- `void update(float deltaTime, const sf::Vector2u& windowSize) override` - Updates position and handles boundary collisions
- `sf::FloatRect getGlobalBounds() const override` - Returns circle's bounding box
- `sf::Vector2f getPosition() const override` - Gets circle center position
- `void setPosition(const sf::Vector2f& position) override` - Sets circle position
- `void setScale(float scale) override` - Applies scale transformation (clamped 0.1 to 4)
- `float getScale() const override` - Returns current scale factor

**Circle-Specific Methods:**
- `float getRadius() const noexcept` - Returns the circle radius
- `void setRadius(float radius)` - Sets the circle radius

#### Private Members:
- `sf::CircleShape circle_` - SFML circle primitive
- `sf::Text text_` - Text label
- `const sf::Font& font_` - Font reference
- `float radius_` - Circle radius value

#### Private Methods:
- `void updateText()` - Updates text position and appearance to center it on the circle

---

### 4. `RectangleShape` (Class)

**File:** `rectangle_shape.hpp`, `rectangle_shape.cpp`

Represents a rectangle with customizable properties. Inherits from `Shape`.

#### Constructor:
```cpp
RectangleShape(
    const std::string& name,
    const sf::Vector2f& position,
    const sf::Vector2f& velocity,
    const sf::Color& color,
    float width,
    float height,
    const sf::Font& font,
    unsigned int fontSize = 16u
)
```

**Parameters:**
- `name` - Display name of the rectangle
- `position` - Initial position (x, y coordinates)
- `velocity` - Initial velocity vector
- `color` - Fill color of the rectangle
- `width` - Rectangle width
- `height` - Rectangle height
- `font` - Reference to the font for text rendering
- `fontSize` - Font size (default: 16)

#### Public Methods:

**Override Methods:**
- `void draw(sf::RenderWindow& window) const override` - Draws the rectangle and its label
- `void update(float deltaTime, const sf::Vector2u& windowSize) override` - Updates position and handles boundary collisions
- `sf::FloatRect getGlobalBounds() const override` - Returns rectangle's bounding box
- `sf::Vector2f getPosition() const override` - Gets rectangle position
- `void setPosition(const sf::Vector2f& position) override` - Sets rectangle position
- `void setScale(float scale) override` - Applies scale transformation (clamped 0.1 to 4)
- `float getScale() const override` - Returns current scale factor

**Rectangle-Specific Methods:**
- `sf::Vector2f getSize() const noexcept` - Returns the rectangle dimensions
- `void setSize(const sf::Vector2f& size)` - Sets the rectangle dimensions

#### Private Members:
- `sf::RectangleShape rectangle_` - SFML rectangle primitive
- `sf::Text text_` - Text label
- `const sf::Font& font_` - Font reference
- `sf::Vector2f size_` - Dimensions (width, height)

#### Private Methods:
- `void updateText()` - Updates text position and appearance to center it on the rectangle

---

### 5. `WindowConfig` (Structure)

**File:** `config_manager.hpp`

Stores window configuration parameters.

#### Members:
- `unsigned int width` - Window width (default: 800)
- `unsigned int height` - Window height (default: 600)

---

### 6. `FontConfig` (Structure)

**File:** `config_manager.hpp`

Stores font configuration parameters.

#### Members:
- `std::string filePath` - Path to the font file
- `unsigned int size` - Font size (default: 16)
- `sf::Color color` - Default text color (default: White)

---

### 7. `ConfigManager` (Class)

**File:** `config_manager.hpp`, `config_manager.cpp`

Manages configuration loading and parsing from text files.

#### Constructor:
- `ConfigManager() = default` - Default constructor

#### Public Methods:
- `bool loadFromFile(const std::string& filePath)` - Loads configuration from a text file
  - **Returns:** `true` if successful, `false` otherwise
  - **Parameter:** `filePath` - Path to the configuration file

- `const WindowConfig& getWindowConfig() const noexcept` - Returns window configuration
- `const FontConfig& getFontConfig() const noexcept` - Returns font configuration
- `const sf::Font& getFont() const noexcept` - Returns the loaded font
- `bool isFontLoaded() const noexcept` - Checks if font was loaded successfully

#### Private Members:
- `WindowConfig windowConfig_` - Window settings
- `FontConfig fontConfig_` - Font settings
- `sf::Font font_` - Loaded SFML font
- `bool fontLoaded_` - Font loading status flag

#### Private Methods:
- `void parseLine(const std::string& line)` - Parses a single configuration line
- `void parseWindow(const std::vector<std::string>& tokens)` - Parses Window configuration lines
- `void parseFont(const std::vector<std::string>& tokens)` - Parses Font configuration lines
- `static std::vector<std::string> tokenize(const std::string& line)` - Splits a line into whitespace-separated tokens

---

### 8. `ShapeLoader` (Class)

**File:** `shape_loader.hpp`, `shape_loader.cpp`

Loads and creates shape instances from configuration files.

#### Public Static Methods:
```cpp
static std::vector<std::unique_ptr<Shape>> loadShapes(
    const ConfigManager& configManager,
    const std::string& filePath
)
```
- Loads shapes from configuration file
- **Parameters:**
  - `configManager` - Pre-loaded configuration manager
  - `filePath` - Path to configuration file
- **Returns:** Vector of unique pointers to Shape objects

#### Private Static Methods:
```cpp
static std::unique_ptr<Shape> parseCircle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize
)
```
- Parses Circle configuration line
- **Expected format:** `Circle Name X Y SX SY R G B Radius`
- **Returns:** Unique pointer to CircleShape or nullptr on error

```cpp
static std::unique_ptr<Shape> parseRectangle(
    const std::vector<std::string>& tokens,
    const sf::Font& font,
    unsigned int fontSize
)
```
- Parses Rectangle configuration line
- **Expected format:** `Rectangle Name X Y SX SY R G B Width Height`
- **Returns:** Unique pointer to RectangleShape or nullptr on error

```cpp
static std::vector<std::string> tokenize(const std::string& line)
```
- Splits a configuration line into tokens

---

## Main Application Functions

### Helper Functions (Anonymous Namespace)

**File:** `main.cpp`

#### `colorToImVec4`
```cpp
ImVec4 colorToImVec4(const sf::Color& color)
```
Converts SFML color to ImGui color format.
- **Parameters:** `color` - SFML color (RGBA 0-255)
- **Returns:** ImVec4 with normalized components (0.0-1.0)

#### `imVec4ToColor`
```cpp
sf::Color imVec4ToColor(const ImVec4& color)
```
Converts ImGui color to SFML color format.
- **Parameters:** `color` - ImGui color (normalized 0.0-1.0)
- **Returns:** SFML color with integer components (0-255)

#### `renderImGuiInterface`
```cpp
void renderImGuiInterface(
    std::vector<std::unique_ptr<bouncer::Shape>>& shapes,
    int& selectedShapeIndex
)
```
Renders the ImGui-based shape editor interface.

**Features:**
- Lists all loaded shapes
- Allows shape selection
- Editable properties for selected shape:
  - Visibility toggle
  - Name editing
  - Scale slider (0 to 4)
  - Velocity X slider (-8 to 8)
  - Velocity Y slider (-8 to 8)
  - Shape color picker
  - Text color picker

### `main` Function

**File:** `main.cpp`

```cpp
int main(int argc, char* argv[])
```

**Command-line Arguments:**
- `--config_file_path <path>` - Specifies custom configuration file path

**Execution Flow:**
1. Parses command-line arguments
2. Loads configuration via `ConfigManager`
3. Loads shapes via `ShapeLoader`
4. Creates SFML render window
5. Initializes ImGui
6. Enters main loop:
   - Processes events
   - Updates ImGui
   - Updates all shapes (physics simulation)
   - Renders ImGui interface
   - Draws all shapes
   - Presents frame
7. Cleans up resources on exit

---

## Configuration File Format

The application reads configuration from a text file (default: `config.txt`).

### Syntax:
- Lines starting with `#` are comments
- Empty lines are ignored
- Tokens are whitespace-separated

### Supported Directives:

#### Window Configuration
```
Window <width> <height>
```
Example: `Window 800 600`

#### Font Configuration
```
Font <filepath> <size> <R> <G> <B>
```
Example: `Font assets/Arial.ttf 16 255 255 255`

#### Circle Shape
```
Circle <name> <x> <y> <velX> <velY> <R> <G> <B> <radius>
```
Example: `Circle Ball1 100 100 2 3 255 0 0 50`

#### Rectangle Shape
```
Rectangle <name> <x> <y> <velX> <velY> <R> <G> <B> <width> <height>
```
Example: `Rectangle Box1 200 200 -1 2 0 255 0 100 50`

---

## Build System

### CMake Configuration

The project uses CMake with the following features:
- **Testing:** Enabled via CTest (`enable_testing()`)
- **Test Framework:** doctest (compatible with CTest)
- **Dependencies:**
  - SFML (Simple and Fast Multimedia Library)
  - Dear ImGui
  - imgui-SFML binding

### Building Instructions

```bash
mkdir build && cd build
cmake ..
cmake --build .
ctest          # Run tests
ctest --verbose  # Run tests with detailed output
```

---

## Dependencies

1. **SFML** - Graphics, window management, and multimedia
2. **Dear ImGui** - Immediate mode GUI library
3. **imgui-SFML** - SFML bindings for Dear ImGui
4. **doctest** - Header-only testing framework

---

## Key Features

1. **Physics Simulation:** Shapes bounce off window boundaries with velocity inversion
2. **Real-time Editing:** Modify shape properties during runtime via ImGui
3. **Configuration-driven:** Load initial state from text files
4. **Extensible Design:** Easy to add new shape types via inheritance
5. **Type Safety:** Uses `std::unique_ptr` for memory management
6. **Testing Support:** Integrated unit tests with CTest

---

## Design Patterns Used

1. **Strategy Pattern:** Different shape types implement common interface
2. **Factory Pattern:** `ShapeLoader` creates appropriate shape instances
3. **Singleton-like Behavior:** `ConfigManager` provides centralized configuration access
4. **RAII:** Smart pointers ensure proper resource cleanup

---

## Thread Safety

This application is single-threaded. No thread synchronization mechanisms are implemented.

---

## Error Handling

- File loading failures are logged to `stderr`
- Invalid configuration lines are skipped with error messages
- Exception handling for numeric conversions
- Graceful degradation when fonts or shapes fail to load

---

## Performance Considerations

- Frame rate limited to 60 FPS
- Delta time-based movement for consistent speed across different frame rates
- Efficient rendering through SFML's batch drawing
- Minimal dynamic allocations after initialization
