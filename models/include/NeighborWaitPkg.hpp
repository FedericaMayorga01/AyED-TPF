#ifndef NEIGHBORWAITPKG_HPP
#define NEIGHBORWAITPKG_HPP

class NeighborWaitPkg {
private:
  int neighbor;
  int waitPkg;

public:
  NeighborWaitPkg(int neighbor, int waitPkg);
  [[nodiscard]] int getNeighbor() const;
  [[nodiscard]] int getWaitPkg() const;
};

#endif // NEIGHBORWAITPKG_HPP