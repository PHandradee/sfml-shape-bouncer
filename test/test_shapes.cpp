#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include <doctest/doctest.h>

#include <SFML/Graphics.hpp>
#include <memory>
#include <string>
#include <vector>

// Forward declarations para testes
namespace bouncer {
  class Shape;
  class CircleShape;
  class RectangleShape;
  class ConfigManager;
  class ShapeLoader;
}

#include "../src/shape.hpp"
#include "../src/circle_shape.hpp"
#include "../src/rectangle_shape.hpp"
#include "../src/config_manager.hpp"
#include "../src/shape_loader.hpp"

using namespace bouncer;

// ============================================================================
// TESTES UNITÁRIOS - CIRCLE SHAPE
// ============================================================================

TEST_SUITE("CircleShape") {
  
  TEST_CASE("Criação de círculo com parâmetros válidos") {
    sf::Vector2f position{100.f, 150.f};
    sf::Color color{sf::Color::Red};
    std::string name{"TestCircle"};
    sf::Vector2f velocity{50.f, -30.f};
    float radius{25.f};
    
    // Usar font padrão do SFML para testes
    sf::Font font;
    
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    CHECK(circle->getName() == name);
    CHECK(circle->getPosition().x == doctest::Approx(position.x));
    CHECK(circle->getPosition().y == doctest::Approx(position.y));
    CHECK(circle->getVelocity().x == doctest::Approx(velocity.x));
    CHECK(circle->getVelocity().y == doctest::Approx(velocity.y));
    CHECK(circle->isVisible() == true);
    CHECK(circle->getScale() == doctest::Approx(1.0f));
  }

  TEST_CASE("Círculo atualiza posição corretamente") {
    sf::Vector2f position{50.f, 50.f};
    sf::Color color{sf::Color::Blue};
    std::string name{"MovingCircle"};
    sf::Vector2f velocity{100.f, 50.f};  // 100 px/s na X, 50 px/s na Y
    float radius{20.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    // Atualizar por 0.5 segundos
    circle->update(0.5f, sf::Vector2u{800, 600});
    
    sf::Vector2f newPosition = circle->getPosition();
    CHECK(newPosition.x == doctest::Approx(100.f).epsilon(0.1f));  // 50 + 100*0.5
    CHECK(newPosition.y == doctest::Approx(75.f).epsilon(0.1f));   // 50 + 50*0.5
  }

  TEST_CASE("Círculo colide com borda direita") {
    sf::Vector2f position{750.f, 300.f};  // Perto da borda direita (800px)
    sf::Color color{sf::Color::Green};
    std::string name{"BounceCircle"};
    sf::Vector2f velocity{100.f, 0.f};  // Movendo para direita
    float radius{20.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    float initialVelX = circle->getVelocity().x;
    
    // Atualizar - deve colidir e inverter velocidade
    circle->update(0.1f, sf::Vector2u{800, 600});
    
    // Velocidade deve ter invertido na direção X
    CHECK(circle->getVelocity().x < 0);
    CHECK(circle->getVelocity().y == doctest::Approx(0.f));
  }

  TEST_CASE("Círculo colide com borda inferior") {
    sf::Vector2f position{400.f, 580.f};  // Perto da borda inferior (600px)
    sf::Color color{sf::Color::Yellow};
    std::string name{"BottomCircle"};
    sf::Vector2f velocity{0.f, 100.f};  // Movendo para baixo
    float radius{15.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    circle->update(0.1f, sf::Vector2u{800, 600});
    
    // Velocidade deve ter invertido na direção Y
    CHECK(circle->getVelocity().y < 0);
  }

  TEST_CASE("Círculo mantém visibilidade") {
    sf::Vector2f position{100.f, 100.f};
    sf::Color color{sf::Color::Cyan};
    std::string name{"VisibleCircle"};
    sf::Vector2f velocity{0.f, 0.f};
    float radius{30.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    circle->setVisible(false);
    CHECK(circle->isVisible() == false);
    
    circle->setVisible(true);
    CHECK(circle->isVisible() == true);
  }

  TEST_CASE("Círculo ajusta escala corretamente") {
    sf::Vector2f position{100.f, 100.f};
    sf::Color color{sf::Color::Magenta};
    std::string name{"ScaledCircle"};
    sf::Vector2f velocity{0.f, 0.f};
    float radius{25.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    circle->setUI_Scale(2.0f);
    CHECK(circle->getUI_Scale() == doctest::Approx(2.0f));
    CHECK(circle->getScale() == doctest::Approx(2.0f));
    
    // Testar limite máximo (4.0)
    circle->setUI_Scale(5.0f);
    CHECK(circle->getUI_Scale() == doctest::Approx(4.0f));
    
    // Testar limite mínimo (0.1)
    circle->setUI_Scale(0.0f);
    CHECK(circle->getUI_Scale() == doctest::Approx(0.1f));
  }

  TEST_CASE("Círculo retorna bounds corretos") {
    sf::Vector2f position{100.f, 100.f};
    sf::Color color{sf::Color::White};
    std::string name{"BoundsCircle"};
    sf::Vector2f velocity{0.f, 0.f};
    float radius{25.f};
    
    sf::Font font;
    auto circle = std::make_unique<CircleShape>(name, position, velocity, color, radius, font);
    
    sf::FloatRect bounds = circle->getGlobalBounds();
    CHECK(bounds.left > 0);
    CHECK(bounds.top > 0);
    CHECK(bounds.width > 0);
    CHECK(bounds.height > 0);
  }
}

// ============================================================================
// TESTES UNITÁRIOS - RECTANGLE SHAPE
// ============================================================================

TEST_SUITE("RectangleShape") {
  
  TEST_CASE("Criação de retângulo com parâmetros válidos") {
    sf::Vector2f position{200.f, 250.f};
    sf::Color color{sf::Color::Red};
    std::string name{"TestRect"};
    sf::Vector2f velocity{-50.f, 30.f};
    float width{80.f};
    float height{60.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    CHECK(rect->getName() == name);
    CHECK(rect->getPosition().x == doctest::Approx(position.x));
    CHECK(rect->getPosition().y == doctest::Approx(position.y));
    CHECK(rect->getVelocity().x == doctest::Approx(velocity.x));
    CHECK(rect->getVelocity().y == doctest::Approx(velocity.y));
  }

  TEST_CASE("Retângulo atualiza posição corretamente") {
    sf::Vector2f position{100.f, 100.f};
    sf::Color color{sf::Color::Blue};
    std::string name{"MovingRect"};
    sf::Vector2f velocity{200.f, -100.f};
    float width{50.f};
    float height{40.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    rect->update(0.5f, sf::Vector2u{800, 600});
    
    sf::Vector2f newPosition = rect->getPosition();
    CHECK(newPosition.x == doctest::Approx(200.f).epsilon(0.1f));  // 100 + 200*0.5
    CHECK(newPosition.y == doctest::Approx(50.f).epsilon(0.1f));   // 100 + (-100)*0.5
  }

  TEST_CASE("Retângulo colide com borda esquerda") {
    sf::Vector2f position{20.f, 300.f};  // Perto da borda esquerda
    sf::Color color{sf::Color::Green};
    std::string name{"LeftRect"};
    sf::Vector2f velocity{-100.f, 0.f};  // Movendo para esquerda
    float width{40.f};
    float height{30.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    rect->update(0.1f, sf::Vector2u{800, 600});
    
    CHECK(rect->getVelocity().x > 0);  // Deve ter invertido
  }

  TEST_CASE("Retângulo colide com borda superior") {
    sf::Vector2f position{400.f, 20.f};  // Perto da borda superior
    sf::Color color{sf::Color::Yellow};
    std::string name{"TopRect"};
    sf::Vector2f velocity{0.f, -100.f};  // Movendo para cima
    float width{60.f};
    float height{40.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    rect->update(0.1f, sf::Vector2u{800, 600});
    
    CHECK(rect->getVelocity().y > 0);  // Deve ter invertido
  }

  TEST_CASE("Retângulo ajusta cor corretamente") {
    sf::Vector2f position{100.f, 100.f};
    sf::Color color{sf::Color::Red};
    std::string name{"ColorRect"};
    sf::Vector2f velocity{0.f, 0.f};
    float width{70.f};
    float height{50.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    rect->setColor(sf::Color::Blue);
    CHECK(rect->getColor() == sf::Color::Blue);
  }

  TEST_CASE("Retângulo retorna bounds corretos") {
    sf::Vector2f position{150.f, 150.f};
    sf::Color color{sf::Color::White};
    std::string name{"BoundsRect"};
    sf::Vector2f velocity{0.f, 0.f};
    float width{60.f};
    float height{40.f};
    
    sf::Font font;
    auto rect = std::make_unique<RectangleShape>(name, position, velocity, color, width, height, font);
    
    sf::FloatRect bounds = rect->getGlobalBounds();
    CHECK(bounds.left >= 0);
    CHECK(bounds.top >= 0);
    CHECK(bounds.width > 0);
    CHECK(bounds.height > 0);
  }
}

// ============================================================================
// TESTES UNITÁRIOS - CONFIG MANAGER
// ============================================================================

TEST_SUITE("ConfigManager") {
  
  TEST_CASE("ConfigManager carrega arquivo válido") {
    ConfigManager config;
    
    // O formato do config.txt é específico: Window width height, Font path size r g b
    std::string testConfigPath = "/tmp/test_config.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "# Test configuration\n";
    testFile << "Window 800 600\n";
    testFile << "Font fonts/ShareTech-Regular.ttf 16 255 255 255\n";
    testFile.close();
    
    bool loaded = config.loadFromFile(testConfigPath);
    CHECK(loaded == true);
    
    const auto& windowConfig = config.getWindowConfig();
    CHECK(windowConfig.width == 800);
    CHECK(windowConfig.height == 600);
    
    // Limpar arquivo de teste
    std::remove(testConfigPath.c_str());
  }

  TEST_CASE("ConfigManager retorna false quando arquivo não existe") {
    ConfigManager config;
    
    bool loaded = config.loadFromFile("/tmp/nonexistent_file_12345.txt");
    CHECK(loaded == false);
  }

  TEST_CASE("ConfigManager carrega fonte corretamente") {
    ConfigManager config;
    
    std::string testConfigPath = "/tmp/test_font.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "Window 800 600\n";
    testFile << "Font fonts/ShareTech-Regular.ttf 20 0 255 0\n";
    testFile.close();
    
    bool loaded = config.loadFromFile(testConfigPath);
    CHECK(loaded == true);
    CHECK(config.isFontLoaded() == true);
    
    const auto& fontConfig = config.getFontConfig();
    CHECK(fontConfig.size == 20);
    CHECK(fontConfig.filePath == "fonts/ShareTech-Regular.ttf");
    
    std::remove(testConfigPath.c_str());
  }
}

// ============================================================================
// TESTES UNITÁRIOS - SHAPE LOADER
// ============================================================================

TEST_SUITE("ShapeLoader") {
  
  TEST_CASE("ShapeLoader carrega círculo do arquivo de configuração") {
    ConfigManager config;
    
    std::string testConfigPath = "/tmp/test_circle.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "Window 800 600\n";
    testFile << "Font fonts/ShareTech-Regular.ttf 16 255 255 255\n";
    testFile << "Circle Circle1 100 150 50 -30 255 0 0 25\n";
    testFile.close();
    
    config.loadFromFile(testConfigPath);
    auto shapes = ShapeLoader::loadShapes(config, testConfigPath);
    
    CHECK(shapes.size() == 1);
    CHECK(shapes[0]->getName() == "Circle1");
    CHECK(shapes[0]->getPosition().x == doctest::Approx(100.f));
    CHECK(shapes[0]->getPosition().y == doctest::Approx(150.f));
    CHECK(shapes[0]->getVelocity().x == doctest::Approx(50.f));
    CHECK(shapes[0]->getVelocity().y == doctest::Approx(-30.f));
    
    std::remove(testConfigPath.c_str());
  }

  TEST_CASE("ShapeLoader carrega retângulo do arquivo de configuração") {
    ConfigManager config;
    
    std::string testConfigPath = "/tmp/test_rect.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "Window 800 600\n";
    testFile << "Font fonts/ShareTech-Regular.ttf 16 255 255 255\n";
    testFile << "Rectangle Rect1 200 250 -50 30 0 255 0 80 60\n";
    testFile.close();
    
    config.loadFromFile(testConfigPath);
    auto shapes = ShapeLoader::loadShapes(config, testConfigPath);
    
    CHECK(shapes.size() == 1);
    CHECK(shapes[0]->getName() == "Rect1");
    CHECK(shapes[0]->getPosition().x == doctest::Approx(200.f));
    CHECK(shapes[0]->getPosition().y == doctest::Approx(250.f));
    
    std::remove(testConfigPath.c_str());
  }

  TEST_CASE("ShapeLoader carrega múltiplas formas") {
    ConfigManager config;
    
    std::string testConfigPath = "/tmp/test_multiple.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "Window 800 600\n";
    testFile << "Font fonts/ShareTech-Regular.ttf 16 255 255 255\n";
    testFile << "Circle C1 100 100 30 20 255 0 0 20\n";
    testFile << "Rectangle R1 500 400 -40 50 0 0 255 60 40\n";
    testFile << "Circle C2 700 500 -25 -35 0 255 0 15\n";
    testFile.close();
    
    config.loadFromFile(testConfigPath);
    auto shapes = ShapeLoader::loadShapes(config, testConfigPath);
    
    CHECK(shapes.size() == 3);
    
    std::remove(testConfigPath.c_str());
  }
}

// ============================================================================
// TESTES DE INTEGRAÇÃO
// ============================================================================

TEST_SUITE("Integration") {
  
  TEST_CASE("Fluxo completo: carregar config -> criar shapes -> atualizar") {
    // Criar arquivo de configuração de teste
    std::string testConfigPath = "/tmp/test_integration.txt";
    std::ofstream testFile(testConfigPath);
    testFile << "window_width: 800\n";
    testFile << "window_height: 600\n";
    testFile << "window_title: Integration Test\n";
    testFile << "shapes:\n";
    testFile << "  - type: circle\n";
    testFile << "    name: TestCircle\n";
    testFile << "    position: [400, 300]\n";
    testFile << "    color: [255, 0, 0]\n";
    testFile << "    velocity: [100, 50]\n";
    testFile.close();
    
    // Carregar configuração
    ConfigManager config;
    config.loadFromFile(testConfigPath);
    
    CHECK(config.getWindowWidth() == 800);
    CHECK(config.getWindowHeight() == 600);
    CHECK(config.getShapes().size() == 1);
    
    // Criar shapes
    std::vector<std::unique_ptr<Shape>> shapes;
    for (const auto& shapeData : config.getShapes()) {
      auto shape = ShapeLoader::createShape(shapeData);
      if (shape) {
        shapes.push_back(std::move(shape));
      }
    }
    
    CHECK(shapes.size() == 1);
    CHECK(shapes[0]->getName() == "TestCircle");
    
    // Atualizar e verificar movimento
    sf::Vector2f initialPos = shapes[0]->getPosition();
    shapes[0]->update(1.0f, sf::Vector2u{800, 600});
    sf::Vector2f newPos = shapes[0]->getPosition();
    
    // Deve ter se movido
    CHECK((newPos.x != initialPos.x || newPos.y != initialPos.y));
    
    std::remove(testConfigPath.c_str());
  }

  TEST_CASE("Múltiplas formas colidem independentemente") {
    std::vector<std::unique_ptr<Shape>> shapes;
    
    sf::Font font;
    
    // Criar múltiplas formas em posições diferentes
    shapes.push_back(std::make_unique<CircleShape>(
      "Circle1", sf::Vector2f{100.f, 100.f}, sf::Vector2f{50.f, 50.f}, sf::Color::Red, 25.f, font
    ));
    
    shapes.push_back(std::make_unique<RectangleShape>(
      "Rect1", sf::Vector2f{700.f, 500.f}, sf::Vector2f{-50.f, -50.f}, sf::Color::Blue, 60.f, 40.f, font
    ));
    
    shapes.push_back(std::make_unique<CircleShape>(
      "Circle2", sf::Vector2f{400.f, 300.f}, sf::Vector2f{100.f, -100.f}, sf::Color::Green, 20.f, font
    ));
    
    CHECK(shapes.size() == 3);
    
    // Atualizar todas as formas
    for (auto& shape : shapes) {
      shape->update(0.5f, sf::Vector2u{800, 600});
    }
    
    // Verificar que todas se moveram e estão dentro dos limites
    for (const auto& shape : shapes) {
      sf::FloatRect bounds = shape->getGlobalBounds();
      CHECK(bounds.position.x >= 0);
      CHECK(bounds.position.y >= 0);
      CHECK(bounds.position.x + bounds.size.x <= 800);
      CHECK(bounds.position.y + bounds.size.y <= 600);
    }
  }
}
