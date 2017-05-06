#include "matutil.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define F_BLACK   "\x1b[30m"
#define F_RED     "\x1b[31m"
#define F_GREEN   "\x1b[32m"
#define F_YELLOW  "\x1b[33m"
#define F_BLUE    "\x1b[34m"
#define F_MAGENTA "\x1b[35m"
#define F_CYAN    "\x1b[36m"
#define F_WHITE   "\x1b[37m"
#define B_BLACK   "\x1b[40m"
#define B_RED     "\x1b[41m"
#define B_GREEN   "\x1b[42m"
#define B_YELLOW  "\x1b[43m"
#define B_BLUE    "\x1b[44m"
#define B_MAGENTA "\x1b[45m"
#define B_CYAN    "\x1b[46m"
#define B_WHITE   "\x1b[47m"
#define RESET     "\x1b[0m"
#define BOLD      "\x1b[1m"
#define UNDERLINE "\x1b[4m"

void err(int errnum)
{
    printf(F_RED BOLD UNDERLINE"Erreur:"RESET F_RED BOLD" ");
    switch (errnum) {
    case ERR_NULLMAT:
        printf("La matrice n'est pas définie.\n");
        break;
    case ERR_WRGID:
        printf("Indice invalide.\n");
        break;
    case ERR_WRGSIZE:
        printf("Mauvais format de matrice\n");
        break;
    default:
        printf("Autre erreur\n");
        break;
    }
    printf(RESET);
}

Matrix id(uint size)
{
    Matrix m = malloc(sizeof(struct matrix));
    m->nb_columns = size;
    m->nb_rows = size;
    m->mat = malloc(m->nb_rows*sizeof(E*));
    for(uint i=0;i<m->nb_rows;i++)
    {
        m->mat[i]=malloc(m->nb_columns*sizeof(E));
        for(uint j=0;j<m->nb_columns;j++)
            m->mat[i][j]=i==j?1:0;
    }
    return m;
}

Matrix newMEmpty(uint r, uint c)
{
    Matrix m = malloc(sizeof(struct matrix));
    m->nb_columns = c;
    m->nb_rows = r;
    m->mat = malloc(m->nb_rows*sizeof(E*));
    for(uint i=0;i<m->nb_rows;i++)
    {
        m->mat[i]=malloc(m->nb_columns*sizeof(E));
        for(uint j=0;j<m->nb_columns;j++)
            m->mat[i][j]=0;
    }
    return m;
}

Matrix newM(uint r, uint c, uint count, ...)
{
    Matrix m = malloc(sizeof(struct matrix));
    m->nb_columns = c;
    m->nb_rows = r;
    m->mat = malloc(m->nb_rows*sizeof(E*));
    va_list args;
    va_start(args, count);
    for(uint i=0;i<m->nb_rows;i++)
    {
        m->mat[i]=malloc(m->nb_columns*sizeof(E));
        for(uint j=0;j<m->nb_columns;j++)
        {
            E val = va_arg(args, double);
            set(m,i,j,val);
        }
    }
    va_end(args);
    return m;
}

E get(Matrix m,uint i, uint j)
{
    if(m!=NULL)
    {
        //printf("not null\n");
        if(i<m->nb_rows && j<m->nb_columns)
            return m->mat[i][j];
        else
        {
            err(ERR_WRGID);
            return 0;
        }
    }
    else{err(ERR_NULLMAT);return 0;}
}

void set(Matrix m, uint i, uint j, E val)
{
    if(m!=NULL)
    {
        if(i<m->nb_rows && j<m->nb_columns)
            m->mat[i][j]=val;
        else{err(ERR_WRGID);}

    }
    else{err(ERR_NULLMAT);}
}

uint rows(Matrix m)
{
    return m->nb_rows;
}

uint cols(Matrix m)
{
    return m->nb_columns;
}

void deleteMatrix(Matrix m)
{
    if(m!=NULL)
    {
        for(uint i=0;i<m->nb_rows;i++)
            free(m->mat[i]);
        free(m->mat);
        m->nb_rows = 0;
        m->nb_columns = 0;
        free(m);
    }
    else{err(ERR_NULLMAT);}
}

void printMatrix(Matrix m)
{
    if(m!=NULL)
    {
        for(uint i = 0;i<m->nb_rows;i++)
        {
            for(uint j=0;j<m->nb_columns;j++)
            {
                printf("%9f",get(m,i,j));
            }
            printf("\n");
        }
    }
    else{err(ERR_NULLMAT);}
}

bool isSymetric(Matrix m)
{
    if(m!=NULL)
    {
        if(m->nb_columns==m->nb_rows)
        {
            bool sym = true;
            for(uint i = 0;i<m->nb_rows;i++)
                for(uint j=i+1;j<m->nb_columns;j++)
                    if(get(m,i,j)!=get(m,j,i))
                        sym = false;
            return sym;
        }
        else
            return false;
    }
    else{err(ERR_NULLMAT);return false;}
}

bool isSquare(Matrix m)
{
    if(m!=NULL)
    {
        if(m->nb_columns==m->nb_rows)
            return true;
        else
            return false;
    }
    else{err(ERR_NULLMAT);return false;}
}

Matrix transpose(Matrix m)
{
    if(m!=NULL)
    {
        Matrix m2 = newMEmpty(m->nb_columns,m->nb_rows);
        for(uint i = 0;i<m->nb_rows;i++)
            for(uint j=0;j<m->nb_columns;j++)
                set(m2,j,i,get(m,i,j));
        return m2;
    }
    else{err(ERR_NULLMAT);return NULL;}
}

Matrix addition(Matrix m1, Matrix m2)
{
    if(m1!=NULL && m2!=NULL)
    {
        Matrix res = newMEmpty(m1->nb_columns,m2->nb_rows);
        if(m1->nb_columns == m2->nb_columns && m1->nb_rows == m2->nb_rows)
        {
            for(uint i = 0;i<m1->nb_rows;i++)
                for(uint j=0;j<m1->nb_columns;j++)
                    set(res,i,j,get(m1,i,j)+get(m2,i,j));
            return res;
        }
        else
        {
            err(ERR_WRGSIZE);
            return NULL;
        }
    }
    else{err(ERR_NULLMAT);return NULL;}
}

Matrix multiplication(Matrix m1,Matrix m2)
{
    if(m1!=NULL && m2!=NULL)
    {
        if(m1->nb_columns == m2->nb_rows)
        {
            Matrix res = newMEmpty(m1->nb_rows,m2->nb_columns);
            for(uint i = 0;i<res->nb_rows;i++)
            {
                for(uint j=0;j<res->nb_columns;j++)
                {
                    float sum = 0;
                    for(uint k=0;k<m1->nb_columns;k++)
                    {
                        sum+=get(m1,i,k)*get(m2,k,j);
                    }
                    set(res,i,j,sum);
                }
            }
            return res;
        }
        else
        {
            err(ERR_WRGSIZE);
            return NULL;
        }
    }
    else{err(ERR_NULLMAT);return NULL;}
}

Matrix scalar(Matrix m, E scalar)
{
    if(m!=NULL)
    {
        Matrix res = newMEmpty(m->nb_rows,m->nb_columns);
        for(uint i = 0;i<m->nb_rows;i++)
            for(uint j=0;j<m->nb_columns;j++)
                set(res,i,j,scalar*get(m,i,j));
        return res;
    }
    else{err(ERR_NULLMAT);return NULL;}
}

Matrix extract(Matrix m, uint i, uint j)
{
    Matrix ext = newMEmpty(m->nb_rows-1, m->nb_columns-1);
    uint ei,ej,fj;
    uint fi = 0;
    for(ei = 0; ei < m->nb_rows; ei++)
    {
        if(ei != i)
        {
            fj = 0;
            for(ej = 0; ej < m->nb_columns; ej++)
            {
                if(ej != j)
                {
                    set(ext,fi,fj,get(m,ei,ej));
                    fj++;
                }
            }
            fi++;
        }
    }
    return ext;

}
