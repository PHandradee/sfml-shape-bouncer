#include <SFML/Graphics.hpp>
#include <imgui-SFML.h>
#include <imgui.h>

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

constexpr std::string file_name{"config.txt"};
sf::Font font{};

// =============== UTILS ===============
bool file_exists(const std::string &name) {
  std::ifstream f(name);
  return f.good();
}

std::string read_file_content(const std::string &file_path) {
  std::ifstream file_stream(file_path);
  std::string content;
  if (file_stream.is_open()) {
    std::string line;
    while (std::getline(file_stream, line)) {
      content += line + "\n";
    }
    file_stream.close();
  }
  return content;
}

// =============== CONFIG ===============
struct ConfigEntityInfo {
  std::string entity_type{};
  std::vector<std::string> attributes{};

  std::string to_string() const {
    std::ostringstream oss;
    oss << "Entity Type: " << entity_type << "\nAttributes: [";
    for (const auto &attr : attributes) {
      oss << attr << " ";
    }
    oss << "]";
    return oss.str();
  }
};

enum class ShapeType {
  CIRCLE = 1,
  RECTANGLE,
  TRIANGLE,
  UNKNOWN,
};

enum class EntityType {
  TEXT = 1,
  SHAPE,
  UNKNOWN,
};

// =============== SHAPE WRAPPERS ===============
class Circle : public sf::CircleShape {
public:
  Circle(const std::string &name, const std::pair<float, float> &position,
         const std::pair<float, float> &speed,
         const std::tuple<int, int, int> &color, float radius)
      : sf::CircleShape(radius), m_name(name), m_position(position),
        m_speed(speed) {
    setFillColor(
        sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
    setPosition(sf::Vector2f(m_position.first, m_position.second));
  }

  const std::string &get_name() const { return m_name; }

private:
  std::string m_name;
  std::pair<float, float> m_position;
  std::pair<float, float> m_speed;
};

class Rectangle : public sf::RectangleShape {
public:
  Rectangle(const std::string &name, const std::pair<float, float> &position,
            const std::pair<float, float> &speed,
            const std::tuple<int, int, int> &color, float width, float height)
      : sf::RectangleShape(sf::Vector2f(width, height)), m_name(name),
        m_position(position), m_speed(speed) {
    setFillColor(
        sf::Color(std::get<0>(color), std::get<1>(color), std::get<2>(color)));
    setPosition(sf::Vector2f(m_position.first, m_position.second));
  }

  const std::string &get_name() const { return m_name; }

private:
  std::string m_name;
  std::pair<float, float> m_position;
  std::pair<float, float> m_speed;
};

// =============== ENTITY ===============
using Entity = std::variant<sf::Text, Circle, Rectangle>;

// =============== FACTORIES ===============
class ShapeFactory {
public:
  static Entity create_shape(const ConfigEntityInfo &info) {
    auto type = create_entity_type(info.entity_type);

    // CIRCLE
    if (type == ShapeType::CIRCLE) {
      if (info.attributes.size() >= 9) {
          return Circle(
              info.attributes[0],
              {std::stof(info.attributes[1]), std::stof(info.attributes[2])},
              {std::stof(info.attributes[3]), std::stof(info.attributes[4])},
              {std::stoi(info.attributes[5]), std::stoi(info.attributes[6]),
               std::stoi(info.attributes[7])},
              std::stof(info.attributes[8]));

      }
      // fallback
      return Circle("FallbackCircle", {0, 0}, {0, 0}, {255, 255, 255}, 10.0f);
    }
    // RECTANGLE
    else if (type == ShapeType::RECTANGLE) {
      if (info.attributes.size() >= 10) {

          return Rectangle(
              info.attributes[0],
              {std::stof(info.attributes[1]), std::stof(info.attributes[2])},
              {std::stof(info.attributes[3]), std::stof(info.attributes[4])},
              {std::stoi(info.attributes[5]), std::stoi(info.attributes[6]),
               std::stoi(info.attributes[7])},
              std::stof(info.attributes[8]), std::stof(info.attributes[9]));

      }
      // fallback
      return Rectangle("FallbackRect", {0, 0}, {0, 0}, {200, 200, 200}, 20.0f,
                       15.0f);
    }
    // TRIANGLE ou UNKNOWN
    else {
      std::cerr << "Warning: Unknown or unsupported shape type: "
                << info.entity_type << "\n";
      return Circle("UnknownShape", {0, 0}, {0, 0}, {255, 0, 255}, 8.0f);
    }
  }

private:
  static ShapeType create_entity_type(const std::string &entity_type_str) {
    if (entity_type_str == "Circle")
      return ShapeType::CIRCLE;
    if (entity_type_str == "Rectangle")
      return ShapeType::RECTANGLE;
    if (entity_type_str == "Triangle")
      return ShapeType::TRIANGLE;
    return ShapeType::UNKNOWN;
  }
};

class EntityFactory {
public:
  static Entity create_entity(const ConfigEntityInfo &info) {
    auto type = create_entity_type(info.entity_type);

    if (type == EntityType::TEXT) {

      std::string font_path =
          !info.attributes.empty() ? info.attributes[0] : "";
      if (!font_path.empty() && font.openFromFile(font_path)) {
        std::string text_str =
            (info.attributes.size() > 1) ? info.attributes[1] : "Sample";
        int size =
            (info.attributes.size() > 2) ? std::stoi(info.attributes[2]) : 20;
        sf::Color color = sf::Color::White;
        if (info.attributes.size() > 5) {
          try {
            color = sf::Color(std::stoi(info.attributes[3]),
                              std::stoi(info.attributes[4]),
                              std::stoi(info.attributes[5]));
          } catch (...) {
          }
        }
        sf::Text text{font};
        text.setString(text_str);
        text.setCharacterSize(size);
        text.setFillColor(color);
        return text;
      } else {
        std::cerr << "Warning: Failed to load font, using dummy shape.\n";
        return Circle("DummyText", {10, 10}, {0, 0}, {100, 100, 100}, 5.0f);
      }
    } else if (type == EntityType::SHAPE) {
      return ShapeFactory::create_shape(info);
    } else {
      std::cerr << "Warning: Unknown entity type: " << info.entity_type << "\n";
      return Circle("UnknownEntity", {0, 0}, {0, 0}, {128, 128, 128}, 6.0f);
    }
  }

private:
  static EntityType create_entity_type(const std::string &entity_type_str) {
    if (entity_type_str == "Font")
      return EntityType::TEXT;
    if (entity_type_str == "Circle" || entity_type_str == "Rectangle" ||
        entity_type_str == "Triangle")
      return EntityType::SHAPE;
    return EntityType::UNKNOWN;
  }
};

// =============== PARSING ===============
ConfigEntityInfo parse_config_line(const std::string &line) {
  ConfigEntityInfo entity_info;
  std::istringstream stream(line);
  std::vector<std::string> tokens;
  std::string token;
  while (stream >> token) {
    tokens.push_back(token);
  }
  if (!tokens.empty()) {
    entity_info.entity_type = tokens[0];
    entity_info.attributes.assign(tokens.begin() + 1, tokens.end());
  }
  return entity_info;
}

void populate_entities_buffer(const std::string &file_content,
                              std::vector<ConfigEntityInfo> &entities) {
  std::istringstream file_stream(file_content);
  std::string line;
  while (std::getline(file_stream, line)) {
    if (!line.empty() &&
        line.find_first_not_of(" \t\r\n") != std::string::npos) {
      entities.push_back(parse_config_line(line));
    }
  }
}

// =============== MAIN ===============
int main(int argc, char *argv[]) {
  std::cout << "Number of Arguments: " << argc << "\n";
  for (int i = 0; i < argc; ++i) {
    std::cout << "  argv[" << i << "]: " << argv[i] << "\n";
  }

  std::string file_path = file_name;
  if (argc > 1 && std::string(argv[1]) == "--config_file_path") {
    if (argc > 2) {
      file_path = std::string(argv[2]) + file_name;
    } else {
      std::cerr << "Error: --config_file_path requires a path.\n";
      return 1;
    }
  }
  std::cout << "Using config file: " << file_path << "\n";

  std::string file_content;
  if (file_exists(file_path)) {
    file_content = read_file_content(file_path);
    std::cout << "Config content:\n" << file_content << "\n";
  } else {
    std::cerr << "Config file not found: " << file_path << "\n";
    return 1;
  }

  std::vector<ConfigEntityInfo> entities{};
  populate_entities_buffer(file_content, entities);

  std::vector<Entity> entity_buffer{};
  for (int i = 0; i < entities.size() - 1; i++) {
    if (i == 0) {
      continue;
    }
    const auto &info = entities[i];
    std::cout << "Processing Entity " << i + 1 << ":\n"
              << info.to_string() << std::endl;

    Entity entity = EntityFactory::create_entity(info);
    entity_buffer.push_back(std::move(entity));
  }

  // =============== SFML + IMGUI ===============
  sf::RenderWindow window(sf::VideoMode({640, 480}), "ImGui + SFML = <3");
  window.setFramerateLimit(60);
  ImGui::SFML::Init(window);

  sf::Clock deltaClock;
  while (window.isOpen()) {
    while (const auto event = window.pollEvent()) {
      ImGui::SFML::ProcessEvent(window, *event);

      if (event->is<sf::Event::Closed>()) {
        window.close();
      }
    }

    ImGui::SFML::Update(window, deltaClock.restart());

    ImGui::Begin("Control");
    ImGui::Text("Entities loaded: %d", static_cast<int>(entity_buffer.size()));
    ImGui::End();

    window.clear(sf::Color::Black);

    for (auto &entity : entity_buffer) {
      std::visit([&window](auto &drawable) { window.draw(drawable); }, entity);
    }

    ImGui::SFML::Render(window);
    window.display();
  }

  ImGui::SFML::Shutdown();
  return 0;
}