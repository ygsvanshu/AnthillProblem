#ifndef TERM_H
#define TERM_H

class Term
{
public:
    // Variables
    double coef;
    int   powx;
    int   powy;
    // Constructors
    Term(double coef, int powx, int powy);
    // Destructors
    ~Term();
    // Methods
    std::string writeTerm();
    double evaluateTerm(double x, double y);
};

#endif