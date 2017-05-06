#include "algo.h"
#include <stdlib.h>
#include <stdio.h>

E detRec(Matrix m)
{
    if(cols(m) == 2 && rows(m)==2)
    {
        return get(m,0,0)*get(m,1,1)-get(m,0,1)*get(m,1,0);
    }
    else
    {
        int signe = 1;
        E d=0;
        for(uint i=0;i<rows(m);i++)
        {
            Matrix ext = extract(m,i,0);
            d+=signe*get(m,i,0)*detRec(ext);
            deleteMatrix(ext);
            signe*=-1;
        }
        return d;
    }
}

Matrix invertComat(Matrix m)
{
    int d = detRec(m);
    if(d != 0)
    {
        short signe = 1;
        Matrix comat = newMEmpty(rows(m),cols(m));
        for(uint i=0; i<rows(m); i++)
            for(uint j=0; j<cols(m); j++)
            {
                Matrix ext = extract(m,i,j);
                set(comat,i,j,signe*detRec(ext));
                signe *= -1;
                deleteMatrix(ext);
            }
        Matrix trans = transpose(comat);
        deleteMatrix(comat);
        Matrix invert = scalar(trans, 1/d);
        deleteMatrix(trans);
        return invert;
    }
    else
    {
        printf("err");
        return NULL;
    }
}

void triU(Matrix m)
{
    for (uint i = 0; i < cols(m); i++)
    {
        E coeffDiagonal = get(m, i, i);

        for (uint j = i+1; j < rows(m); j++)
        {
            E q = get(m, j, i);
            set(m, j, i, 0.0);

            for (uint k = i+1; k < cols(m); k++)
                set(m, j, k, get(m, j, k) - (get(m, i, k)*(q/coeffDiagonal)) );
        }
    }
}

// Fonctions relatives au pivot de gauss

