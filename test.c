#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define breath 0
#define deapth 1
#define a_star 2
#define best   3

#define add 0
#define sub 1
#define mul 2

typedef struct tree_node {
    int number;
    int h;
    int g;
    struct tree_node *parent;
    //struct tree_node *children[4];
} tree_node;

typedef struct frontier_node{
    tree_node *leaf;
    struct frontier_node *next;
    struct frontier_node *prev;
} frontier_node;

tree_node *root;
frontier_node *frontier_head;
frontier_node *frontier_tail;

int start, target;


//take the type of algo and check the validity
int get_method(char method[]){

    int method_final = -1;
    
    if(strcmp(method,"breath") == 0){
        method_final = breath;
    }
    else if(strcmp(method,"deapth") == 0){
        method_final = deapth;
    }
    else if(strcmp(method,"a_star") == 0){
        method_final = a_star;
    }
    else if(strcmp(method,"best") == 0){
        method_final = best;
    }
    if (method_final == -1)
    {
        printf("Invalid type of algo, try use deapth, breath, a_star Or best\n");
    }
    
    return method_final;
}

//calculation of g
int calc_cost(int parent_num, int type) {

    int cost;

    switch (type)
    {
    case -1:
        cost = 0;
    case add:
    case sub:
        cost = 1;
        break;
    case mul:
        cost = parent_num;
        break;
    
    default:
        printf("The type of act is invalid, try to use add, sub or mul\n");
        cost = -1;
        break;
    }

    return cost;
}

//calculation of h
int calc_dist(int parent_num, int type) {

    int dist_h;

    switch (type)
    {
    case -1:
        dist_h = abs(target - parent_num);
    case add:
        dist_h = abs(target - parent_num + 1);
        break;
    case sub:
        dist_h = abs(target - parent_num - 1);
        break;
    case mul:
        dist_h = abs(target - parent_num * 2);
        break;
    
    default:
        printf("The type of act is invalid, try to use add, sub or mul\n");
        dist_h = -1;
        break;
    }
    return dist_h;
}

//take the start and target number and check the validity
int valid_input() {
    if(start < 0 || target < 0 || start == target) {
        printf("Invalid input, try something else\n");
        return 0;
    }
    return 1;
}

int check_same_number (tree_node *child) {
    tree_node *temp = child;
    while (temp!= NULL) {
        if (temp->number == temp->parent->number) {
            return 1;
        }
        temp = temp->parent;
    }
    return 0;
}

tree_node * search_tree (int type) {
    tree_node *temp;
    switch (type)
    {
    case 0:
    {
        temp = frontier_head->next->leaf;
        break;
    }
    case 1:
    {
        temp = frontier_head->next->leaf;
        break;
    }
    case 2:
    {
        
        break;
    }
    case 3:
    {
        
        break;
    }
    
    default:
        break;
    }

    return temp;
}


void create_children (int type) {
    tree_node *temp = search_tree(type);
    while (temp != NULL) {
        
        //Add
        tree_node *child_add = (tree_node *)malloc(sizeof(tree_node));
        child_add->number = child_add->number + 1;
        child_add->parent = temp;
        child_add->g = calc_cost(temp->number, add) + temp->g;
        child_add->h = calc_dist(temp->number, add);
        if(check_same_number(child_add) == 1) free(child_add);

        //Sub
        tree_node *child_sub = (tree_node *)malloc(sizeof(tree_node));
        child_sub->number = child_sub->number - 1;
        child_sub->parent = temp;
        child_sub->g = calc_cost(temp->number, sub) + temp->g;
        child_sub->h = calc_dist(temp->number, sub);
        if(check_same_number(child_sub) == 1) free(child_sub);

        //Mul
        tree_node *child_mul = (tree_node *)malloc(sizeof(tree_node));
        child_mul->number = child_mul->number * 2;
        child_mul->parent = temp;
        child_mul->g = calc_cost(temp->number, mul) + temp->g;
        child_mul->h = calc_dist(temp->number, mul);
        if(check_same_number(child_mul) == 1) free(child_mul);

    }
}

void initialize_tree() {
    int first = -1;

    root = (tree_node *)malloc(sizeof(tree_node));
    root->number = start;
    root->h = calc_dist(root->number, first);
    root->g = calc_cost(root->number, first);
    root->parent = NULL;

    // root->children[0] = NULL;
    // root->children[1] = NULL;
    // root->children[2] = NULL;
    // root->children[3] = NULL;

    frontier_node *frontier_root = (frontier_node *)malloc(sizeof(frontier_node));

    frontier_root->leaf = root;
    frontier_root->next = NULL;
    frontier_root->prev = NULL;

    frontier_head->leaf = NULL;
    frontier_head->next = frontier_root;
    frontier_head->prev = frontier_root;

    frontier_tail->leaf = NULL;
    frontier_tail->next = frontier_root;
    frontier_tail->prev = frontier_root;
    
}

int main(int argc, char *argv[])
{
    
    int method;

    if(get_method(argv[1]) == - 1) return 0; else method = get_method(argv[1]);
    if(valid_input(atoi(argv[2]), atoi(argv[3])) == 0) return 0; else {start = atoi(argv[2]);target = atoi(argv[3]);}

    printf("Enter the type of algo: %s is coded: %d\n", argv[1], method);
    printf("Enter the start number: %d\n", start);
    printf("Enter the target number: %d\n", target);
    
    return 0;
}
