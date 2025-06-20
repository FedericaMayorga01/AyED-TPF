#ifndef NETWORKCONFIG_HPP
#define NETWORKCONFIG_HPP

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

struct NeighborConfig {
    int neighbor_address;
    int bandwidth;
};

struct NodeConfig {
    int node_address;
    std::vector<NeighborConfig> neighbors;
};

struct NetworkConfig {
    int total_cycle;
    std::vector<NodeConfig> nodes;
};

// Utility class for address manipulation (treating as 16-bit binary)
class AddressUtils {
public:
    // Extract router number from address (first byte)
    static int getRouterNumber(int address) {
        return (address >> 8) & 0xFF; // Get upper 8 bits
    }
    
    // Extract terminal number from address (second byte)
    static int getTerminalNumber(int address) {
        return address & 0xFF; // Get lower 8 bits
    }
    
    // Check if address is a router (terminal number = 0)
    static bool isRouter(int address) {
        return getTerminalNumber(address) == 0;
    }
    
    // Check if address is a terminal (terminal number > 0)
    static bool isTerminal(int address) {
        return getTerminalNumber(address) > 0;
    }
    
    // Create router address from router number
    static int createRouterAddress(int routerNumber) {
        return (routerNumber << 8); // routerNumber in upper 8 bits, 0 in lower
    }
    
    // Create terminal address from router number and terminal number
    static int createTerminalAddress(int routerNumber, int terminalNumber) {
        return (routerNumber << 8) | terminalNumber;
    }
    
    // Get the router address that a terminal is connected to
    static int getRouterAddressForTerminal(int terminalAddress) {
        return createRouterAddress(getRouterNumber(terminalAddress));
    }
};

// JSON serialization support
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NeighborConfig, neighbor_address, bandwidth)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NodeConfig, node_address, neighbors)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NetworkConfig, total_cycle, nodes)

class ConfigParser {
public:
    static NetworkConfig parseConfig(const std::string& filename);
    static NetworkConfig parseJsonString(const std::string& jsonContent);
};

#endif // NETWORKCONFIG_HPP