#ifndef ADMINISTRATOR_HPP
#define ADMINISTRATOR_HPP

// Headers
#include <map>
#include <list>

// Forward declarations
class RoutingStrategy;
class Router;
class Link;
class NeighborWaitPkg;

class Administrator
{
    private:
        RoutingStrategy *routingStrategy; // Pointer to the routing strategy
        std::map<int, std::list<Link>> globalRoutingTable; // Global routing table mapping router addresses to lists of links
    
    public:
        // Constructor
        Administrator(RoutingStrategy *routingStrategy);

        // Getters
        RoutingStrategy* getRoutingStrategy() const;
        std::map<int, std::list<Link>> getGlobalRoutingTable() const;

        // Methods
        void recomputes();
        std::map<int, std::list<NeighborWaitPkg>> collectRouterQueues(const std::list<Router> &routers);
        void updateAllRoutingTables(const std::map<int, std::map<int, int>>& routingTables);
};

#endif // ADMINISTRATOR_HPP