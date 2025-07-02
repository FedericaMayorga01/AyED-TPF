#include "../include/Administrator.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include "../../models/include/Router.hpp"
#include "../include/RoutingStrategy.hpp"
#include "../include/NetworkSimulator.hpp"
#include <iostream>
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
void Administrator::recomputes(std::map<int, std::list<Link>> globalRoutingTable)
{
    routingStrategy->computeOptimalPaths(collectRouterQueues(), globalRoutingTable);
}

std::map<int, std::list<NeighborWaitPkg>> Administrator::collectRouterQueues()
{
    std::map<int, std::list<NeighborWaitPkg>> routerWaitingQueues;
    for (auto& router : networkSimulator->getRouters())
    {
        std::list<NeighborWaitPkg> neighborWaitingPackages;
        for (auto mapQueue : router.getPackageQueuesByNeighbor())
        {
            neighborWaitingPackages.push_back(NeighborWaitPkg(mapQueue.first, mapQueue.second.size()));
        }
        routerWaitingQueues.emplace(router.getRouterAddress(), neighborWaitingPackages);
    }

    return routerWaitingQueues;
}

void Administrator::updateAllRoutingTables(std::map<int, std::map<int, Link>> routingTables)
{
    for (auto& router : networkSimulator->getRouters())
    {
        router.updateRoutingTable(routingTables[router.getRouterAddress()]);
    }
}
