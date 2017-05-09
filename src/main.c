#define _GNU_SOURCE
#define MAX_USER_VAR 26

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>
#include <errno.h>
#include <math.h>

#include "algo.h"
#include "string_split.h"
#include "interface.h"


/**
 * User's variables
 * He can use them for calculations
 */

typedef struct userMatrix {
    char id;
    Matrix m;
} userMatrix;
userMatrix userMatrixes[MAX_USER_VAR];
int nuserMatrixes = 0;

char addUserMatrix(Matrix m){
    if (nuserMatrixes >= MAX_USER_VAR)
        return -1;

    userMatrixes[nuserMatrixes].m = m;
    userMatrixes[nuserMatrixes].id = nuserMatrixes+'A';
    return userMatrixes[nuserMatrixes++].id;
}

Matrix getUserMatrix(char id){
    for (int i = 0; i < MAX_USER_VAR; ++i){
        if (userMatrixes[i].id == id)
            return userMatrixes[i].m;
    }
    return NULL;
}

/**
 * Parsing
 */

int PARSE_ERROR;
void raler(char* msg, int n){
    static int doprint = 1;

    // Resetting if msg==NULL
    if (msg == NULL){
        doprint = 1;
        PARSE_ERROR = 0;
        return;
    }

    // Printing the warning
    printw("%s\n", msg);

    // Only erroring if it has an error code
    if (doprint && n){
        PARSE_ERROR = n;
        printf("ERROR\n");
        doprint = 0;
    }
}
void warning(char* msg){
    raler(msg, 0);
}

typedef enum tn {
    UNAIRE, BINAIRE, ERREUR, CONSTANTE, VARIABLE
} type_noeud;

typedef enum operation {
    AUCUNE, FETCH,

    // Opérations binaires
    ADD, MULT, SCALE,

    // Opérations unaires
    INV, GAUSS, TRANSPOSE, EXTRACT, TRIANGULE,

    // Opérations d'erreur
    INCONNU, MANQUANT
} operation;

typedef union noeud { 
    struct s_unaire* u;
    struct s_binaire* b;
    struct s_constante* c;
    struct s_variable* v;
    struct s_erreur* e;
    void* _v;
} noeud;

typedef struct s_binaire { // 68 (96)
    operation op;       // 4 (32)
    union noeud fg;    // 32
    union noeud fd;    // 32
} binaire;


typedef struct s_unaire { // 34 (64)
    operation op;   // 2 (32)
    union noeud f; // 32
} unaire;

typedef struct s_constante { // 64 (96)
    operation op; // 2 (32)
    double v;     // 64
} constante;

typedef struct s_variable {
    operation op;
    char id;
} variable;

typedef struct s_erreur {
    operation op;
} erreur;

type_noeud optotype(operation op){
    switch (op){
        case AUCUNE: 
            return CONSTANTE;
        case FETCH: 
            return VARIABLE;

        // Opérations binaires
        case ADD: 
            return BINAIRE;
        case MULT: 
            return BINAIRE;
        case SCALE: 
            return BINAIRE;

        // Opérations unaires
        case INV: 
            return UNAIRE;
        case GAUSS: 
            return UNAIRE;
        case TRANSPOSE: 
            return UNAIRE;
        case EXTRACT: 
            return UNAIRE;
        case TRIANGULE: 
            return UNAIRE;

        // Opérations d'erreur
        case INCONNU: 
            return ERREUR;
        case MANQUANT: 
            return ERREUR;
        default: 
            return ERREUR;
    }
}

char* optostr(operation op){
    switch (op){
        case AUCUNE:
            return "<AUCUNE>";
        case ADD: 
            return "ADD";
        case MULT: 
            return "MULT";
        case SCALE: 
            return "SCALE";

        // Opérations unaires
        case INV: 
            return "INVERSE";
        case GAUSS: 
            return "GAUSS";
        case TRANSPOSE: 
            return "TRANSPOSE";
        case EXTRACT: 
            return "EXTRACT";
        case TRIANGULE: 
            return "TRIANGULE";

        // Opérations d'erreur
        case MANQUANT:
            return "<MISSING>";
        default:
            return "<UNKNOWN>";
    }
}

operation strtoop(char* mot){
    if (mot[0] >= '0' && mot[0] <= '9')
        return AUCUNE;
    if (mot[0] == '-' || mot[0] == '+')
        if (mot[1] >= '0' && mot[1] <= '9')
            return AUCUNE;

    if (mot[0] >= 'A' && mot[0] <= 'Z')
        return FETCH;

    switch (mot[0]){
        case '+':
            return ADD;
        case '*':
            return MULT;
        case '.':
            return SCALE;
        default:
            break;
    }

    if (strcasecmp(mot, "inv") == 0)
        return INV;
    else if (strcasecmp(mot, "gauss") == 0)
        return GAUSS;
    else if (strcasecmp(mot, "transpose") == 0)
        return TRANSPOSE;
    else if (strcasecmp(mot, "extract") == 0)
        return EXTRACT;
    else if (strcasecmp(mot, "triangule") == 0)
        return TRIANGULE;

    return INCONNU;
}

operation get_operation(noeud n){
    return n.u->op;
}

type_noeud get_type(noeud n){
    operation op = get_operation(n);
    return optotype(op);
}

noeud new_constante(double v){
    constante* f = malloc(sizeof(struct s_constante));
    f->op = AUCUNE;
    f->v = v;
    return (noeud)f;
}

noeud new_variable(char id){
    variable* f = malloc(sizeof(struct s_variable));
    f->op = FETCH;
    f->id = id;
    return (noeud)f;
}

noeud new_unaire(operation op, noeud f){
    unaire* n = malloc(sizeof(struct s_unaire));
    n->op = op;

    n->f = f;
    return (noeud)n;
}

noeud new_binaire(operation op, noeud fg, noeud fd){
    binaire* f = malloc(sizeof(struct s_binaire));
    f->op = op;
    f->fg = fg;
    f->fd = fd;
    return (noeud)f;
}

noeud new_erreur(operation op){
    erreur* f = malloc(sizeof(struct s_erreur));
    f->op = op;
    return (noeud)f;
}

noeud fils_gauche(noeud n){
    if (get_type(n) == BINAIRE || get_type(n) == UNAIRE)
        return n.b->fg;
    return (noeud)NULL;
}

noeud fils_droit(noeud n){
    if (get_type(n) == BINAIRE)
        return n.b->fd;
    else
        return (noeud)NULL;
}

noeud fils(noeud n){
    if (get_type(n) == BINAIRE || get_type(n) == UNAIRE)
        return n.u->f;
    return (noeud)NULL;
}

double get_value(noeud n){
    if (get_type(n) == CONSTANTE)
        return n.c->v;

    raler("Error: trying to get the value of an operator", 2);
    return 0;
}

void free_noeud(noeud n){
    type_noeud t = get_type(n);

    switch (t){
        case BINAIRE:
            free_noeud(fils_gauche(n));
            free_noeud(fils_droit(n));
            free(n.b);
            break;

        case UNAIRE:
            free_noeud( fils(n) );
            free(n.u);
            break;

        case CONSTANTE:
            free(n.c);
            break;

        case VARIABLE:
            free(n.v);
            break;

        case ERREUR:
            free(n.e);
            break;

        default:
            raler("Error: trying to free something that's not a node", 3);
            break;
    }
}

void print_noeud_rec(noeud n){
    operation op = get_operation(n);
    type_noeud t = get_type(n);
    switch (t){
        case BINAIRE:
            printw("(%s ", optostr(op));
            print_noeud_rec(fils_gauche(n));
            printw(" ");
            print_noeud_rec(fils_droit(n));
            printw(")");
            break;
        case UNAIRE:
            printw("(%s ", optostr(op));
            print_noeud_rec(fils(n));
            printw(")");
            break;
        case CONSTANTE:
            printw("%.4f", n.c->v);
            break;
        case VARIABLE:
            printw("%c", n.v->id);
            break;
        case ERREUR:
            printw("%s", optostr(op));
            break;
        default:
            break;
    }
}

void print_noeud(noeud n){
    printw("Tree: '");
    print_noeud_rec(n);
    printw("'\n\n");
}

noeud parse_mots_rec(char** mots){
    if (mots[0] == NULL){
        raler("Error: MISSING operand", 4);
        return new_erreur(MANQUANT);
    }

    if (mots[0][0] == '#')
        return parse_mots_rec(&mots[1]);

    operation op = strtoop(mots[0]);
    type_noeud t = optotype(op);
    double valeur;

    noeud n;
    n.c = NULL;
    switch (t){
        case BINAIRE:
            n = new_binaire(op, parse_mots_rec(&mots[1]),
                                parse_mots_rec(&mots[2]));
            break;

        case UNAIRE:
            n = new_unaire(op, parse_mots_rec(&mots[1]));
            break;

        case CONSTANTE:
            valeur = strtod(mots[0], NULL);
            n = new_constante(valeur);
            break;

        case VARIABLE:
            if (mots[0][0] >= 'A' && mots[0][0] <= 'Z' && mots[0][1] == '\0'){
                n = new_variable(mots[0][0]);
            }
            else {
                raler("Error: UNKNOWN operation", 5);
                n = new_erreur(INCONNU);
            }
            break;

        default: //case ERREUR
            raler("Error: UNKNOWN operation", 5);
            n = new_erreur(INCONNU);
            break;
    }
    
    mots[0][0] = '#';
    return n;
}

noeud parse_mots(char** mots){
    noeud n = parse_mots_rec(mots);

    for (int i = 0; mots[i]; ++i){
        if (mots[i][0] != '#')
            raler("Warning: Stray operand left behind", 0);
        free(mots[i]);
    }
    free(mots);
    
    return n;
}

Matrix calcule_noeud(noeud n){
    type_noeud t = get_type(n);
    operation op = get_operation(n);

    if (t == CONSTANTE)
        return newM(1, 1, 1, get_value(n));


    if (t == VARIABLE)
        return getUserMatrix(n.v->id);


        // Opérations unaires
    Matrix gauche = calcule_noeud(fils_gauche(n));
    (void)gauche;
    switch (op){
        case INV: 
            return invertComat(gauche);
        case GAUSS: 
            return invertGauss(gauche);
        case TRANSPOSE: 
            return transpose(gauche);
        case TRIANGULE: 
            return triU(gauche);
        default:
            break;
    }

        // Opérations binaires
    Matrix droit = calcule_noeud(fils_droit(n));
    (void)droit;
    switch (op){
        case ADD: 
            return addition(gauche, droit);
        case MULT: 
            return multiplication(gauche, droit);
        //case SCALE: 
            //return scalar(gauche, droit);
        default: 
            break;
    }
    raler("Error: undefined operation", 7);
    return 0;
}

/**
 * User input handling
 * 
 */

void print_help(){
    printf("matrix <r> <c> <a1, ..., aN> : Create a new matrix\n");
    printf("\n");
}

int readCmd(){
    unsigned long zero = 0;
    char* ligne = NULL;
    char** mots;
    int lu = 0;

    lu = getline(&ligne, &zero, stdin);

    if (lu < 0) // ctrl+d
        exit(0);
    
    ligne[--lu] = '\0';

    if (lu <= 0) // vide
        return 1;

    if (ligne[0] == '#') // commentaire
        return 1;

    // printf("'%s'\n", ligne);
    mots = string_split(ligne, " ");

    if (strcasecmp(mots[0], "matrix") == 0){
        char* word;
        uint rows, columns;

        word = mots[1];
        if (word == NULL){
            printw("How many rows ? ");
            word = scan_next_word();
            printw("\n");
        }
        rows = atoi(word);
        
        word = mots[2];
        if (mots[1] == NULL || word == NULL){
            printw("How many columns ? ");
            word = scan_next_word();
            printw("\n");
        }
        columns = atoi(word);

        Matrix m;
        m = scan_matrix(rows, columns);
        if (m){
            char id = addUserMatrix(m);
            printf("%c = \n", id);
            printMatrix(m);
        }
    }
    else if (strcasecmp(mots[0], "help") == 0){
        print_help();
    } 
    else {
        noeud n = parse_mots(mots);
        //printf("%s\n", ligne);
        if (PARSE_ERROR){
            raler(NULL, 0);
            print_noeud(n);
        }
        else {
            Matrix resultat;
            resultat = calcule_noeud(n);
            printMatrix(resultat);
            print_noeud(n);
        }
        free_noeud(n);
    }

    free(ligne);

    return 1;
}

int main()
{
    clear();
    move(0,0);
    printw("   Matrix Polish Calculator\n");
    printw("Nathan ROTH & Nicolas ARGYRIOU\n");
    printw("\n");
    while (1){
        printw(">");
        if (readCmd() <= 0)
            break;
    }

    return 0;
}
