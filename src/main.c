#include <stdio.h>
#include "algo.h"

int main()
{
    Matrix m1 = newM(3  ,  3  , 9  ,
                     3.0, -2.0, 4.0,
                     2.0, -4.0, 5.0,
                     1.0,  8.0, 2.0);
    deleteMatrix(m1);
    return 0;
}
