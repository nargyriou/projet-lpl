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


#endif // ALGO_H
