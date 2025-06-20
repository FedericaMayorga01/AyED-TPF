#ifndef ROUTER_HPP
#define ROUTER_HPP

// Headers
#include <map>
#include <queue>
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
    std::map<int, std::queue<Package>> packageQueuesByNeighbor; // Maps neighbor address to its package queue
    std::map<int, int> routingTable;                            // Maps destination address to next hop address
    std::map<int, std::list<Package>> pendingPackagesByPageId;  // Maps page ID to a list of packages that are pending for that page
    bool amIEndNode;

public:
    Router(int routerAddress, std::map<int, int> routingTable);

    // Getters
    int getRouterAddress() const;
    bool getAmIEndNode() const;

    // Methods
    std::queue<Package> splitPage(const Page &page);
    void sendPackage(int destAddress, const Package &package);
    void processQueues();
    void updateRoutingTable(std::map<int, int> newRoutingTable); // newRoutingTable????

    // End node methods
    Page rebuildPage();
    void sendPage(int destTerminalAddress);
};

#endif // ROUTER_HPP