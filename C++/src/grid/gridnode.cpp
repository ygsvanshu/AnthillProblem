#include <string>
#include <sstream>
#include <vector>

#include "../polynomial/term.h"
#include "../polynomial/polyfunc.h"
#include "../polynomial/polynomial.h"
#include "gridnode.h"

GridNode::GridNode(int position_x, int position_y, double value)
{
    GridNode::position_x = position_x;
    GridNode::position_y = position_y;
    GridNode::value = value;
}

GridNode::~GridNode()
{
}

bool GridNode::isCoincident(GridNode node)
{
    if ((GridNode::position_x==node.position_x) && (GridNode::position_y==node.position_y))
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

bool GridNode::isExternal(Polynomial boundary)
{
    double originValue = boundary.evaluatePolynomial(0.0,0.0);
    double gpointValue = boundary.evaluatePolynomial(double(GridNode::position_x),double(GridNode::position_y));
    if ((gpointValue*originValue) <= 0)
    {
        return(true);
    }
    else
    {
        return(false);
    }
}

double GridNode::getDistance(Polynomial boundary, char direction, double tol, int maxiter)
{
    double xi = double(GridNode::position_x);
    double yi = double(GridNode::position_y);
    
    double x1 = xi;
    double y1 = yi;

    double x2;
    double y2;

    double xn;
    double yn;

    double v0;
    double v1;
    double v2;
    double vn;

    int iter = 0;
    double ds = tol*10;

    if (direction=='N') 
    {
        x2 = x1;
        y2 = y1 - 1.0;
    }

    if (direction=='S') 
    {
        x2 = x1;
        y2 = y1 + 1.0;
    }

    if (direction=='E') 
    {
        x2 = x1 - 1.0;
        y2 = y1;
    }

    if (direction=='W') 
    {
        x2 = x1 + 1.0;
        y2 = y1;
    }

    v1 = boundary.evaluatePolynomial(x1, y1);
    if (v1==0)
    {
        return(1.0);
    }
    v2 = boundary.evaluatePolynomial(x2, y2);
    if (v2==0)
    {
        return(0.0);
    }

    while ((ds > tol) && (iter < maxiter))
    {
        v0 = boundary.evaluatePolynomial(0.0, 0.0);
        v1 = boundary.evaluatePolynomial(x1, y1);
        v2 = boundary.evaluatePolynomial(x2, y2);
    
        xn = (x1*abs(v2) + x2*abs(v1))/(abs(v1) + abs(v2));
        yn = (y1*abs(v2) + y2*abs(v1))/(abs(v1) + abs(v2));

        vn = boundary.evaluatePolynomial(xn, yn);

        if ((vn*v0)<0)
        {
            ds = std::pow((std::pow((x1 - xn),2.0) + std::pow((y1 - yn),2.0)),0.5);
            x1 = xn;
            y1 = yn;   
        }

        if ((vn*v0)>0)
        {
            ds = std::pow((std::pow((x2 - xn),2.0) + std::pow((y2 - yn),2.0)),0.5);
            x2 = xn;
            y2 = yn;
        }

        if (vn==0)
        {
            ds = 0.0;
        }
        iter += 1;
        // std::cout << "Iteration " << iter << " [" << xn << ", " << yn << "] " << ds << std::endl;
    }

    ds = std::pow((std::pow((xi - xn),2.0) + std::pow((yi - yn),2.0)),0.5);
    return(1.0 - ds);
}

std::string GridNode::writeNode()
{
    std::stringstream textStream;
    textStream << "[" << GridNode::position_x << ", " << GridNode::position_y << "], " << GridNode::value;
    return(textStream.str());
}