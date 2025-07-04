#ifndef ROUTING_STRATEGY_HPP
#define ROUTING_STRATEGY_HPP
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include <list>
#include <map>
#include "Administrator.hpp"

class RoutingStrategy
{
public:
    virtual ~RoutingStrategy() = default;

    /**
     * @brief Virtual method to do strategy pattern, this function will be used in search algorithms.
     * @param waitQueueRouter A map where the key is the router address and the value is a list of NeighborWaitPkg.
     * @param globalRoutingTable A map where the key is the router address and the value is a list of Link objects.
     * @param administrator
     * @return A map where the key is the source router address and the value is another map with destination router addresses as keys and the next hop address.
     * This method is pure virtual, meaning that it must be implemented by any derived class.
     */
    virtual std::map<int, std::map<int, Link>> computeOptimalPaths(
        std::map<int, std::list<NeighborWaitPkg>> waitQueueRouter,
        std::map<int, std::list<Link>> globalRoutingTable,
        const Administrator* administrator) = 0;
};

#endif // ROUTING_STRATEGY_HPP