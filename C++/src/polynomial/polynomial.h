#ifndef POLYNOMIAL_H
#define POLYNOMIAL_H

class Polynomial
{
public:
    // Variables
    std::vector<Term> polynomial;
    // Constructors
    Polynomial(std::vector<Term> polynomial);
    Polynomial(std::string filepath);
    // Destructors
    ~Polynomial();
    // Methods
    void cleanPolynomial();
    double evaluatePolynomial(double x, double y);
    void normalize(double speed);
    std::string writePolynomial();
};

#endif