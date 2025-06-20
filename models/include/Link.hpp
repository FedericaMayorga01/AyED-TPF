#ifndef LINK_HPP
#define LINK_HPP

class Link {
private:
    int sourceAddress;
    int destinationAddress;
    int bandwidth;

public:
    // Constructors
    Link(int source, int destination, int bandwidth = 100);
    Link(int neighbor, int bandwidth); // Keep for backward compatibility
    
    // Destructor
    ~Link();

    // Getters
    int getSourceAddress() const;
    int getDestinationAddress() const;
    int getNeighbor() const; // Keep for backward compatibility
    int getBandwidth() const;
    
    // Setters
    void setSourceAddress(int source);
    void setDestinationAddress(int destination);
    void setBandwidth(int bandwidth);
};

#endif // LINK_HPP