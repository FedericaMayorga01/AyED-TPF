#ifndef DIKJSTRA_STRATEGY_HPP
#define DIKJSTRA_STRATEGY_HPP

#include "../../models/include/Link.hpp"
#include "../../models/include/NeighborWaitPkg.hpp"
#include "RoutingStrategy.hpp"
#include <list>
#include <map>
#include "Administrator.hpp"

class DijkstraStrategy final : public RoutingStrategy
{
public:
  std::map<int, std::map<int, Link>> computeOptimalPaths(std::map<int, std::list<NeighborWaitPkg>> waitQueueRouter,
                                                         std::map<int, std::list<Link>> globalRoutingTable,
                                                         const Administrator* admin) override;
};

#endif // DIKJSTRA_STRATEGY_HPP