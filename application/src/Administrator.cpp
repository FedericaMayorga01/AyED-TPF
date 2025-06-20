#include "../include/Administrator.hpp"
#include "../include/RoutingStrategy.hpp"
#include "../../models/include/Router.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include <iostream>

// Constructor
Administrator::Administrator(RoutingStrategy *routingStrategy)
    : routingStrategy(routingStrategy) {
    // Initialize the global routing table
}

// Getters
RoutingStrategy* Administrator::getRoutingStrategy() const {
    return routingStrategy;
}

std::map<int, std::list<Link>> Administrator::getGlobalRoutingTable() const {
    return std::map<int, std::list<Link>>();
}

// Methods
void Administrator::recomputes() {
    // This method recomputes the routing tables based on the current network state
}

std::map<int, std::list<NeighborWaitPkg>> Administrator::collectRouterQueues(const std::list<Router> &routers) {
    return std::map<int, std::list<NeighborWaitPkg>>();
}

void Administrator::updateAllRoutingTables(const std::map<int, std::map<int, int>>& routingTables) {
    // This method updates all routing tables with the new routing information
}
