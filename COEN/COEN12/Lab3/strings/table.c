/*  Tamir Enkhjargal
    COEN 12 Lab 3
    May 5, 2019

    Creating Lab 3, with functionality for hash tables */

/* Calling necessary libraries */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "set.h"
#include <assert.h>
#include <stdbool.h>

/* ---------------------------------------------------------------------- */

/* Defining other necessary variables or function prototypes */

SET *createSet(int maxElts);
void destroySet(SET *sp);
int numElements(SET *sp);
void addElement(SET *sp, char *elt);
void removeElement(SET *sp, char *elt);
char *findElement(SET *sp, char *elt);
char **getElements(SET *sp);
static int search(SET *sp, char *elt, bool *found);
static unsigned strhash(char *s);

#define EMPTY 0
#define DELETED 1
#define FILLED 2

/* ---------------------------------------------------------------------- */

/* Creating the structure for our SET */

struct set {
    char *flag;
    char **data;
    int length;
    int count;
};

/* ---------------------------------------------------------------------- */

/* Create Set Function */

SET *createSet(int maxElts) {
    int i;
    SET *sp = malloc(sizeof(SET));
    assert(sp != NULL);
    sp->length = maxElts;
    sp->count = 0;
    sp->data = malloc(sizeof(char*)*maxElts);
    sp->flag = malloc(sizeof(char)*maxElts);
    assert(sp->data != NULL && sp->flag != NULL);
    for(i = 0; i < sp->length; i++) {
        sp->flag[i] = EMPTY;
    }
    return sp;
}

/* ---------------------------------------------------------------------- */

/* Destroy Set Function */

void destroySet(SET *sp) {
    assert(sp != NULL);
    int i;
    for (i = 0; i < sp->length; i++) {
        if(sp->flag[i] == FILLED) {
            free(sp->data[i]);
        }
    }
    free(sp->data);
    free(sp->flag);
    free(sp);
    return;
}

/* ---------------------------------------------------------------------- */

/* Number of Elements Function */

int numElements(SET *sp) {
    assert(sp != NULL);
    return sp->count;
}

/* ---------------------------------------------------------------------- */

/* Add Element Function */

void addElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    bool found = false;
    int index, i; 
    index = search(sp, elt, &found);
    if(index == -1) {
        return;
    }
    if(found == false) {
        sp->data[index] = strdup(elt);
        assert(sp->data[index] != NULL);
        sp->flag[index] = FILLED;
        sp->count++;
    }
    if(found == true) {
        return;
    }
}

/* ---------------------------------------------------------------------- */

/* Remove Element Function */

void removeElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    bool found = false;
    int index;
    index = search(sp, elt, &found);
    if(found == true) {
        assert(sp->data[index] != NULL);
        sp->flag[index] = DELETED;
        free(sp->data[index]);
        sp->count--;
        return;
    }
}

/* ---------------------------------------------------------------------- */

/* Find Element Function  */

char *findElement(SET *sp, char *elt) {
    assert(sp != NULL && elt != NULL);
    bool found = false;
    search(sp, elt, &found);
    if(found == true) {
        return elt;
    }
    return NULL;
}

/* ---------------------------------------------------------------------- */

/* Get Elements Function */

char** getElements(SET *sp) {
    assert(sp != NULL);
    char **elts;
    elts = malloc(sizeof(char*)*(sp->count));
    int i;
    int j = 0;
    for(i = 0; i < sp->length; i++) {
        if(sp->flag[i] == FILLED) {
            elts[j] = sp->data[i];
            j++;    
        }
    }
    return elts;
}

/* ---------------------------------------------------------------------- */

/* Private Search Function */

static int search(SET *sp, char *elt, bool *found) {
    assert(sp != NULL && elt != NULL);
    unsigned hash;
    int index, i, flag, temp;
    flag = 0;
    *found = false;
    hash = strhash(elt);
    for(i = 0; i < sp->length; i++) {
        index = (hash+i) % (sp->length);
        if(sp->flag[index] == FILLED) {
            if(strcmp(sp->data[index], elt)==0) {
                *found = true;
                return index;
            }
        }
        if(sp->flag[index] == DELETED) {
            if(flag == 0) {
                temp = index;
                flag = 1;
            }
        }
        if(sp->flag[index] == EMPTY) {
            if(flag == 1) {
                return temp;
            }
            return index;
        }   
    }
    return -1;
}

/* ---------------------------------------------------------------------- */

/* Hash Function */

static unsigned strhash(char *s) {
    unsigned hash = 0;
    while (*s != '\0') {
        hash = 31 * hash + *s ++;
    }
    return hash;
}

/* ---------------------------------------------------------------------- */
