#include "algo.h"
#include <stdlib.h>
#include <stdio.h>

E detRec(Matrix m)
{
    if(cols(m) == 1 && rows(m)==1)
    {
        return get(m,0,0);
    }
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

E detTri(Matrix m)
{
    Matrix tri = triU(m);
    E det = 1;
    for(uint i = 0; i<rows(m);i++){
        det *= get(m,i,i);
    }
    return det;
}

Matrix invertComat(Matrix m)
{
    E d = detRec(m);
    if(d != 0)
    {
        if(cols(m) == 2 && rows(m)==2)
        {
            Matrix inv = newM(2,2,4,
                                get(m,1,1),-get(m,0,1),
                                -get(m,1,0),get(m,0,0));
            return scalar(inv,1/d);
        }
        else
        {
            short signe = 1;
            Matrix comat = newMEmpty(rows(m),cols(m));
            for(uint i=0; i<rows(m); i++){
                for(uint j=0; j<cols(m); j++)
                {
                    Matrix ext = extract(m,i,j);
                    printMatrix(ext);
                    set(comat,i,j,signe*detRec(ext));
                    signe *= -1;
                    deleteMatrix(ext);
                }
            }
            Matrix trans = transpose(comat);
            deleteMatrix(comat);
            Matrix invert = scalar(trans, 1/d);
            deleteMatrix(trans);
            return invert;
        }
    }
    else
    {
        printf("err");
        return NULL;
    }
}

Matrix invertGauss(Matrix m)
{
    int c = 0;
    E coeff_diag = 0, coeff = 0;
    Matrix tmp1 = multiplication(m,id(rows(m)));
    Matrix tmp2 = id(rows(m));

    for (uint h = 0; h < rows(m); h++) {
        coeff_diag = get(tmp1, h, h);
        c = 0;

        while (abs (coeff_diag) < 0.000000001) {
            c++;
            
            for (uint i = 0; i < rows(m); i++) {
                set(tmp1, h, i, get(tmp1, h, i) + get(tmp1, h + c, i));
                set(tmp2, h, i, get(tmp2, h, i) + get(tmp2, h + c, i));
            }
            coeff_diag = get(tmp1, h, h);
        }
        
        for (uint j = 0; j < rows(m); j++) {
            set(tmp1, h, j, get(tmp1, h, j) / coeff_diag);
            set(tmp2, h, j, get(tmp2, h, j) / coeff_diag);
        }
        
        for (uint k = 0; k < rows(m); k++) {
            coeff = get(tmp1, k, h);
            
            if (k != h) {
                for (uint l = 0; l < rows(m); l++) {
                    set(tmp1, k, l,get (tmp1, k, l) - coeff * get(tmp1, h, l));
                    set(tmp2, k, l,get(tmp2, k, l) - coeff * get(tmp2, h, l));
                }
            }
        }
    }

    return tmp2;
}

Matrix triU(Matrix m)
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
    return m;
}

void triL(Matrix m)
{
    for (uint i = 0; i < cols(m); i++)
    {
        //E coeffDiagonal = get(m, i, i);

        for (uint j = 0; j < i+1; j++)
        {
            //E q = get(m, i, j);
            //set(m, i, j, 0.0);
            if(i==j){
                set(m,i,j,1);                                                                                                                                                                       
            }
            for (uint k = 0; k < i+1; k++)
                set(m, i, j, get(m, i, k) - (get(m, k, j)*get(m,k,j)));
        }
    }
}

void PLU(Matrix m)
{
    E p = 0, q = 0, tamp = 0;
    uint l;
    Matrix P = id(rows(m));
    Matrix L = id(rows(m)); 
    
    for (uint k = 0; k < rows(m); k++)
    {
        p = get(m,k,k); l = k;
        for(uint i = k; i < rows(m); i++)
        {
            if(abs(get(m,i,k)) > p)
            {
                p = get(m,i,k); l = i;
            }
        }
        if(l != k)
        {
            for(uint j = 0; j < rows(m); j++)
            {
                tamp = get(m,k,j); 
                set(m,k,j,get(m,l,j));
                set(m,l,j,tamp);
                if(j < k)
                {
                    tamp = get(L,k,j); 
                    set(L,k,j, get(L,l,j));
                    set(L,l,j, tamp);
                }
                tamp = get(P,k,j); 
                set(P,k,j,get(P,l,j));
                set(P,l,j, tamp);
            }
        }
        for(uint i = k+1; i < rows(m); i++)
        {
            q = get(m,i,k); 
            set(m,i,k, 0);
            set(L,i,k,q/p);
            for(uint j = k+1; j < rows(m); j++)
            {
                set(m,i,j, get(m,i,j) - (get(m,k,j)*(q/p)));
            }
        }
    }
    printMatrix(P);
    printMatrix(L);
    printMatrix(m);
}

// Fonctions relatives au pivot de gauss

void scalar_row(Matrix m, uint row, E scal)
{
    for (uint j=0; j < cols(m); j++)
    {
        E val = get(m, row, j);
        set(m, row, j, scal*val);
    }
}

void switch_rows(Matrix m, uint row1, uint row2){
    E* temp = getRow(m, row1);
    setRow(m, row1, getRow(m, row2));
    setRow(m, row2, temp);
}

void add_rows(Matrix m, uint result, uint row1, uint row2){
    for (uint j=0; j < cols(m); j++)
    {
        E val = get(m, row1, j) + get(m, row2, j);
        set(m, result, j, val);
    }
}