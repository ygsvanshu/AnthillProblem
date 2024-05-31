#include <cmath>
#include <string>

#include "term.h"

bool compareTerms(Term t1, Term t2)
{
    if ((t1.powx + t1.powy) > (t2.powx + t2.powy))
    {
        return(true);
    }
    else
    {
        if ((t1.powx + t1.powy) == (t2.powx + t2.powy))
        {
            if (t1.powx > t2.powx)
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
        else
        {
            return(false);
        }
    }
}