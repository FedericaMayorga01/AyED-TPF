#ifndef ROUTER_HPP
#define ROUTER_HPP

// Headers
#include <map>
#include <boost/circular_buffer.hpp>
#include <list>
#include "Package.hpp"
#include "Page.hpp"

// Forward declaration
class Package;
class Page;

class Router
{
private:
    int routerAddress;
    int packageQuantity;                                         // Number of packages to divade a page into
    std::map<int, boost::circular_buffer<Package>> packageQueuesByNeighbor; // Maps neighbor address to its package queue
    std::map<int, int> routingTable;                                        // Maps destination address to next hop address
    std::map<int, std::list<Package>> pendingPackagesByPageId;              // Maps page ID to a list of packages that are pending for that page
    bool amIEndNode;

public:
    Router(int routerAddress, std::map<int, int> routingTable, int queueSize, int packageQuantity);

    // Getters
    int getRouterAddress() const;
    bool getAmIEndNode() const;

    // Methods
    std::list<Package> splitPage(const Page page);
    void sendPackage(int destAddress, const Package package);
    void processQueues();
    void updateRoutingTable(std::map<int, int> newRoutingTable);
    bool hasQueueFreeSpaceForPkg(int neighborAddress) const;
    bool hasQueueFreeSpaceForPage(int neighborAddress, int pageSize) const;

    // End node methods
    Page rebuildPage();
    void sendPage(int destTerminalAddress);
};

#endif // ROUTER_HPP