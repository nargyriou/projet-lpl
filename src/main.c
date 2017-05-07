#include <stdio.h>
#include <ncurses.h>
#include <unistd.h>

#include "algo.h"
#include "interface.h"

int main()
{
    // tout ce qui vient de ncurses doit sortir sur stderr
    int saved_stdout = dup(1);
    
    // je redirige stdout sur stderr
    dup2(2, 1);
    
    // sorcellerie ncurses
    initscr();

    Matrix m3 = scan_matrix();

    endwin();
    // fin sorcellerie

    // je restore stdout
    dup2(saved_stdout, 1);

    printMatrix(m3);
    deleteMatrix(m3);

    return 0;

}
