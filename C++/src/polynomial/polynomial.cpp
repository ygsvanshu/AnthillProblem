#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cmath>
#include <string>

#include "term.h"
#include "polyfunc.h"
#include "polynomial.h"

Polynomial::Polynomial(std::vector<Term> polynomial)
{
    Polynomial::polynomial = polynomial;
    Polynomial::cleanPolynomial();
}

Polynomial::Polynomial(std::string filePath)
{
    std::ifstream polyFile;
    std::string line;
    std::string number;
    std::vector<std::string> numbers;
    polyFile.open(filePath);
    while(std::getline(polyFile, line))
    {
        std::stringstream sline(line);
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
                Polynomial::polynomial.push_back(Term(std::stod(numbers[0]),std::stoi(numbers[1]),std::stoi(numbers[2])));
            }
            catch(...)
            {
            }
        }
        numbers.clear();
    }
    polyFile.close();
    Polynomial::cleanPolynomial();
}

Polynomial::~Polynomial()
{
}

void Polynomial::cleanPolynomial()
{
    for(unsigned long int i = 0; i < Polynomial::polynomial.size(); i++)
    {
        if (Polynomial::polynomial[i].coef == 0)
        {
            Polynomial::polynomial.erase(Polynomial::polynomial.begin()+i);
            i--;
        }
    }
    std::sort(Polynomial::polynomial.begin(), Polynomial::polynomial.end(), compareTerms);
}

double Polynomial::evaluatePolynomial(double x, double y)
{
    double result = 0.0;
    for(unsigned long int i = 0; i < Polynomial::polynomial.size(); i++)
    {
        result += Polynomial::polynomial[i].evaluateTerm(x, y);
    }
    return(result);
}

void Polynomial::normalize(double value)
{
    int maxExponent = 0;
    for(unsigned long int i = 0; i < Polynomial::polynomial.size(); i++)
    {
        if ((Polynomial::polynomial[i].powx + Polynomial::polynomial[i].powy) > maxExponent)
        {
            maxExponent = Polynomial::polynomial[i].powx + Polynomial::polynomial[i].powy;
        }
    }
    for(unsigned long int i = 0; i < Polynomial::polynomial.size(); i++)
    {
        Polynomial::polynomial[i].coef = Polynomial::polynomial[i].coef*std::pow(value,(Polynomial::polynomial[i].powx + Polynomial::polynomial[i].powy - maxExponent));
    }
}

std::string Polynomial::writePolynomial()
{
    std::stringstream textStream;
    std::string textString;
    
    for(unsigned long int i = 0; i < Polynomial::polynomial.size(); i++)
    {
        textStream << Polynomial::polynomial[i].writeTerm();         
    }
    textStream << " = 0";
    textString = textStream.str();
    if (textString[1] == '+')
    {
        textString.erase(textString.begin(),textString.begin()+3);
    }
    return(textString);
}