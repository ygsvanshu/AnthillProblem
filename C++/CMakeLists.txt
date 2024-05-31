cmake_minimum_required(VERSION 3.19)
project(OptiverProblem)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

#MAIN
add_executable(runcalc 
    src/main.cpp 
    src/polynomial/term.h 
    src/polynomial/term.cpp 
    src/polynomial/polyfunc.h 
    src/polynomial/polyfunc.cpp 
    src/polynomial/polynomial.h 
    src/polynomial/polynomial.cpp 
    src/grid/gridnode.h 
    src/grid/gridnode.cpp 
    src/grid/gridfunc.h 
    src/grid/gridfunc.cpp
    src/grid/grid.h 
    src/grid/grid.cpp
    src/functions/functions.h
    src/functions/functions.cpp
)