#include "../include/Administrator.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include "../../models/include/Router.hpp"
#include "../include/RoutingStrategy.hpp"
#include "../include/NetworkSimulator.hpp"
#include <list>

// Constructor
Administrator::Administrator(NetworkSimulator* NetworkSimulator, RoutingStrategy* routingStrategy)
    : routingStrategy(routingStrategy), networkSimulator(NetworkSimulator)
{
}

// Getters
RoutingStrategy* Administrator::getRoutingStrategy() const
{
    return routingStrategy;
}

void Administrator::setRoutingStrategy(RoutingStrategy* routingStrategy)
{
    this->routingStrategy = routingStrategy;
}

// Methods
void Administrator::recomputes(int cycle, const std::map<int, std::list<Link>> &globalRoutingTable)
{
    const auto result = routingStrategy->computeOptimalPaths(collectRouterQueues(), globalRoutingTable, this);
    updateAllRoutingTables(cycle, result);
}

std::map<int, std::list<NeighborWaitPkg>> Administrator::collectRouterQueues() const {
    std::map<int, std::list<NeighborWaitPkg>> routerWaitingQueues;
    for (auto& router : networkSimulator->getRouters())
    {
        std::list<NeighborWaitPkg> neighborWaitingPackages;
        for (const auto& mapQueue : router.getPackageQueuesByNeighbor())
        {
            neighborWaitingPackages.emplace_back(mapQueue.first, mapQueue.second.size());
        }
        routerWaitingQueues.emplace(router.getRouterAddress(), neighborWaitingPackages);
    }

    return routerWaitingQueues;
}

void Administrator::updateAllRoutingTables(int cycle, std::map<int, std::map<int, Link>> routingTables) const {
    bool initialize = cycle == 1;
    for (auto& router : networkSimulator->getRouters())
    {
        router.updateRoutingTable(initialize, networkSimulator->getQueueSize(), routingTables[router.getRouterAddress()]);
    }
}

double Administrator::calculatePathWeight(int queueSize, int bandwidth) const {
    return 1.0 + std::ceil(static_cast<double>(queueSize) / bandwidth);
}