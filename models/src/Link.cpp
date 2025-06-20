#include "../include/Link.hpp"
#include <iostream>

// Constructor
Link::Link(int neighbor, int bandwidth)
    : neighbor(neighbor), bandwidth(bandwidth) {}

// Destructor
Link::~Link() {
    // Cleanup if necessary
}

// Getters
int Link::getNeighbor() const {
    return neighbor;
}

int Link::getBandwidth() const {
    return bandwidth;
}