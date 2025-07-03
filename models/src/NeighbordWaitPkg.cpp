#include "../include/NeighborWaitPkg.hpp"

NeighborWaitPkg::NeighborWaitPkg(const int neighbor, const int waitPkg) {
  this->neighbor = neighbor;
  this->waitPkg = waitPkg;
}

int NeighborWaitPkg::getNeighbor() const { return neighbor; }
int NeighborWaitPkg::getWaitPkg() const { return waitPkg; }