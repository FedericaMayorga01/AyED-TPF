#include "../include/DijkstraStrategy.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include <list>
#include <map>

std::map<int, std::map<int, Link>> DijkstraStrategy::computeOptimalPaths(
    std::map<int, std::list<NeighborWaitPkg>> waitQueueRouter,
    std::map<int, std::list<Link>> globalRoutingTable) {
        // TODO implement Dijkstra's algorithm
        return std::map<int, std::map<int, Link>>();
}