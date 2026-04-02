#include <imgui-SFML.h>
#include <imgui.h>

#include <SFML/Graphics.hpp>

#include "config_manager.hpp"
#include "shape_loader.hpp"
#include "shape.hpp"

#include <algorithm>
#include <cstring>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

namespace {
  constexpr const char* CONFIG_FILE_PATH = "config.txt";
}

/**
 * @brief Helper function to convert sf::Color to ImVec4
 * @param color SFML color object
 * @return ImVec4 color with normalized RGB values (0.0-1.0)
 */
ImVec4 colorToImVec4(const sf::Color& color) {
  return ImVec4(
    static_cast<float>(color.r) / 255.f,
    static_cast<float>(color.g) / 255.f,
    static_cast<float>(color.b) / 255.f,
    static_cast<float>(color.a) / 255.f
  );
}

/**
 * @brief Helper function to convert ImVec4 to sf::Color
 * @param color ImGui color vector with values in range 0.0-1.0
 * @return SFML color object with RGB values (alpha set to 255)
 */
sf::Color imVec4ToColor(const ImVec4& color) {
  return sf::Color(
    static_cast<unsigned char>(std::clamp(color.x, 0.f, 1.f) * 255.f),
    static_cast<unsigned char>(std::clamp(color.y, 0.f, 1.f) * 255.f),
    static_cast<unsigned char>(std::clamp(color.z, 0.f, 1.f) * 255.f),
    255u
  );
}

/**
 * @brief Renders the ImGui interface for shape editing
 * @param shapes Vector of unique pointers to shapes
 * @param selectedShapeIndex Index of currently selected shape (-1 if none)
 */
void renderImGuiInterface(
  std::vector<std::unique_ptr<bouncer::Shape>>& shapes,
  int& selectedShapeIndex)
{
  ImGui::Begin("Shape Editor");
  
  if (shapes.empty()) {
    ImGui::Text("No shapes loaded");
    ImGui::End();
    return;
  }
  
  // Shape list
  ImGui::Text("Shapes (%zu):", shapes.size());
  ImGui::Separator();
  
  for (size_t i = 0; i < shapes.size(); ++i) {
    const std::string label = shapes[i]->getName() + "##" + std::to_string(i);
    
    if (ImGui::Selectable(label.c_str(), selectedShapeIndex == static_cast<int>(i))) {
      selectedShapeIndex = static_cast<int>(i);
    }
  }
  
  ImGui::Separator();
  
  // Properties editor for selected shape
  if (selectedShapeIndex >= 0 && selectedShapeIndex < static_cast<int>(shapes.size())) {
    bouncer::Shape& shape = *shapes[selectedShapeIndex];
    
    ImGui::Text("Properties: %s", shape.getName().c_str());
    ImGui::Separator();
    
    // Visibility toggle
    bool visible = shape.isVisible();
    if (ImGui::Checkbox("Visible", &visible)) {
      shape.setVisible(visible);
    }
    
    // Name input
    char nameBuffer[64];
    std::strncpy(nameBuffer, shape.getName().c_str(), sizeof(nameBuffer) - 1);
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    
    if (ImGui::InputText("Name", nameBuffer, sizeof(nameBuffer))) {
      shape.setName(std::string(nameBuffer));
    }
    
    // Scale slider (0 to 4)
    float scale = shape.getUI_Scale();
    if (ImGui::SliderFloat("Scale", &scale, 0.f, 4.f, "%.2f")) {
      shape.setUI_Scale(scale);
    }
    
    // Velocity X slider (-8 to 8)
    sf::Vector2f velocity = shape.getVelocity();
    float velX = velocity.x;
    if (ImGui::SliderFloat("Velocity X", &velX, -8.f, 8.f, "%.2f")) {
      shape.setVelocity(sf::Vector2f(velX, velocity.y));
    }
    
    // Velocity Y slider (-8 to 8)
    float velY = velocity.y;
    if (ImGui::SliderFloat("Velocity Y", &velY, -8.f, 8.f, "%.2f")) {
      shape.setVelocity(sf::Vector2f(velocity.x, velY));
    }
    
    // Shape color picker
    sf::Color shapeColor = shape.getColor();
    ImVec4 colorVec = colorToImVec4(shapeColor);
    if (ImGui::ColorEdit3("Shape Color", &colorVec.x)) {
      shape.setColor(imVec4ToColor(colorVec));
    }
    
    // Text color picker
    sf::Color textColor = shape.getTextColor();
    ImVec4 textColorVec = colorToImVec4(textColor);
    if (ImGui::ColorEdit3("Text Color", &textColorVec.x)) {
      shape.setTextColor(imVec4ToColor(textColorVec));
    }
  }
  
  ImGui::End();
}

/**
 * @brief Main entry point of the application
 * @param argc Number of command-line arguments
 * @param argv Array of command-line argument strings
 * @return EXIT_SUCCESS on success, EXIT_FAILURE on error
 */
int main(int argc, char* argv[]) {
  std::cout << "=== SFML Shape Bouncer ===" << std::endl;
  std::cout << "Arguments: " << argc << std::endl;
  
  // Determine configuration file path
  std::string configFilePath = CONFIG_FILE_PATH;
  
  if (argc > 1 && std::string(argv[1]) == "--config_file_path" && argc > 2) {
    configFilePath = std::string(argv[2]) + CONFIG_FILE_PATH;
  }
  
  std::cout << "Configuration file: " << configFilePath << std::endl;
  
  // Load configuration settings
  bouncer::ConfigManager configManager;
  
  if (!configManager.loadFromFile(configFilePath)) {
    std::cerr << "Error loading configuration file" << std::endl;
    return EXIT_FAILURE;
  }
  
  if (!configManager.isFontLoaded()) {
    std::cerr << "Error: Font not loaded" << std::endl;
    return EXIT_FAILURE;
  }
  
  // Load shapes from configuration
  std::vector<std::unique_ptr<bouncer::Shape>> shapes = 
    bouncer::ShapeLoader::loadShapes(configManager, configFilePath);
  
  std::cout << "Shapes loaded: " << shapes.size() << std::endl;
  
  if (shapes.empty()) {
    std::cerr << "Warning: No shapes were loaded" << std::endl;
  }
  
  // Setup SFML window
  const bouncer::WindowConfig& windowConfig = configManager.getWindowConfig();
  sf::RenderWindow window(
    sf::VideoMode({windowConfig.width, windowConfig.height}),
    "SFML Shape Bouncer"
  );
  window.setFramerateLimit(60);
  
  // Initialize ImGui
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;
  
  // Selected shape index
  int selectedShapeIndex = shapes.empty() ? -1 : 0;
  
  // Main application loop
  while (window.isOpen()) {
    // Process SFML events
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    
    // Update ImGui
    ImGui::SFML::Update(window, deltaClock.restart());
    
    // Update shapes with delta time and window size
    const float deltaTime = deltaClock.getElapsedTime().asSeconds();
    const sf::Vector2u windowSize = window.getSize();
    
    for (auto& shape : shapes) {
      shape->update(deltaTime, windowSize);
    }
    
    // Render ImGui interface
    renderImGuiInterface(shapes, selectedShapeIndex);
    
    // Draw frame
    window.clear(sf::Color::Black);
    
    for (const auto& shape : shapes) {
      shape->draw(window);
    }
    
    ImGui::SFML::Render(window);
    window.display();
  }
  
  // Cleanup
  ImGui::SFML::Shutdown();
  
  std::cout << "Program finished successfully" << std::endl;
  return EXIT_SUCCESS;
}