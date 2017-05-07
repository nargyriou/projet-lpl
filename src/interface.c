#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
//#include <term.h>

#include "interface.h"
#include "string_split.h"

// Royaume de la sorcellerie
#define clear()      fprintf(stderr, "\033[H\033[J")
#define move(x,y)    fprintf(stderr, "\033[%d;%dH", (x), (y))
// Contrée de la magie noire
#define printw(x...) fprintf(stderr, x)
// Fin du royaume

// pour pouvoir rediriger proprement le programme
#define NCURSES_TO_STDERR

void printw_matrix(Matrix m){
    printw("Your matrix:\n");

    uint sizeY = rows(m);
    uint sizeX = cols(m);
    E val;

    for (uint i = 0; i < sizeY; ++i)
    {
        printw("  (");
        for (uint j = 0; j < sizeX; ++j){
            val = get(m, i, j);
            if (val == 0)
                printw("          ");
            else
                printw("%9f ", val);
        }
        printw(")\n");
    }
}

char* scan_line(){
    int lu = 0;
    char* cmd = NULL;
    unsigned long zero = 0;

    lu = getline(&cmd, &zero, stdin);

    if (lu < 0)
        return NULL;

    cmd[--lu] = '\0';

    if (lu <= 0){
        return NULL;
    }

    return cmd;
}

char* scan_next_word(){
    static char** words = NULL;
    static int i = -1;
    char* str;
    
    // If end of the buffer
    if (words && words[i] == NULL){
        // Empty it
        free_split(words);
        words = NULL;
    }

    // If nothing in the buffer
    if (words == NULL){
        // Fill it
        str = scan_line();
        words = string_split(str, " ");
        i=0;
    }

    return words[i++];
}

Matrix scan_matrix(){
    char* word;
    uint rows;
    uint columns;
    uint MaxY, MaxX;
    struct winsize w;
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    MaxY = w.ws_row;
    MaxX = w.ws_col;

    printw("%d, %d\n", MaxX, MaxY);

    printw("Please, enter your matrix dimensions:\n");

    printw("Rows: ");
    word = scan_next_word();
    printw("%s\n", word);
    rows = atoi(word);

    printw("Columns: ");
    word = scan_next_word();
    printw("%s\n", word);
    columns = atoi(word);

    Matrix m = newMEmpty(rows, columns);

    for (uint i = 0; i < rows; ++i)
    {
        for (uint j = 0; j < columns; ++j)
        {
            clear();
            move(0, 0);
            printw_matrix(m);

            move(MaxY-1, 0);
            printw("Number: ");
            set(m, i, j, strtod(scan_next_word(), NULL));

        }
    }

    return m;
}