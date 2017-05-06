#include "interface.h"

#include <ncurses.h>

void printw_matrix(Matrix m){
    printw("Your matrix:\n");

    uint sizeY = rows(m);
    uint sizeX = cols(m);
    E val;
    sizeX--;

    for (uint i = 0; i < sizeY; ++i)
    {
        printw("(");
        for (uint j = 0; j < sizeX; ++j){
            val = get(m, i, j);
            if (val == 0)
                printw("          ");
            else
                printw("%9f ", val);
        }
        
        val = get(m, i, sizeX);
        if (val == 0)
            printw("         )\n");
        else
            printw("%9f)\n", val);
    }
}

E get_next_val(){
    E val;
    scanw("%lf", &val);
    return val;
}

Matrix scan_matrix(){
    uint rows;
    uint columns;
    uint MaxY;
    uint MaxX;
    (void)MaxX;
    (void)MaxY;

    getmaxyx(stdscr, MaxY, MaxX);
    printw("%d, %d\n", MaxX, MaxY);

    printw("Please, enter your matrix dimensions:\n");
    printw("Rows: ");
    scanw("%u", &rows);
    printw("Columns: ");
    scanw("%u", &columns);

    Matrix m = newMEmpty(rows, columns);

    for (uint i = 0; i < rows; ++i)
    {
        for (uint j = 0; j < columns; ++j)
        {
            move(0, 0);
            printw_matrix(m);

            move(MaxY-1, 0);
            clrtoeol(); //clear line
            printw("Number: ");
            set(m, i, j, get_next_val());

        }
    }

    return m;
}