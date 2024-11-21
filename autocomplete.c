#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "autocomplete.h"

int cmpr_func(const void *a, const void *b){
    const term *term1 = (const term *)a;
    const term *term2 = (const term *)b;
    return strcmp(term1->term, term2->term);
}

void read_in_terms(term **terms, int *pnterms, char *filename)
{
    // Open the file for reading
    FILE *fptr = fopen(filename, "r");

    // Error checking for file opening
    if (fptr == NULL) {
        exit(1);
    }

    // Initialize number of terms and store it in variable
    fscanf(fptr, "%d", pnterms);

    // Allocate memory for all the terms
    *terms = (term *)malloc((*pnterms) * sizeof(term));

    // Error checking for memory allocation
    if (*terms == NULL) {
        exit(1);
    }
    
    // Read and store terms and weight
    for (int i = 0; i < *pnterms; i++) {
    
        // Search for newline character between term and weight to sort them
        // Read file contents and store them in separate parts 
        // of the 'term' structure
        fscanf(fptr, "%lf %[^\n]", &((*terms)[i].weight), (*terms)[i].term);

    }
    
    // Sort the terms in non-descending lexicographic order (alphabetical order)
    qsort(*terms, *pnterms, sizeof(term), cmpr_func);

    fclose(fptr);
}

int lowest_match(term *terms, int nterms, char *substr){
    int left = 0;
    int right = nterms - 1;
    int index = -1;

    while (left <= right){

        // Need to declare center variables inside
        // while loop to make sure it gets updated each time

        int center = left + (right - left)/2;
        int term_compare = strncmp(terms[center].term, substr, strlen(substr));

        if (term_compare >= 0){
            index = center;
            right = center - 1;

        } 
        // in case that subtsr is larger -> ignore left side from center
        // and check for lowest match in right side
        else {
            left = center + 1;
        }
    }
    return index;
}

int highest_match(struct term *terms, int nterms, char *substr){
    int left = 0;
    int right = nterms - 1;
    int index = -1;

    while (left <= right){

        // Need to declare center variables inside
        // while loop to make sure it gets updated each time

        int center = left + (right - left)/2;
        int term_compare = strncmp(terms[center].term, substr, strlen(substr));

        if (term_compare <= 0){
            index = center;
            left = center + 1;

        }  // subtsr is larger -> ignore left side
        else {
            right = center - 1;
        }
    }

    return index;
}

int cmpr_func2(const void *a, const void *b){
    const term *termA = (const term *)a;
    const term *termB = (const term *)b;
    if ((*termA).weight > (*termB).weight){
        return -1;
    }
    else if (termA->weight < termB->weight){
        return 1;
    }
    else {
        return 0;
    }
}

void autocomplete(struct term **answer, int *n_answer, struct term *terms, int nterms, char *substr){

    // Initialize variables
    int highest_match_num = highest_match(terms, nterms, substr);
    int lowest_match_num = lowest_match(terms, nterms, substr);

    // Initialize n_answer
    *n_answer = (highest_match_num - lowest_match_num) + 1;

    // Obtain appropriate terms/answers to substrings
    // Allocate space in memory for answers
    (*answer) = (term *)malloc((*n_answer) * sizeof(term));

    // Error checking for memory allocation
    if (*answer == NULL) {
        exit(1);
    }

    for (int i = 0; i < *n_answer; i++){
        // Place the answers in 'term' struct
        strcpy((*answer)[i].term, terms[lowest_match_num + i].term);
        // Update the weight of the terms
        (*answer)[i].weight = terms[lowest_match_num + i].weight;
    }

    // Sort the answers in non-increasing order by their weight
    // Non-increasing / non-ascending is diff because it includes
    // the possibility of adjacent terms being equal 

    // Note to self: Remember, qsort by default sorts in increasing order
    // so you need to reverse the 1 and -1 in the cmpr_func2
    qsort(*answer, *n_answer, sizeof(term), cmpr_func2);
}