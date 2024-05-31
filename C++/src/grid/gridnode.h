#ifndef GRIDNODE_H
#define GRIDNODE_H

class GridNode
{
public:
    // Variables
    int position_x;
    int position_y;
    double value;
    // Constructors
    GridNode(int position_x, int position_y, double value);
    // Destructors
    ~GridNode();
    // Methods
    bool isCoincident(GridNode node);
    bool isExternal(Polynomial boundary);
    double getDistance(Polynomial boundary, char direction, double tol, int maxiter);
    std::string writeNode();
};

#endif