#ifndef NETWORKSIMULATOR_HPP
#define NETWORKSIMULATOR_HPP

#include "NetworkConfig.hpp"
#include "../../models/include/Router.hpp"
#include "../../models/include/Terminal.hpp"
#include "../../models/include/Link.hpp"
#include "Administrator.hpp"
#include "DijkstraStrategy.hpp"
#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <set>

class NetworkSimulator {
private:
    std::vector<Router> routers;
    std::vector<Terminal> terminals;
    std::vector<Link> links;
    std::unique_ptr<Administrator> administrator;
    NetworkConfig config;

public:
    NetworkSimulator(const std::string& configFile);
    Router* getRouterByAddress(int address);
    void loadConfiguration(const std::string& configFile);
    void initializeNetwork();
    void initializeTerminals();
    void setupAdministrator();
    void run();
    void printNetworkInfo();
};

#endif // NETWORKSIMULATOR_HPP