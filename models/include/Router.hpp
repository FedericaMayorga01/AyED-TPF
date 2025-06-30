#ifndef ROUTER_HPP
#define ROUTER_HPP

// Headers
#include "Package.hpp"
#include "Page.hpp"
#include <boost/circular_buffer.hpp>
#include <list>
#include <map>

// Forward declaration
class Package;
class Page;
class NetworkSimulator;
class Link;

class Router
{
  private:
    int routerAddress;
    int packageSize; // Number of packages to divade a page into
    std::map<int, boost::circular_buffer<Package*>>
        packageQueuesByNeighbor;     // Maps neighbor address to its package queue
    std::map<int, Link> routingTable; // Maps destination address to next hop address
    std::map<int, std::list<Package*>>
        pendingPackagesByPageId; // Maps page ID to a list of packages that are pending for that page
    bool amIEndNode;
    NetworkSimulator* networkSimulator;

  public:
    Router(int routerAddress, int queueSize, int packageSize, NetworkSimulator* networkSimulator);

    // Getters
    int getRouterAddress() const;
    bool getAmIEndNode() const;

    // Methods
    void receivePage(Page* page);
    std::list<Package*> splitPage(Page* page);
    void sendPackage(int destAddress, Package* package);
    void receivePackage(Package* package);
    void processQueues();
    void updateRoutingTable(std::map<int, int> newRoutingTable);
    bool hasQueueFreeSpaceForPkg(int neighborAddress) const;
    bool hasQueueFreeSpaceForPage(int neighborAddress, int pageSize) const;

    // End node methods
    bool checkPagesById(int pageId);
    void printRouteTakenByPackage(Package* package);
    Page* rebuildPage(int pageId);
    void sendPage(Page* page);
};

#endif // ROUTER_HPP
