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
class NetworkSimulator;
class Administrator
{
  private:
    RoutingStrategy* routingStrategy;
    NetworkSimulator* networkSimulator;

  public:
    // Constructor
    Administrator(NetworkSimulator*, RoutingStrategy*);

    // Getters
    RoutingStrategy* getRoutingStrategy() const;

    // Setters
    void setRoutingStrategy(RoutingStrategy* routingStrategy);

    // Methods
    void recomputes(std::map<int, std::list<Link>> globalRoutingTable);
    std::map<int, std::list<NeighborWaitPkg>> collectRouterQueues();
    void updateAllRoutingTables(std::map<int, std::map<int, Link>> routingTables);
};

#endif // ADMINISTRATOR_HPP
