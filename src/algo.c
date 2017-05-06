#include "algo.h"
#include <stdlib.h>
#include <stdio.h>

E detRec(Matrix m)
{
    if(m->nb_columns == 2 && m->nb_rows==2)
    {
        return get(m,0,0)*get(m,1,1)-get(m,0,1)*get(m,1,0);
    }
    else
    {
        int signe = 1;
        E d=0;
        for(uint i=0;i<m->nb_rows;i++)
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
        Matrix comat = newMEmpty(m->nb_rows,m->nb_columns);
        for(uint i=0; i<m->nb_rows; i++)
            for(uint j=0; j<m->nb_columns; j++)
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
