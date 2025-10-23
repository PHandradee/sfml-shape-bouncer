#include <bitset>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

constexpr std::string file_name{"config.txt"};

bool file_exists(const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

std::string read_file_content(const std::string& file_path) {
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

struct ConfigEntityInfo {
    std::string entity_type {};
    std::vector<std::string> attributes {};

    std::string to_string() const {
        std::ostringstream oss;
        oss << "Entity Type: " << entity_type << "\nAttributes: [";
        for (const auto& attr : attributes) {
            oss <<  attr << " ";
        }
        oss << "]";
        return oss.str();
    }
};

ConfigEntityInfo parse_config_line(const std::string& line) {
    ConfigEntityInfo entity_info;
    std::istringstream stream(line);
    std::vector<std::string> tokens{};
    std::string token;

    while (stream >> token)  {
        tokens.push_back(token);
    }

    if (!tokens.empty()) {
        entity_info.entity_type = tokens[0];
        entity_info.attributes.assign(tokens.begin() + 1, tokens.end());
    }

    return entity_info;
}

void populate_entities_buffer(const std::string& file_content, std::vector<ConfigEntityInfo>& entities) {
    std::istringstream file_stream(file_content);
    std::string line;
    while (std::getline(file_stream, line)) {
        if (!line.empty()) {
            ConfigEntityInfo entity = parse_config_line(line);
            entities.push_back(entity);
        }
    }
}

int main(int argc, char* argv[]) {

    std::cout << "Number of Arguments provided: " << argc << std::endl;
    std::cout << "Arguments:" << std::endl;
    for(int i = 0; i < argc; ++i) {
        std::cout << "  argv[" << i << "]: " << argv[i] << std::endl;
    }
    
    if (argc == 1) {
        std::cerr << "No arguments provided." << std::endl;
        return 1;
    }
    
    std::string file_path {};

    if(argc > 1 && std::string(argv[1]) == "--config_file_path") {
        if (argc > 2) {
            file_path = argv[2] + file_name;
        } else {
            std::cerr << "Error: --config_file_path argument provided but no file path specified." << std::endl;
            return 1;
        }
        std::cout << "Config file path provided via argument: " << file_path << std::endl;
    } else {
        file_path = file_name;
        std::cout << "No config file path argument provided. Using default: " << file_path << std::endl;
    }

    std::string file_content {};

    if (file_exists(file_path)) {
        std::cout << "Config file found at: " << file_path << std::endl;
        file_content = read_file_content(file_path);
        std::cout << "Config file content:\n" << file_content << std::endl;
    } else {
        std::cerr << "Config file not found at: " << file_path << std::endl;
    }

    std::vector<ConfigEntityInfo> entities{};
    populate_entities_buffer(file_content, entities);

    for (const auto& e : entities) {
        std::cout << e.to_string() << std::endl;
    
    }

    return 0;
}