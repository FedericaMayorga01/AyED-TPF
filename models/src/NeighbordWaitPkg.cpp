#include "../include/NeighborWaitPkg.hpp"

NeighborWaitPkg::NeighborWaitPkg(int neighbor, int waitPkg) {
  this->neighbor = neighbor;
  this->waitPkg = waitPkg;
}

int NeighborWaitPkg::getNeighbor() const { return neighbor; }
int NeighborWaitPkg::getWaitPkg() const { return waitPkg; }