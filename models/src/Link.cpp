#include "Link.hpp"
#include <iostream>

// New constructor
Link::Link(int source, int destination, int bandwidth)
    : sourceAddress(source), destinationAddress(destination), bandwidth(bandwidth) {}

// Backward compatibility constructor
Link::Link(int neighbor, int bandwidth)
    : sourceAddress(0), destinationAddress(neighbor), bandwidth(bandwidth) {}

Link::~Link() {
    // Cleanup if necessary
}

// Getters
int Link::getSourceAddress() const {
    return sourceAddress;
}

int Link::getDestinationAddress() const {
    return destinationAddress;
}

int Link::getNeighbor() const {
    return destinationAddress; // For backward compatibility
}

int Link::getBandwidth() const {
    return bandwidth;
}

// Setters
void Link::setSourceAddress(int source) {
    sourceAddress = source;
}

void Link::setDestinationAddress(int destination) {
    destinationAddress = destination;
}

void Link::setBandwidth(int bw) {
    bandwidth = bw;
}