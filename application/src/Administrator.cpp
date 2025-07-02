#include "../include/Administrator.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include "../../models/include/Router.hpp"
#include "../include/RoutingStrategy.hpp"
#include <iostream>
#include <list>

// Constructor
Administrator::Administrator(RoutingStrategy* routingStrategy) : routingStrategy(routingStrategy)
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
void Administrator::recomputes(const std::list<Router> routers, std::map<int, std::list<Link>> globalRoutingTable)
{
    routingStrategy->computeOptimalPaths(collectRouterQueues(routers), globalRoutingTable);
}

std::map<int, std::list<NeighborWaitPkg>> Administrator::collectRouterQueues(const std::list<Router> routers)
{
    std::map<int, std::list<NeighborWaitPkg>> routerWaitingQueues;
    for (auto router : routers)
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

void Administrator::updateAllRoutingTables(const std::map<int, std::map<int, Link>> routingTables)
{
}
