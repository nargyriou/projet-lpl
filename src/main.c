#include <stdio.h>
#include <ncurses.h>

#include "algo.h"
#include "interface.h"

int main()
{
    //Matrix m1 = newM(3, 3, 9,
    //                 3.0, -2.0, 4.0,
    //                 2.0, -4.0, 5.0,
    //                 1.0,  8.0, 2.0);
    Matrix m1 = newM(3, 3, 9,
    		 2.0, -1.0,  0.0,
    		-1.0,  2.0, -1.0,
    		 0.0, -1.0,  2.0);

    printf("newM:\n");
    printMatrix(m1);

    printf("id et mult:\n");
    Matrix identite = id(3);
    Matrix m2 = multiplication(m1, identite);
    printMatrix(m2);

    printf("triU:\n");
    triU(m2);
    printMatrix(m2);

    printf("switch_rows 2&3:\n");
    switch_rows(m2, 1, 2);
    printMatrix(m2);

    printf("add_rows:\n");
    add_rows(m2, 2, 1, 2);
    printMatrix(m2);

    printf("Suppression... ");
    fflush(stdout); 
    // ça print maintenant au cas ou on segfault juste après lel
    deleteMatrix(m1);
    deleteMatrix(m2);
    deleteMatrix(identite);
    printf("OK.\n");

    // sorcellerie ncurses

    initscr();

    Matrix m3 = scan_matrix();

    endwin();

    printMatrix(m3);

    return 0;

}
