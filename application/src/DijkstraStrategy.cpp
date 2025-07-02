#include "../include/DijkstraStrategy.hpp"
#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include <list>
#include <map>
#include <iostream>

std::map<int, std::map<int, Link>> DijkstraStrategy::computeOptimalPaths(
    std::map<int, std::list<NeighborWaitPkg>> waitQueueRouter, std::map<int, std::list<Link>> globalRoutingTable)
{

    std::cout << "DijkstraStrategy: Computing mocked optimal paths..." << std::endl;

    std::map<int, std::map<int, Link>> allRoutingTables;

    // ✅ ROUTER 256 - Routing table
    std::map<int, Link> router256Table;
    router256Table[257] = Link(256, 257, 100); // Direct connection
    router256Table[258] = Link(256, 258, 100); // Direct connection
    router256Table[512] = Link(256, 512, 80);  // Direct connection
    router256Table[768] = Link(256, 512, 80);  // Via 512 (256->512->768)
    router256Table[769] = Link(256, 512, 80);  // Via 512 (256->512->768->769)
    allRoutingTables[256] = router256Table;

    // ✅ ROUTER 257 - Routing table
    std::map<int, Link> router257Table;
    router257Table[256] = Link(257, 256, 100); // Direct connection
    router257Table[258] = Link(257, 256, 100); // Via 256 (257->256->258)
    router257Table[512] = Link(257, 256, 100); // Via 256 (257->256->512)
    router257Table[768] = Link(257, 256, 100); // Via 256 (257->256->512->768)
    router257Table[769] = Link(257, 256, 100); // Via 256 (257->256->512->768->769)
    allRoutingTables[257] = router257Table;

    // ✅ ROUTER 258 - Routing table
    std::map<int, Link> router258Table;
    router258Table[256] = Link(258, 256, 100); // Direct connection
    router258Table[257] = Link(258, 256, 100); // Via 256 (258->256->257)
    router258Table[512] = Link(258, 256, 100); // Via 256 (258->256->512)
    router258Table[768] = Link(258, 256, 100); // Via 256 (258->256->512->768)
    router258Table[769] = Link(258, 256, 100); // Via 256 (258->256->512->768->769)
    allRoutingTables[258] = router258Table;

    // ✅ ROUTER 512 - Routing table
    std::map<int, Link> router512Table;
    router512Table[256] = Link(512, 256, 80); // Direct connection
    router512Table[257] = Link(512, 256, 80); // Via 256 (512->256->257)
    router512Table[258] = Link(512, 256, 80); // Via 256 (512->256->258)
    router512Table[768] = Link(512, 768, 90); // Direct connection
    router512Table[769] = Link(512, 768, 90); // Via 768 (512->768->769)
    allRoutingTables[512] = router512Table;

    // ✅ ROUTER 768 - Routing table
    std::map<int, Link> router768Table;
    router768Table[256] = Link(768, 512, 90);  // Via 512 (768->512->256)
    router768Table[257] = Link(768, 512, 90);  // Via 512 (768->512->256->257)
    router768Table[258] = Link(768, 512, 90);  // Via 512 (768->512->256->258)
    router768Table[512] = Link(768, 512, 90);  // Direct connection
    router768Table[769] = Link(768, 769, 100); // Direct connection
    allRoutingTables[768] = router768Table;

    // ✅ ROUTER 769 - Routing table
    std::map<int, Link> router769Table;
    router769Table[256] = Link(769, 768, 100); // Via 768 (769->768->512->256)
    router769Table[257] = Link(769, 768, 100); // Via 768 (769->768->512->256->257)
    router769Table[258] = Link(769, 768, 100); // Via 768 (769->768->512->256->258)
    router769Table[512] = Link(769, 768, 100); // Via 768 (769->768->512)
    router769Table[768] = Link(769, 768, 100); // Direct connection
    allRoutingTables[769] = router769Table;

    // ✅ Debug output
    std::cout << "DijkstraStrategy: Generated routing tables for " << allRoutingTables.size()
              << " routers:" << std::endl;

    for (const auto& routerEntry : allRoutingTables)
    {
        int routerAddr = routerEntry.first;
        const auto& table = routerEntry.second;

        std::cout << "  Router " << routerAddr << " has " << table.size() << " routes:" << std::endl;

        for (const auto& route : table)
        {
            int dest = route.first;
            const Link& link = route.second;
            std::cout << "    To " << dest << " via " << link.getNeighbor() << " (BW: " << link.getBandwidth() << ")"
                      << std::endl;
        }
    }

    return allRoutingTables;
}

// 257 ──100─── 256 ──100─── 258
//              │
//              │80
//              │ 512 ──90─── 768 ──100─── 769
