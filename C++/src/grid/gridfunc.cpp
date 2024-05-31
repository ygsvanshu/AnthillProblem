#include <string>
#include <vector>

#include "../polynomial/term.h"
#include "../polynomial/polyfunc.h"
#include "../polynomial/polynomial.h"
#include "gridnode.h"
#include "gridfunc.h"

bool compareGridNodes(GridNode n1, GridNode n2)
{
    if (n1.position_y > n2.position_y)
    {
        return(true);
    }
    else if (n1.position_y < n2.position_y)
    {
        return(false);
    }
    else
    {
        if (n1.position_x > n2.position_x)
        {
            return(true);
        }
        else if (n1.position_x < n2.position_x)
        {
            return(false);
        }
        else
        {
            if (n1.value > n2.value)
            {
                return(true);
            }
            else
            {
                return(false);
            }
        }
    }
}