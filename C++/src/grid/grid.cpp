#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cmath>

#include "../polynomial/term.h"
#include "../polynomial/polyfunc.h"
#include "../polynomial/polynomial.h"
#include "gridnode.h"
#include "gridfunc.h"
#include "grid.h"

Grid::Grid()
{
    // Initialize origin to have unit probability
    Grid::nodes.push_back(GridNode(0,0,1.0));
}

Grid::Grid(std::string filePath)
{
    std::ifstream gridFile;
    std::string line;
    std::stringstream sline;
    std::string number;
    std::vector<std::string> numbers;
    gridFile.open(filePath);
    while(std::getline(gridFile, line))
    {
        sline.clear();
        sline.str(line);
        while(std::getline(sline, number, ' '))
        {
            if (number!="") 
            {
                numbers.push_back(number);
            }
        }
        if (numbers.size() >= 3)
        {
            try
            {
                Grid::nodes.push_back(GridNode(std::stoi(numbers[0]),std::stoi(numbers[1]),std::stod(numbers[2])));
            }
            catch(...)
            {
            }
        }
        numbers.clear();
    }
    gridFile.close();
    // Default initialisation
    if (Grid::nodes.size() == 0)
    {
        Grid::nodes.push_back(GridNode(0,0,1.0));    
    }
}

Grid::~Grid()
{
    //
}

double Grid::distanceHistory(GridNode testNode, Polynomial boundary, char direction, double tol, int maxiter, bool useHistory)
{
    bool found = false;
    double crossedDist;

    if (useHistory)
    {
        for (GridNode currentNode : Grid::nodes)
        {
            if (currentNode.isCoincident(testNode))
            {
                found = true;
                crossedDist = currentNode.value;
            }

        }
        if (!found)
        {
            crossedDist = testNode.getDistance(boundary, direction, tol, maxiter);
            Grid::nodes.push_back(GridNode(testNode.position_x, testNode.position_y, crossedDist));
        }
    }
    else
    {
        crossedDist = testNode.getDistance(boundary, direction, tol, maxiter);
    }
    return(crossedDist);
}

void Grid::advanceStep()
{
    std::vector<GridNode> newNodes;
    newNodes.reserve(4*Grid::nodes.size());

    for (GridNode currentNode: Grid::nodes)
    {        
        // Travel North
        newNodes.push_back(GridNode(currentNode.position_x,currentNode.position_y+1,currentNode.value*0.25));
        // Travel South
        newNodes.push_back(GridNode(currentNode.position_x,currentNode.position_y-1,currentNode.value*0.25));
        // Travel East
        newNodes.push_back(GridNode(currentNode.position_x+1,currentNode.position_y,currentNode.value*0.25));
        // Travel West
        newNodes.push_back(GridNode(currentNode.position_x-1,currentNode.position_y,currentNode.value*0.25));
    }
    Grid::nodes.swap(newNodes);
}

void Grid::updateExternal(Polynomial boundary, int tstep, double &crossedTime, double &crossedProb, double tol, int maxiter, Grid history[4], bool useHistory)
{
    double crossedDist;
    char directions[4] = {'N','S','E','W'};
    
    std::vector<GridNode> newNodes;
    newNodes.reserve(Grid::nodes.size());

    int i = 0;
    for (GridNode currentNode : Grid::nodes)
    {
        if (currentNode.isExternal(boundary))
        {
            crossedDist = history[i%4].distanceHistory(currentNode, boundary, directions[i%4], tol, maxiter, useHistory);
            crossedTime = crossedTime + (double(tstep-1)+crossedDist)*currentNode.value;
            crossedProb = crossedProb + currentNode.value;
        }
        else
        {
            newNodes.push_back(currentNode);
        }
        i++;
    }
    Grid::nodes.swap(newNodes);
}

void Grid::updateInternal(double &internalProb)
{

    unsigned long int j;
    std::vector<GridNode> newNodes;

    internalProb = 0.0;
    
    std::sort(Grid::nodes.begin(), Grid::nodes.end(), compareGridNodes);

    newNodes.reserve(Grid::nodes.size());
    newNodes.clear();
    newNodes.push_back(Grid::nodes.front());

    internalProb += Grid::nodes.front().value;
    
    j = 0;
    for (unsigned long int i = 1; i < Grid::nodes.size(); i++)
    {
        if (Grid::nodes[i].isCoincident(Grid::nodes[i-1]))
        {
            newNodes[j].value += Grid::nodes[i].value;
        }
        else
        {
            newNodes.push_back(Grid::nodes[i]);
            j += 1;
        }
        internalProb += Grid::nodes[i].value;
    }
    Grid::nodes.swap(newNodes);
}

std::string Grid::writeNodes()
{
    std::stringstream textStream;
    for (GridNode currentNode : Grid::nodes)
    {
        textStream << currentNode.writeNode() << std::endl;
    }
    return(textStream.str());
}

void Grid::saveGrid(std::string filePath)
{
    std::ofstream gridFile;
    int maxDigit;
    int curDigit;
    gridFile.open(filePath);

    maxDigit = 0;
    for (GridNode currentNode : Grid::nodes)
    {
        curDigit = int(std::log10(currentNode.position_x)) + 1;
        if (curDigit > maxDigit) {maxDigit = curDigit;}
        curDigit = int(std::log10(currentNode.position_y)) + 1;
        if (curDigit > maxDigit) {maxDigit = curDigit;}
    }

    maxDigit = 2*(maxDigit/2 + 2);

    gridFile << std::setw(maxDigit) << "x";
    gridFile << std::setw(maxDigit) << "y";
    gridFile << std::setw(24) << "value";
    gridFile << std::endl;
    for (GridNode currentNode : Grid::nodes)
    {
        gridFile << std::setw(maxDigit) << std::fixed << currentNode.position_x;
        gridFile << std::setw(maxDigit) << std::fixed << currentNode.position_y;
        gridFile << std::setw(24) << std::setprecision(16) << std::scientific << currentNode.value;
        gridFile << std::endl;
    }
    gridFile.close();
}