#ifndef ALGO_H
#define ALGO_H

#include "matutil.h"

E detRec(Matrix m);
E detTri(Matrix m);
Matrix invertComat(Matrix m);
Matrix invertGauss(Matrix m);
Matrix triU(Matrix m);
Matrix triL(Matrix m);
Matrix PLU();


#endif // ALGO_H
