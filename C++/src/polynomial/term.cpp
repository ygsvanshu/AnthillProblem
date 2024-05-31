#include <cmath>
#include <string>
#include <sstream>
#include <iomanip>

#include "term.h"

Term::Term(double coef, int powx, int powy)
{
    Term::coef = coef;
    Term::powx = powx;
    Term::powy = powy;
}

Term::~Term()
{
}

std::string Term::writeTerm()
{
    std::stringstream textStream;
    if (Term::coef > 0.0)
    {
        textStream << " + ";
    }
    if (Term::coef < 0.0)
    {
        textStream << " - ";
    }
    if (Term::coef != 1.0)
    {
        textStream << std::setprecision(3) << abs(Term::coef);
    }
    if (Term::powx != 0)
    {
        textStream << "x";
        if (Term::powx > 1) 
        {
            textStream << "^" << Term::powx;
        }
    }
    if (Term::powy != 0) 
    {
        textStream << "y";
        if (Term::powy > 1.0) 
        {
            textStream << "^" << Term::powy;
        }
    }
    return(textStream.str());
}

double Term::evaluateTerm(double x, double y)
{
    double value;
    value = Term::coef*std::pow(x,Term::powx)*std::pow(y,Term::powy);
    return(value);
}