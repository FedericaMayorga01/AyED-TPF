#ifndef NETWORKSIMULATOR_HPP
#define NETWORKSIMULATOR_HPP

#include "../../models/include/Link.hpp"
#include "../../models/include/Router.hpp"
#include "../../models/include/Terminal.hpp"
#include "Administrator.hpp"
#include "DijkstraStrategy.hpp"
#include "NetworkConfig.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <set>
#include <vector>

class NetworkSimulator {
private:
    std::vector<Router> routers;
    std::vector<Terminal> terminals;
    std::vector<Link> links;
    std::unique_ptr<Administrator> administrator;
    NetworkConfig config;
    std::map<int, std::list<Link> > globalTable;
    int currentCycle;

public:
    NetworkSimulator(const std::string &configFile, int currentCycle);

    Router *getRouterByAddress(int address);

    Terminal *getTerminalByAddress(int address);

    std::vector<Router> &getRouters();

    [[nodiscard]] int getQueueSize() const;

    [[nodiscard]] int getCycle() const;

    int getCurrentCycle() const;

    void incrementCurrentCycle();

    void updatePendingCurrentCycles() const;

    void loadConfiguration(const std::string &configFile);

    void initializeNetwork();

    void initializeTerminals();

    void setupAdministrator();

    void run();

    void printNetworkInfo() const;
};

#endif // NETWORKSIMULATOR_HPP
