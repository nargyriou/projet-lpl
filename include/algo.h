#ifndef ALGO_H
#define ALGO_H

#include "matutil.h"

E detRec(Matrix m);
E detTri(Matrix m);
Matrix invertComat(Matrix m);
Matrix invertGauss(Matrix m);
void triU(Matrix m);
void triL(Matrix m);
Matrix PLU();


// Fonctions relatives au pivot de Gauss
void scalar_row(Matrix m, uint row, E scal);
void switch_rows(Matrix m, uint row1, uint row2);
void add_rows(Matrix m, uint result, uint row1, uint row2);


#endif // ALGO_H
