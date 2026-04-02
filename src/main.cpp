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
 * @brief Função auxiliar para converter sf::Color em ImVec4
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
 * @brief Função auxiliar para converter ImVec4 em sf::Color
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
 * @brief Renderiza a interface ImGui para edição de formas
 */
void renderImGuiInterface(
  std::vector<std::unique_ptr<bouncer::Shape>>& shapes,
  int& selectedShapeIndex)
{
  ImGui::Begin("Shape Editor");
  
  if (shapes.empty()) {
    ImGui::Text("Nenhuma forma carregada");
    ImGui::End();
    return;
  }
  
  // Lista de formas
  ImGui::Text("Formas (%zu):", shapes.size());
  ImGui::Separator();
  
  for (size_t i = 0; i < shapes.size(); ++i) {
    const std::string label = shapes[i]->getName() + "##" + std::to_string(i);
    
    if (ImGui::Selectable(label.c_str(), selectedShapeIndex == static_cast<int>(i))) {
      selectedShapeIndex = static_cast<int>(i);
    }
  }
  
  ImGui::Separator();
  
  // Editor de propriedades da forma selecionada
  if (selectedShapeIndex >= 0 && selectedShapeIndex < static_cast<int>(shapes.size())) {
    bouncer::Shape& shape = *shapes[selectedShapeIndex];
    
    ImGui::Text("Propriedades: %s", shape.getName().c_str());
    ImGui::Separator();
    
    // Visibilidade
    bool visible = shape.isVisible();
    if (ImGui::Checkbox("Visivel", &visible)) {
      shape.setVisible(visible);
    }
    
    // Nome
    char nameBuffer[64];
    std::strncpy(nameBuffer, shape.getName().c_str(), sizeof(nameBuffer) - 1);
    nameBuffer[sizeof(nameBuffer) - 1] = '\0';
    
    if (ImGui::InputText("Nome", nameBuffer, sizeof(nameBuffer))) {
      shape.setName(std::string(nameBuffer));
    }
    
    // Escala (0 a 4)
    float scale = shape.getUI_Scale();
    if (ImGui::SliderFloat("Escala", &scale, 0.f, 4.f, "%.2f")) {
      shape.setUI_Scale(scale);
    }
    
    // Velocidade X (-8 a 8)
    sf::Vector2f velocity = shape.getVelocity();
    float velX = velocity.x;
    if (ImGui::SliderFloat("Velocidade X", &velX, -8.f, 8.f, "%.2f")) {
      shape.setVelocity(sf::Vector2f(velX, velocity.y));
    }
    
    // Velocidade Y (-8 a 8)
    float velY = velocity.y;
    if (ImGui::SliderFloat("Velocidade Y", &velY, -8.f, 8.f, "%.2f")) {
      shape.setVelocity(sf::Vector2f(velocity.x, velY));
    }
    
    // Cor da forma
    sf::Color shapeColor = shape.getColor();
    ImVec4 colorVec = colorToImVec4(shapeColor);
    if (ImGui::ColorEdit3("Cor da Forma", &colorVec.x)) {
      shape.setColor(imVec4ToColor(colorVec));
    }
    
    // Cor do texto
    sf::Color textColor = shape.getTextColor();
    ImVec4 textColorVec = colorToImVec4(textColor);
    if (ImGui::ColorEdit3("Cor do Texto", &textColorVec.x)) {
      shape.setTextColor(imVec4ToColor(textColorVec));
    }
  }
  
  ImGui::End();
}

int main(int argc, char* argv[]) {
  std::cout << "=== SFML Shape Bouncer ===" << std::endl;
  std::cout << "Argumentos: " << argc << std::endl;
  
  // Determina o caminho do arquivo de configuração
  std::string configFilePath = CONFIG_FILE_PATH;
  
  if (argc > 1 && std::string(argv[1]) == "--config_file_path" && argc > 2) {
    configFilePath = std::string(argv[2]) + CONFIG_FILE_PATH;
  }
  
  std::cout << "Arquivo de configuracao: " << configFilePath << std::endl;
  
  // Carrega configurações
  bouncer::ConfigManager configManager;
  
  if (!configManager.loadFromFile(configFilePath)) {
    std::cerr << "Erro ao carregar arquivo de configuração" << std::endl;
    return EXIT_FAILURE;
  }
  
  if (!configManager.isFontLoaded()) {
    std::cerr << "Erro: Fonte não carregada" << std::endl;
    return EXIT_FAILURE;
  }
  
  // Carrega formas
  std::vector<std::unique_ptr<bouncer::Shape>> shapes = 
    bouncer::ShapeLoader::loadShapes(configManager, configFilePath);
  
  std::cout << "Formas carregadas: " << shapes.size() << std::endl;
  
  if (shapes.empty()) {
    std::cerr << "Aviso: Nenhuma forma foi carregada" << std::endl;
  }
  
  // Configura janela SFML
  const bouncer::WindowConfig& windowConfig = configManager.getWindowConfig();
  sf::RenderWindow window(
    sf::VideoMode({windowConfig.width, windowConfig.height}),
    "SFML Shape Bouncer"
  );
  window.setFramerateLimit(60);
  
  // Inicializa ImGui
  ImGui::SFML::Init(window);
  sf::Clock deltaClock;
  
  // Índice da forma selecionada
  int selectedShapeIndex = shapes.empty() ? -1 : 0;
  
  // Loop principal
  while (window.isOpen()) {
    // Processa eventos
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);
      
      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }
    
    // Atualiza ImGui
    ImGui::SFML::Update(window, deltaClock.restart());
    
    // Atualiza formas
    const float deltaTime = deltaClock.getElapsedTime().asSeconds();
    const sf::Vector2u windowSize = window.getSize();
    
    for (auto& shape : shapes) {
      shape->update(deltaTime, windowSize);
    }
    
    // Renderiza interface ImGui
    renderImGuiInterface(shapes, selectedShapeIndex);
    
    // Desenha
    window.clear(sf::Color::Black);
    
    for (const auto& shape : shapes) {
      shape->draw(window);
    }
    
    ImGui::SFML::Render(window);
    window.display();
  }
  
  // Limpeza
  ImGui::SFML::Shutdown();
  
  std::cout << "Programa finalizado com sucesso" << std::endl;
  return EXIT_SUCCESS;
}