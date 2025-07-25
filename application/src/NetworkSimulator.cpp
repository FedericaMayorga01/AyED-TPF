#include "../include/NetworkSimulator.hpp"
#include "../../models/include/Link.hpp"
#include <iostream>
#include <vector>

NetworkSimulator::NetworkSimulator(const std::string& configFile, int currentCycle)
{
    this -> currentCycle = currentCycle;
    loadConfiguration(configFile);
    initializeNetwork();
    setupAdministrator();
}

int NetworkSimulator::getQueueSize() const
{
    return config.queue_size;
}

int NetworkSimulator::getCycle() const
{
    return config.total_cycle;
}

int NetworkSimulator::getCurrentCycle() const {
    return currentCycle;
}

void NetworkSimulator::incrementCurrentCycle() {
    currentCycle += 1;
}

void NetworkSimulator::updatePendingCurrentCycles() const {
    for (auto& router : routers)
    {
        for (auto& entry : router.getPackageQueuesByNeighbor())
        {
            for (auto& package : entry.second)
            {
                package->setCurrentCycle(currentCycle);
            }
        }
    }
}

std::vector<Router>& NetworkSimulator::getRouters()
{
    return routers;
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
            globalTable[nodeConfig.node_address] = std::list<Link>();
            //std::cout << "Found router: " << nodeConfig.node_address << " (Router #"
            //          << AddressUtils::getRouterNumber(nodeConfig.node_address) << ")" << std::endl;
        }
        else if (AddressUtils::isTerminal(nodeConfig.node_address))
        {
            terminalNodes.push_back(nodeConfig);
            //std::cout << "Found terminal: " << nodeConfig.node_address << " (Router #"
            //          << AddressUtils::getRouterNumber(nodeConfig.node_address) << ", Terminal #"
            //          << AddressUtils::getTerminalNumber(nodeConfig.node_address) << ")" << std::endl;
        }
        routerAddresses.insert(nodeConfig.node_address);
    }

    // Initialize routers
    std::cout << std::endl << "Initializing routers..." << std::endl;
    for (int address : routerAddresses)
    {
        Router router(address, config.package_size, this);
        routers.push_back(router);

        //std::cout << "Created router with address: " << address << " (Router #"
        //          << AddressUtils::getRouterNumber(address) << ")" << std::endl;
    }

    std::cout << "Initializing terminals..." << std::endl;
    // Initialize terminals
    for (const auto& terminalConfig : terminalNodes)
    {
        int terminalAddress = terminalConfig.node_address;
        int routerAddress = AddressUtils::getRouterAddressForTerminal(terminalAddress);

        Terminal terminal(terminalAddress, routerAddress, this);
        terminals.push_back(terminal);

        //std::cout << "Created terminal " << terminalAddress << " connected to router " << routerAddress << std::endl;

    }

    for (auto& terminal : terminals)
    {
        for(auto& terminal2: terminals)
        {
            if (terminal.getTerminalAddress() == terminal2.getTerminalAddress())
            {
                continue; // Skip adding itself
            }
                        
            terminal.addTerminalNode(terminal2);
        }
    }

    // Initialize links
    std::cout << "Initializing links..." << std::endl << std::endl;
    for (const auto& nodeConfig : config.nodes)
    {
        for (const auto& neighbor : nodeConfig.neighbors)
        {
            Link link(nodeConfig.node_address, neighbor.neighbor_address, neighbor.bandwidth);
            links.push_back(link);

            //std::cout << "Created link: " << neighbor.neighbor_address << " (bandwidth: " << neighbor.bandwidth << ")"
            //          << std::endl;
        }
    }
    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Network initialization completed:" << std::endl;
    std::cout << "  - Routers: " << routers.size() - terminals.size() << std::endl;
    std::cout << "  - Links: " << links.size()/2 << std::endl;
    std::cout << "  - Terminals: " << terminals.size() << std::endl;
    std::cout << "-----------------------------------" << std::endl << std::endl;
}

void NetworkSimulator::setupAdministrator()
{
    // Create routing strategy
    auto routingStrategy = new DijkstraStrategy();

    // Create administrator
    administrator = std::make_unique<Administrator>(this, routingStrategy);

    for (const auto& link : links)
    {
        //std::cout << "Adding link from " << link.getSourceAddress()
        //          << " to neighbor " << link.getNeighbor()
        //          << " with bandwidth " << link.getBandwidth() << std::endl;

        if (AddressUtils::isRouter(link.getSourceAddress()))
        {
            //std::cout << "GLOBAL_TABLE[ROUTERS]: Adding link to global routing table for node " << link.getSourceAddress() << std::endl;
            globalTable[link.getSourceAddress()].push_back(link);
            continue;
        }

        if (AddressUtils::isTerminal(link.getNeighbor())) {
            continue;
        }

        globalTable[link.getSourceAddress()].push_back(link);
        //std::cout << "GLOBAL_TABLE[TERMINALS]: Adding link to global routing table for terminal " << link.getSourceAddress() << std::endl;
    }

    std::cout << "Administrator initialized with: \033[1mDijkstraStrategy\033[0m" << std::endl;
    std::cout << "Global routing table contains " << globalTable.size() << " entries" << std::endl;
}

void NetworkSimulator::run()
{
    std::cout << "\n>>>>>>>>>>===== Starting Network Simulation =====<<<<<<<<<<" << std::endl;

    for (int cycle = 1; cycle <= config.total_cycle; cycle++)
    {
        std::cout << "\n\033[1m============================== <<< Cycle " << cycle << " >>> ==============================\033[0m" << std::endl;

        if (cycle % 2)
        {
            administrator->setRoutingStrategy(new DijkstraStrategy());
            std::cout << "Using Dijkstra's algorithm for routing" << std::endl;
        }
        else
        {
            // TODO: Implement alternative routing strategy if needed
            administrator->setRoutingStrategy(new DijkstraStrategy());
            std::cout << "Using Dijkstra's algorithm for routing" << std::endl;
        }

        // Collect router queues
        auto queueInfo = administrator->collectRouterQueues();

        // Recompute optimal paths
        administrator->recomputes(cycle, globalTable);

        // Send pages from terminals to routers
        for (auto& terminal : terminals)
        {
            terminal.sendPage();
        }

        // Process each router
        for (auto& router : routers)
        {
            std::cout << "\033[1m\033[36mProcess Queues: \033[0m" <<"Processing router " << router.getRouterAddress() << " queues..." << std::endl;
            router.processQueues();
        }

        std::cout << "\033[1m############################# Cycle " << cycle << " completed #############################\033[0m" << std::endl;
        updatePendingCurrentCycles();
        incrementCurrentCycle();
    }

    std::cout << "\n\033[32m=========================== Simulation Completed ===========================\033[0m" << std::endl;
}

void NetworkSimulator::printNetworkInfo() const {
    std::cout << "\n\033[36m========= Network Information =========\033[0m" << std::endl;
    std::cout << "Total routers: " << routers.size() - terminals.size() << std::endl;
    std::cout << "Total links: " << links.size() << std::endl;
    std::cout << "Total terminals: " << terminals.size() << std::endl;
    std::cout << "Total cycles configured: " << config.total_cycle << std::endl;
    std::cout << "\033[36m=======================================\033[0m" << std::endl;

    //std::cout << "\nRouter details:" << std::endl;
    //for (const auto& router : routers)
    //{
    //    std::cout << "  Router " << router.getRouterAddress()
    //              << " (End node: " << (router.getAmIEndNode() ? "Yes" : "No") << ")" << std::endl;
    //}
}
