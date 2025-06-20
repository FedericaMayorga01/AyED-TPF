#include "application/include/NetworkSimulator.hpp"

int main(int argc, char* argv[]) {
    try {
        std::string configFile = "config.json";
        
        // Allow config file to be specified as command line argument
        if (argc > 1) {
            configFile = argv[1];
        }
        
        std::cout << "Network Simulator Starting..." << std::endl;
        std::cout << "Using config file: " << configFile << std::endl;
        
        NetworkSimulator simulator(configFile);
        simulator.printNetworkInfo();
        simulator.run();
        
    } catch (const std::exception& e) {
        std::cerr << "Simulation failed: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}