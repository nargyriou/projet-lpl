#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "algo.h"
#include "interface.h"

void print_help(){
    printw("matrix: Create a new matrix");
}

int readline(){
    char* cmd = NULL;
    Matrix m;

    cmd = scan_next_word();

    if (cmd == NULL){
        printw("Exit ? y/n ");
        cmd = scan_next_word();
        if (cmd == NULL || strcasecmp(cmd, "y") == 0){
            return 0;
        }
        else
            printf("'%p'\n", cmd);
    }
    else if (strcasecmp(cmd, "matrix") == 0){
        m = scan_matrix();
        if (m){
            printMatrix(m);
            deleteMatrix(m);
        }
    }
    else {
        printw("Unknown command: %s\n", cmd);
    }

    return 1;
}

int main()
{
    while (readline() > 0);

    return 0;
}
