#ifndef NEIGHBORWAITPKG_HPP
#define NEIGHBORWAITPKG_HPP

class NeighborWaitPkg {
private:
  int neighbor;
  int waitPkg;

public:
  NeighborWaitPkg(int neighbor, int waitPkg);
  int getNeighbor() const;
  int getWaitPkg() const;
};

#endif // NEIGHBORWAITPKG_HPP