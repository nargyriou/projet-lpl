#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

#include "algo.h"
#include "interface.h"



Matrix readline(){
    int lu = 0;
    char* cmd = NULL;
    unsigned long zero = 0;
    Matrix m;

    fflush(stdout);
    lu = getline(&cmd, &zero, stdin);

    if (lu < 0)
        return NULL;

    cmd[--lu] = '\0';

    if (lu <= 0){
        return NULL;
    }
    else if (strcmp(cmd, "scan") == 0){
        m = scan_matrix();
        return m;
    }

    return NULL;
}

int main()
{
    Matrix m3 = NULL;

    m3 = readline();

    if (m3 != NULL){
        printMatrix(m3);
        deleteMatrix(m3);
    }

    return 0;
}
