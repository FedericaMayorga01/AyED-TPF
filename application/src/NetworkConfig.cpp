#include "NetworkConfig.hpp"
#include <fstream>
#include <sstream>
#include <stdexcept>

NetworkConfig ConfigParser::parseConfig(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Cannot open config file: " + filename);
    }
    
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string content = buffer.str();
    
    return parseJsonString(content);
}

NetworkConfig ConfigParser::parseJsonString(const std::string& jsonContent) {
    try {
        const nlohmann::json j = nlohmann::json::parse(jsonContent);
        auto config = j.get<NetworkConfig>();
        return config;
    } catch (const nlohmann::json::exception& e) {
        throw std::runtime_error("JSON parsing error: " + std::string(e.what()));
    }
}