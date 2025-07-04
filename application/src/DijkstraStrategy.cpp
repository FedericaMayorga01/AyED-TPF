#include "../include/DijkstraStrategy.hpp"

#include <iostream>

#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include <list>
#include <map>
#include <queue>
#include <limits>
#include <ostream>
#include <set>
#include <unordered_map>
#include <stdexcept>

std::map<int, std::map<int, Link>> DijkstraStrategy::computeOptimalPaths(
    std::map<int, std::list<NeighborWaitPkg>> waitQueueRouter,
    std::map<int, std::list<Link>> globalRoutingTable,
    const Administrator* admin)
{
    // Validates the administrator pointer
    if (!admin) {
        throw std::invalid_argument("Administrator cannot be null");
    }

    std::map<int, std::map<int, Link>> allRoutingTables;

    // Lookup tables for wait queues and links
    std::map<int, std::unordered_map<int, int>> waitQueueLookup;
    std::map<int, std::unordered_map<int, Link>> linkLookup;

    // Lookup for wait queues
    for (const auto& [routerId, neighbors] : waitQueueRouter) {
        for (const NeighborWaitPkg& nwp : neighbors) {
            waitQueueLookup[routerId][nwp.getNeighbor()] = nwp.getWaitPkg();
        }
    }

    // Lookup for links in the global routing table
    for (const auto& [routerId, links] : globalRoutingTable) {
        for (const Link& link : links) {
            linkLookup[routerId][link.getNeighbor()] = link;
        }
    }

    for (const auto& routerPair : globalRoutingTable) {
        int source = routerPair.first;
        std::map<int, double> dist;
        std::map<int, int> prev;
        std::map<int, int> firstHop;
        std::set<int> visited;

        // Initialize distances
        for (const auto& router : globalRoutingTable)
            dist[router.first] = std::numeric_limits<double>::infinity();
        dist[source] = 0;

        // Min-heap: (cost, router)
        using QElem = std::pair<double, int>;
        std::priority_queue<QElem, std::vector<QElem>, std::greater<>> priorityQueue;
        priorityQueue.emplace(0, source);

        while (!priorityQueue.empty()) {
            auto [cost, currentRouter] = priorityQueue.top();
            priorityQueue.pop();

            if (visited.count(currentRouter)) continue;
            visited.insert(currentRouter);

            for (const Link& link : globalRoutingTable[currentRouter]) {
                int neighborRouter = link.getNeighbor();

                int queueSize = 0;
                auto routerIt = waitQueueLookup.find(currentRouter);
                if (routerIt != waitQueueLookup.end()) {
                    auto neighborIt = routerIt->second.find(neighborRouter);
                    if (neighborIt != routerIt->second.end()) {
                        queueSize = neighborIt->second;
                    }
                }

                int bandwidth = link.getBandwidth();

                double linkCost;
                try {
                    linkCost = admin->calculatePathWeight(queueSize, bandwidth);
                } catch (const std::exception& e) {
                    linkCost = std::numeric_limits<double>::infinity();
                    continue;
                }

                double newCost = cost + linkCost;
                if (newCost < dist[neighborRouter]) {
                    dist[neighborRouter] = newCost;
                    prev[neighborRouter] = currentRouter;
                    // Set first hop: if coming from source, it's v; else, inherit
                    firstHop[neighborRouter] = (currentRouter == source) ? neighborRouter : firstHop[currentRouter];
                    priorityQueue.emplace(newCost, neighborRouter);
                }
            }
        }

        // Build the routing table for this source router
        std::map<int, Link> routingTable;
        for (const auto& routerEntry : globalRoutingTable) {
            int dest = routerEntry.first;
            if (dest == source) continue;

            if (firstHop.count(dest)) {
                //std::cout << "Router " << source << " found path to " << dest << " via first hop " << firstHop[dest] << std::endl;
                int nextHop = firstHop[dest];
                //std::cout << "Router " << source << " found nextHop " << nextHop << std::endl;

                auto routerLinks = linkLookup.find(source);
                if (routerLinks != linkLookup.end()) {
                    auto linkIt = routerLinks->second.find(nextHop);
                    if (linkIt != routerLinks->second.end()) {
                        //std::cout << "Router " << source << " has links to neighbors: "
                        //          << nextHop << " with bandwidth " << linkIt->second.getBandwidth() << std::endl;
                        routingTable[dest] = linkIt->second;
                    }
                }
            }
        }
        allRoutingTables[source] = routingTable;
        //std::cout << "Router " << source << " has routing table with " << routingTable.size() << " entries." << std::endl;
    }
    return allRoutingTables;
}

// 257 ──100─── 256 ──100─── 258
//              │
//              │80
//              │ 512 ──90─── 768 ──100─── 769