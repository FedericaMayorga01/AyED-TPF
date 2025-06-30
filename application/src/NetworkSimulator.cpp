#include "../include/NetworkSimulator.hpp"
#include "../../models/include/Link.hpp"
#include <iostream>
#include <stdexcept>
#include <vector>

NetworkSimulator::NetworkSimulator(const std::string& configFile)
{
    loadConfiguration(configFile);
    initializeNetwork();
    setupAdministrator();
}

Router* NetworkSimulator::getRouterByAddress(int address)
{
    for (auto& router : routers)
    {
        if (router.getRouterAddress() == address)
        {
            return &router;
        }
    }
    return nullptr; // Return nullptr if no router found with the given address
}

Terminal* NetworkSimulator::getTerminalByAddress(int address)
{
    for (auto& terminal : terminals)
    {
        if (terminal.getTerminalAddress() == address)
        {
            return &terminal;
        }
    }
    return nullptr; // Return nullptr if no terminal found with the given address
}

void NetworkSimulator::loadConfiguration(const std::string& configFile)
{
    try
    {
        config = ConfigParser::parseConfig(configFile);
        std::cout << "Configuration loaded successfully" << std::endl;
        std::cout << "Total cycles: " << config.total_cycle << std::endl;
        std::cout << "Queue size: " << config.queue_size << std::endl;
        std::cout << "Package size: " << config.package_size << std::endl;
        std::cout << "Number of nodes: " << config.nodes.size() << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error loading configuration: " << e.what() << std::endl;
        throw;
    }
}

void NetworkSimulator::initializeNetwork()
{
    std::set<int> routerAddresses;
    std::vector<NodeConfig> terminalNodes;

    // Separate routers and terminals based on address format
    for (const auto& nodeConfig : config.nodes)
    {
        if (AddressUtils::isRouter(nodeConfig.node_address))
        {
            routerAddresses.insert(nodeConfig.node_address);
            std::cout << "Found router: " << nodeConfig.node_address << " (Router #"
                      << AddressUtils::getRouterNumber(nodeConfig.node_address) << ")" << std::endl;
        }
        else if (AddressUtils::isTerminal(nodeConfig.node_address))
        {
            terminalNodes.push_back(nodeConfig);
            std::cout << "Found terminal: " << nodeConfig.node_address << " (Router #"
                      << AddressUtils::getRouterNumber(nodeConfig.node_address) << ", Terminal #"
                      << AddressUtils::getTerminalNumber(nodeConfig.node_address) << ")" << std::endl;
        }
    }

    // Initialize routers
    for (int address : routerAddresses)
    {
        Router router(address, config.queue_size, config.package_size, this);
        routers.push_back(router);

        std::cout << "Created router with address: " << address << " (Router #"
                  << AddressUtils::getRouterNumber(address) << ")" << std::endl;
    }

    // Initialize terminals
    for (const auto& terminalConfig : terminalNodes)
    {
        int terminalAddress = terminalConfig.node_address;
        int routerAddress = AddressUtils::getRouterAddressForTerminal(terminalAddress);

        Terminal terminal(terminalAddress, routerAddress, this);
        terminals.push_back(terminal);

        std::cout << "Created terminal " << terminalAddress << " connected to router " << routerAddress << std::endl;
    }

    for (Terminal terminal : terminals)
    {
        terminal.setTerminalNodes(terminals);
    }

    // Initialize links
    for (const auto& nodeConfig : config.nodes)
    {
        for (const auto& neighbor : nodeConfig.neighbors)
        {
            Link link(0, neighbor.neighbor_address, neighbor.bandwidth);
            links.push_back(link);

            std::cout << "Created link: " << neighbor.neighbor_address
                      << " (bandwidth: " << neighbor.bandwidth << ")" << std::endl;
        }
    }

    std::cout << "Network initialization completed:" << std::endl;
    std::cout << "  - Routers: " << routers.size() << std::endl;
    std::cout << "  - Links: " << links.size() << std::endl;
    std::cout << "  - Terminals: " << terminals.size() << std::endl;
}

void NetworkSimulator::setupAdministrator()
{
    // Create routing strategy
    auto routingStrategy = new DijkstraStrategy();

    // Create administrator
    administrator = std::make_unique<Administrator>(routingStrategy);

    // Build global routing table from links (only router-to-router links)
    std::map<int, std::list<Link>> globalTable;
    for (const auto& link : links)
    {
        // Only add router-to-router links to global routing table
        if (AddressUtils::isRouter(link.getSourceAddress()) && AddressUtils::isRouter(link.getNeighbor()))
        {
            globalTable[link.getSourceAddress()].push_back(link);
        }
    }

    std::cout << "Administrator initialized with DijkstraStrategy" << std::endl;
    std::cout << "Global routing table contains " << globalTable.size() << " router entries" << std::endl;
}

void NetworkSimulator::run()
{
    std::cout << "\n=== Starting Network Simulation ===" << std::endl;

    for (int cycle = 0; cycle < config.total_cycle; ++cycle)
    {
        std::cout << "\n--- Cycle " << cycle + 1 << " ---" << std::endl;

        // Collect router queues
        std::list<Router> routerList(routers.begin(), routers.end());
        auto queueInfo = administrator->collectRouterQueues(routerList);

        // Recompute optimal paths
        administrator->recomputes();

        // Process each router
        for (auto& router : routers)
        {
            router.processQueues();
        }

        std::cout << "Cycle " << cycle + 1 << " completed" << std::endl;
    }

    std::cout << "\n=== Simulation Completed ===" << std::endl;
}

void NetworkSimulator::printNetworkInfo()
{
    std::cout << "\n=== Network Information ===" << std::endl;
    std::cout << "Total routers: " << routers.size() << std::endl;
    std::cout << "Total links: " << links.size() << std::endl;
    std::cout << "Total terminals: " << terminals.size() << std::endl;
    std::cout << "Total cycles configured: " << config.total_cycle << std::endl;

    std::cout << "\nRouter details:" << std::endl;
    for (const auto& router : routers)
    {
        std::cout << "  Router " << router.getRouterAddress()
                  << " (End node: " << (router.getAmIEndNode() ? "Yes" : "No") << ")" << std::endl;
    }
}
