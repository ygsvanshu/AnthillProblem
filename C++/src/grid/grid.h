#ifndef GRID_H
#define GRID_H

class Grid
{
public:
    // Variables
    std::vector<GridNode> nodes;
    // Constructors
    Grid();
    Grid(std::string filePath);
    // Destructors
    ~Grid();
    // Methods
    double distanceHistory(GridNode testNode, Polynomial boundary, char direction, double tol, int maxiter, bool useHistory);
    void advanceStep();
    void updateExternal(Polynomial polynomial, int tstep, double &crossedTime, double &crossedProb, double tol, int maxiter, Grid history[4], bool useHistory=false);
    void updateInternal(double &internalProb);
    std::string writeNodes();
    void saveGrid(std::string filePath);
};

#endif