#ifndef ADMINISTRATOR_HPP
#define ADMINISTRATOR_HPP

// Headers
#include <list>
#include <map>

// Forward declarations
class RoutingStrategy;
class Router;
class Link;
class NeighborWaitPkg;

class Administrator
{
  private:
    RoutingStrategy* routingStrategy;

  public:
    // Constructor
    Administrator(RoutingStrategy* routingStrategy);

    // Getters
    RoutingStrategy* getRoutingStrategy() const;

    // Setters
    void setRoutingStrategy(RoutingStrategy* routingStrategy);

    // Methods
    void recomputes(const std::list<Router> routers, std::map<int, std::list<Link>> globalRoutingTable);
    std::map<int, std::list<NeighborWaitPkg>> collectRouterQueues(const std::list<Router> routers);
    void updateAllRoutingTables(const std::map<int, std::map<int, Link>> routingTables);
};

#endif // ADMINISTRATOR_HPP
