#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/ioctl.h>
//#include <term.h>

#include "interface.h"
#include "string_split.h"

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

char* scan_next_word(){
    static char* buff = NULL;
    static int size = 256;
    int i = 0;
    char c;

    if (buff == NULL)
        buff = malloc(size);

    c = getchar();
    while (c == ' ' || c == '\n')
        c = getchar();

    if (c == EOF){
        // printf("NULL\n");
        buff[0] = '\0';
        return buff;
    }
    

    while (c != EOF && c != '\n' && c != ' '){
        buff[i++] = c;
        c = getchar();

        if (i >= size){
            size += 256;
            buff = realloc(buff, size);
        }
    }
    buff[i] = '\0';

    // printf("%s\n", buff);
    return buff;
}

Matrix scan_matrix(uint columns, uint rows){
    // char* word;

    uint MaxY; 
    // uint MaxX;
    struct winsize w;
    
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);
    MaxY = w.ws_row;
    // MaxX = w.ws_col;

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
            printw("\n");
        }
    }

    //free(word);

    return m;
}

