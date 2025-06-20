#ifndef LINK_HPP
#define LINK_HPP

// Headers

class Link {
    private:
    int neighbor;
    int bandwidth;

    public:
        // Constructor
        Link(int neighbor, int bandwidth);
        // Destructor
        ~Link();

        // Getters
        int getNeighbor() const;
        int getBandwidth() const;
};

#endif // LINK_HPP