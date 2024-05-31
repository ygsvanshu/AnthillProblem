********************************************************************************************************************************
*                                                   Anthill Problem (C++)                                                      *
*                                           Guru Sreevanshu Yerragolam, 30 May 2024                                            *
********************************************************************************************************************************

+-------------------+
| Problem statement |
+-------------------+
The code solves the following problem:
Given that an ant starts at origin and moves with a given speed every second randomly towards either North (+y), South (-y), East (+x), or West (-x), how long does it take on average to reach food kept on the boundary defined by a supplied polynomial equation.

+---------------+
| Solution idea |
+---------------+
The idea for the solution is as follows:
1. Assume a large number "n" of ants are started at origin at time t=0.
2. Consider a grid of "nodes" of coordinates (i*speed, j*speed) where i, j are integers.
3. At each subsequent second, a quarter of them are likely to move in each of the four cardinal directions from their current position.
4. Track the probability of an ant being at any "node" given by the fraction likely to move to the node while evolving timesteps.
5. When a node that is outside the polynomial is assigned a non-zero probability, flag this as a "crossing".
6. Compute the crossing distance and time required to reach the polynomial boundary (which may not be equal to speed*timestep).
7. Add the total time required to reach the boundary weighted by the probability assigned to the crossed node.
8. Erase the crossed node.
9. Perform steps 4-8 until the fraction crossed is sufficiently close to unity to obtain the mean expected crossing time.

+-------------------+
| Building the code |
+-------------------+
The code build uses CMake to prepare the Makefile.
CMakeLists.txt is already included in the root directory.
1. Navigate to the build directory
2. Enter the following in a unix style terminal
    $ cmake ../ -DCMAKE_BUILD_TYPE=Release
    $ make clean
    $ make
3. Executable runcalc should now be generated in build directory

+------------------+
| Running the code |
+------------------+
The code already contains default inputs for running, therefore the direct invocation
    $ ./runcalc
should work given that the boundary polynomial data is provided in boundary.in.

The boundary polynomial data is entered as a set of lines, each containing a term consisting of a floating point coefficient, an integer power of x and an integer power of y. The equation for the boundary is given by equating the sum of terms to zero. Note that the x-y plane represents the flat surface over which the ants travel. For examples, see the examples directory.

The other inputs are tabulated as follows:
Input                       Command line tag    Default value   Explanation
-----                       ----------------    -------------   -----------
Restart calculation         --restart           False           Whether to restart the calculation or 
                                                                to continue from an existing checkpoint.
Use crossing history        --history           False           Whether to note the crossing distances for quicker calculation
                                                                (faster but uses more memory).
Number of timesteps         --timesteps=        1000000         Number of timesteps to evolve the probability nodes.
Number of iterations        --iterations=       1000000         Number of iterations for the numerical root finding function.
                                                                that computes the crossing distance to the boundary.
Console output frequency    --printout=         100             Number of timesteps between each console output.
Speed of ants               --speed=            1.0             Speed at which ants travel.
Tolerance                   --tolerance=        1.0e-6          The distance tolerance in determining a boundary crossing
                                                                i.e., in the numerical root finding function.
Convergence                 --convergence=      1.0e-6          The value of the probability left uncrossed at which point
                                                                the computation is assumed to be converged. 
Boundary file path          --boundary=         ./boundary.in   The path of the boundary.in file.
Grid checkpoint file path   --checkpoint=       ./grid.txt      The path for the file to save the grid checkpoint.
Time series data file       --output=           ./result.out    The path for the file to write the time series data.

Once the calculation is completed, the code stores the time-series data in results.out file and saves a snapshot of the grid in grid.txt file as a checkpoint for futher calculation. If the calculation is started with a --restart command line flag, these files are overwritten at the end of the calculation. If the calculation is started without the flag, by default the code attempts to read in these files and continue the calculation from the last checkpoint. If unsuccesful, the calculation is started from scratch.

+---------------+
| Documentation |
+---------------+
To be completed ... 