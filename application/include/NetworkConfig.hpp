#ifndef NETWORKCONFIG_HPP
#define NETWORKCONFIG_HPP

#include <random>
#include <string>
#include <vector>
#include <boost/circular_buffer.hpp>
#include <nlohmann/json.hpp>

class Package;

struct NeighborConfig {
    int neighbor_address;
    int bandwidth;
};

struct NodeConfig {
    int node_address{};
    std::vector<NeighborConfig> neighbors;
};

struct NetworkConfig {
    int total_cycle;
    int queue_size;
    int package_size;
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

    static void shuffleQueue(boost::circular_buffer<Package*>& queue) {
        std::vector temp(queue.begin(), queue.end());
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(temp.begin(), temp.end(), g);
        queue.clear();
        for (Package* pkg : temp) {
            queue.push_back(pkg);
        }
    }
};

// JSON serialization support
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NeighborConfig, neighbor_address, bandwidth)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NodeConfig, node_address, neighbors)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(NetworkConfig, total_cycle, queue_size, nodes, package_size)

class ConfigParser {
public:
    static NetworkConfig parseConfig(const std::string& filename);
    static NetworkConfig parseJsonString(const std::string& jsonContent);
};

#endif // NETWORKCONFIG_HPP