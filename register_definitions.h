#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#ifndef REGISTER_DEFINITIONS_H
#define REGISTER_DEFINITIONS_H



enum BOOL{
    FALSE, TRUE
};

//Method Type
#define BREATH 0
#define DEAPTH 1
#define A_STAR 2
#define BEST   3

//Timeout time
#define TIMEOUT		60

//Operations
#define INCREASE    0
#define DECREASE    1
#define DOUBLE_OP   2
#define HALF        3
#define SQUARE      4
#define ROOT_OP     5

//Tree Node
typedef struct tree_node {
    unsigned long int number;
    unsigned long int h;
    unsigned long int f;
    unsigned long int g;
    struct tree_node *parent;
    int last_operation;
} tree_node;

//Frontier Node
typedef struct frontier_node{
    tree_node *leaf;
    struct frontier_node *next;
    struct frontier_node *prev;
} frontier_node;


//Take the type of algo and check the validity
int get_method(char temp_method[]){

    // Checks and assigns method based on input string
    int temp_method_final = -1;
    if(strcmp(temp_method,"breath") == 0){
        temp_method_final = BREATH;
    }
    else if(strcmp(temp_method,"deapth") == 0){
        temp_method_final = DEAPTH;
    }
    else if(strcmp(temp_method,"a_star") == 0){
        temp_method_final = A_STAR;
    }
    else if(strcmp(temp_method,"best") == 0){
        temp_method_final = BEST;
    }

    // If no valid method was found, inform user and return -1
    if (temp_method_final == -1)
    {
        printf("Invalid type of algo, try use DEAPTH, BREATH, A_STAR Or BEST\n");
    }

    return temp_method_final;
}

//take the start and target number and check the validity
int valid_input(double start_temp, double target_temp) {
    // Checks if input values are valid
    if(start_temp < 0 || target_temp < 0 || start_temp == target_temp || fmod(start_temp, 1) != 0 || fmod(target_temp, 1) != 0) {
        printf("Invalid input, try something else\n");
        return 0;
    }
    return 1;
}

// Calculate heuristic cost
unsigned long int heuristic(unsigned long int current_produced_number, int target) {
    double temp = abs(target - current_produced_number) / 4.0;
    unsigned long int result = ceil(temp);

    return result;
}

// Function to check if the current child has the same number as its parent(s)
int check_same_number (tree_node *child) {
    tree_node *temp = child;

    // Loop until we reach the root of the tree
    while (temp->parent != NULL) {
        // If the current child's number is equal to its parent's number, return 1 (true)
        if (child->number == temp->parent->number) {
            return 1;
        }
        temp = temp->parent; // Move up to the parent node
    }

    // If no parent node had the same number, return 0 (false)
    return 0;
}

#endif