#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <ctype.h>

#include "functions.h"

void readLastLine(std::string filePath, int &tstep, double &crossedTime, double &crossedProb)
{
    std::ifstream readFile;
    std::string line;
    std::stringstream sline;
    std::string number;
    std::vector<std::string> numbers;
    char c;
    int i;

    readFile.open(filePath);
    readFile.seekg(0,readFile.end);
    c = ' ';
    i = readFile.tellg();
    while ((!isdigit(c)) && (i>0))
    {
        readFile.seekg(-1,readFile.cur);
        c = readFile.peek();
        i--;
    }

    while ((c!='\n') && (i>0))
    {
        readFile.seekg(-1,readFile.cur);
        c = readFile.peek();
        line.push_back(c);
        i--;
    }
    std::reverse(line.begin(),line.end()-1);
    // std::cout << "Line -> " << line << std::endl;

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
        tstep = std::stoi(numbers[0]);
        crossedTime = std::stod(numbers[1]);
        crossedProb = std::stod(numbers[2]);
    }
    readFile.close();
}

std::string writeTime(double seconds)
{
    int h;
    int m;
    int s;

    double f;

    std::stringstream timeStream;

    s = (int)seconds;
    h = s/3600;
    m = (s - (3600*h))/60;
    s = (s - (3600*h) - (60*m));
    f = seconds - (3600.0*double(h)) - (60.0*double(m));

    timeStream << std::setfill('0') << std::setw(2) << std::fixed << h << ":";
    timeStream << std::setfill('0') << std::setw(2) << std::fixed << m << ":";
    timeStream << std::setfill('0') << std::setw(9) << std::setprecision(6) << std::fixed << f;
 
    return(timeStream.str());
}