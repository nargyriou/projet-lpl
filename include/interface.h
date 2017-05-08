#ifndef __INTERFACE_H
#define __INTERFACE_H

// Royaume de la sorcellerie
#define clear()      fprintf(stderr, "\033[H\033[J")
#define move(x,y)    fprintf(stderr, "\033[%d;%dH", (x), (y))
// Contrée de la magie noire
#define printw(x...) fprintf(stderr, x)
// Fin du royaume

#include "matutil.h"

void printw_matrix(Matrix m);
char* scan_line();
char* scan_next_word();
Matrix scan_matrix();

#endif