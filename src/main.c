#include <stdio.h>
#include "algo.h"

int main()
{
    Matrix m1 = newM(3, 3, 9,
                     3.0, -2.0, 4.0,
                     2.0, -4.0, 5.0,
                     1.0,  8.0, 2.0);

    printMatrix(m1);

    Matrix identite = id(3);

    Matrix m2 = multiplication(m1, identite);

    printMatrix(m2);

    deleteMatrix(m1);
    deleteMatrix(m2);
    deleteMatrix(identite);
    return 0;
}
