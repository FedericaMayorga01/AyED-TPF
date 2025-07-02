#include "Link.hpp"
#include <iostream>

// Default constructor
Link::Link() : sourceAddress(0), neighbor(0), bandwidth(0)
{
}

Link::Link(int sourceAddress, int neighbor, int bandwidth)
    : sourceAddress(sourceAddress), neighbor(neighbor), bandwidth(bandwidth)
{
}

Link::~Link()
{
}

// Getters
int Link::getSourceAddress() const
{
    return sourceAddress;
}

int Link::getNeighbor() const
{
    return neighbor;
}

int Link::getBandwidth() const
{
    return bandwidth;
}

// Setters
void Link::setBandwidth(int bw)
{
    bandwidth = bw;
}
