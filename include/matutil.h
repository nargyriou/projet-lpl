#ifndef MATUTIL_H
#define MATUTIL_H

#define ERR_WRGID   0x10
#define ERR_NULLMAT 0x11
#define ERR_WRGSIZE 0x12

#include <stdbool.h>

typedef double E;
typedef unsigned int uint;

//typedef enum {
//	false,
//	true
//} bool;

typedef struct matrix {
    E **mat;
    uint nb_rows;
    uint nb_columns;
} *Matrix;

Matrix newMEmpty(uint r, uint c);
Matrix newM(uint r, uint c, uint count, ...);
Matrix id(uint size);

E get(Matrix m, uint i, uint j);
void set(Matrix m, uint i, uint j, E val);
uint rows(Matrix m);
uint cols(Matrix m);

E*   getRow(Matrix m, uint i);
void setRow(Matrix m, uint i, E* row);

void deleteMatrix(Matrix m);
void printMatrix(Matrix m);
bool isSymetric(Matrix m);
bool isSquare(Matrix m);
Matrix transpose(Matrix m);
Matrix addition(Matrix m1, Matrix m2);
Matrix multiplication(Matrix m1, Matrix m2);
Matrix scalar(Matrix m, E scalar);
Matrix extract(Matrix m, uint i, uint j);



#endif // MATUTIL_H
