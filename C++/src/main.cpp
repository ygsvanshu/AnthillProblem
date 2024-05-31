#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <ctime>

#include "polynomial/term.h"
#include "polynomial/polyfunc.h"
#include "polynomial/polynomial.h"
#include "grid/gridnode.h"
#include "grid/gridfunc.h"
#include "grid/grid.h"
#include "functions/functions.h"

int main (int argc, char *argv[])
{
    // Default values for parameters
    bool restart = false;
    bool useHistory = false;
    int timesteps = 1000000;
    int maxiter = 1000000;
    int printout = 10;
    double speed = 1.0;
    double tolerance = 1e-6;
    double convergence = 1e-6;
    std::string polyPath = "./boundary.in";
    std::string gridPath = "./grid.txt";
    std::string dataPath = "./result.out";

    // Variables
    std::string argument;
    std::string line;
    double crossedTime;
    double crossedProb;
    double internalProb;
    std::ofstream dataFile;
    Grid grid;
    int t;
    time_t currentTime;
    clock_t clockTime0;
    clock_t clockTime1;

    // Start main computation

    currentTime = time(NULL);
    line = std::string(ctime(&currentTime));
    line.erase(line.end()-1);

    clockTime0 = clock();
    
    for (int i=0; i<argc; i++)
    {
        argument = argv[i];
        if (argument.compare("--restart") == 0) {restart=true;}
        if (argument.compare("--history") == 0) {useHistory=true;}
        if (argument.compare(0, 12, "--timesteps=") == 0) {timesteps=std::stoi(argument.substr(12));}
        if (argument.compare(0, 13, "--iterations=") == 0) {maxiter=std::stoi(argument.substr(13));}
        if (argument.compare(0, 11, "--printout=") == 0) {printout=std::stoi(argument.substr(11));}
        if (argument.compare(0, 8, "--speed=") == 0) {speed=std::stod(argument.substr(8));}
        if (argument.compare(0, 12, "--tolerance=") == 0) {tolerance=std::stod(argument.substr(12));}
        if (argument.compare(0, 14, "--convergence=") == 0) {convergence=std::stod(argument.substr(14));}
        if (argument.compare(0, 11, "--boundary=") == 0) {polyPath=std::stod(argument.substr(11));}
        if (argument.compare(0, 13, "--checkpoint=") == 0) {gridPath=std::stod(argument.substr(13));}
        if (argument.compare(0, 9, "--output=") == 0) {dataPath=std::stod(argument.substr(9));}
    }

    // Print out the inputs 

    std::cout << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << std::endl;

    if (restart)
    {
        std::cout << "Beginning calculation\n" << std::endl;
        dataFile.open(dataPath);
        grid = Grid();
        t = 0;
        crossedProb = 0.0;
        crossedTime = 0.0;
    }
    else
    {   
        try
        {
            std::cout << "Continuing calculation\n" << std::endl;
            grid = Grid(gridPath);
            readLastLine(dataPath, t, crossedTime, crossedProb);
            dataFile.open(dataPath, std::ios_base::app);
            timesteps += t;
            std::cout << "Last Step   = " << t << std::endl;
            std::cout << "Mean Time   = " << crossedTime << std::endl;
            std::cout << "Mean Prob   = " << crossedProb << std::endl;
            std::cout << std::endl;
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << '\n';
            std::cout << "Beginning calculation\n" << std::endl;
            dataFile.open(dataPath);
            grid = Grid();
            t = 0;
            crossedProb = 0.0;
            crossedTime = 0.0;
        }
    }

    grid.updateInternal(internalProb);

    Polynomial boundary = Polynomial(polyPath);
    boundary.normalize(speed);

    Grid histNorth = Grid();
    Grid histSouth = Grid();
    Grid histEast = Grid();
    Grid histWest = Grid();
    
    histNorth.nodes.clear();
    histSouth.nodes.clear();
    histEast.nodes.clear();
    histWest.nodes.clear();

    Grid history[4] = {histNorth, histSouth, histEast, histWest};

    std::cout << "Speed       = " << speed << std::endl;
    std::cout << "Timesteps   = " << timesteps - t << std::endl;
    std::cout << "Iterations  = " << maxiter << std::endl;
    std::cout << "Tolerance   = " << tolerance << std::endl;
    std::cout << "Convergence = " << convergence << std::endl;
    std::cout << std::endl;
    std::cout << "Boundary polynomial (normalised): " << boundary.writePolynomial() << std::endl;
    std::cout << std::endl;

    if (internalProb>convergence)
    {
        std::cout << std::setw(8) << "Timestep";
        std::cout << std::setw(20) << "Number of nodes";
        std::cout << std::setw(24) << "Mean crossing time";
        std::cout << std::setw(24) << "Crossed probability";
        std::cout << std::setw(24) << "Internal probability";
        std::cout << std::setw(24) << "Total probability";
        std::cout << std::setw(24) << "Clock time per step";
        std::cout << std::setw(24) << "Elapsed clock time";
        std::cout << std::endl;
        
        dataFile << "****************************************************************" << std::endl;
        dataFile << "*       Calculation started on: " << line << "       *" << std::endl;
        dataFile << "****************************************************************" << std::endl;
    
        dataFile << std::setw(8) << "Timestep";
        dataFile << std::setw(20) << "Number of nodes";
        dataFile << std::setw(24) << "Mean crossing time";
        dataFile << std::setw(24) << "Crossed probability";
        dataFile << std::endl;
    }
        
    while ((t<timesteps) && (internalProb>convergence))
    {
        clockTime1 = clock();

        grid.advanceStep();
        grid.updateExternal(boundary, t+1, crossedTime, crossedProb, tolerance, maxiter, history, useHistory);
        grid.updateInternal(internalProb);
        
        if (((t+1)%printout)==0)
        {
            std::cout << std::setw(8) << t+1; 
            std::cout << std::setw(20) << grid.nodes.size();
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << crossedTime;
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << crossedProb;
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << internalProb;
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << crossedProb + internalProb;
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << writeTime((double)(clock() - clockTime1)/CLOCKS_PER_SEC);
            std::cout << std::setw(24) << std::setprecision(16) << std::fixed << writeTime((double)(clock() - clockTime0)/CLOCKS_PER_SEC);
            std::cout << std::endl;
        }

        dataFile << std::setw(8) << t+1;
        dataFile << std::setw(20) << grid.nodes.size();
        dataFile << std::setw(24) << std::setprecision(16) << std::fixed << crossedTime;
        dataFile << std::setw(24) << std::setprecision(16) << std::fixed << crossedProb;
        dataFile << std::endl;
        
        t++;
    }

    std::cout << std::endl;

    if (t>=timesteps)
    {
        std::cout << "Number of timesteps exceeded" << std::endl;
    }
    if (internalProb<=convergence)
    {
        std::cout << "Solution converged" << std::endl;
    }

    grid.saveGrid(gridPath);

    std::cout << std::endl;
    std::cout << "****************************************************************" << std::endl;
    std::cout << std::endl;

}