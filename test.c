/*
* Name : Chrysostomos Panagiotis Penloglou
* AEM : ICS22116
* ΕΤΟΣ : 3o
* Lesson : AIMA
* Teacher : Doc. Ioannis Refanidis
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define breath 0
#define deapth 1
#define a_star 2
#define best   3


enum bool{
    FALSE, TRUE
};

#define increase    0
#define decrease    1
#define double_op   2
#define half        3
#define square      4
#define root_op     5

typedef struct tree_node {
    int number;
    int h;
    int f;
    int g;
    struct tree_node *parent;
    int last_operation;
} tree_node;

typedef struct frontier_node{
    tree_node *leaf;
    struct frontier_node *next;
    struct frontier_node *prev;
} frontier_node;


frontier_node *frontier_head;
frontier_node *frontier_tail;
tree_node* solution;

int start, target;
int method;
clock_t t1;				// Start time of the search algorithm
clock_t t2;				// End time of the search algorithm
#define TIMEOUT		60


//take the type of algo and check the validity
int get_method(char temp_method[]){

    int temp_method_final = -1;

    if(strcmp(temp_method,"breath") == 0){
        temp_method_final = breath;
    }
    else if(strcmp(temp_method,"deapth") == 0){
        temp_method_final = deapth;
    }
    else if(strcmp(temp_method,"a_star") == 0){
        temp_method_final = a_star;
    }
    else if(strcmp(temp_method,"best") == 0){
        temp_method_final = best;
    }
    if (temp_method_final == -1)
    {
        printf("Invalid type of algo, try use deapth, breath, a_star Or best\n");
    }

    return temp_method_final;
}

//calculation of g
int calc_cost(int parent_num, int type) {

    int cost = -1;

    switch (type)
    {
        case -1:
            {cost = 0;
            break;}

        case increase:
            {if(parent_num < pow(10, 9))
                cost = 2;
            break;}

        case decrease:
            {if(parent_num > 0)
                cost = 2;
            break;}

        case double_op:
            {if(parent_num > 0.2 * parent_num && 0.2 * parent_num <= pow(10,9))
                cost = ceil(parent_num / 2 + 1);
            break;}

        case half:
            {if(parent_num > 0)
                cost = ceil(parent_num / 4 + 1);
            break;}

        case square:
            {if(pow(parent_num, 2) < pow(10,9))
                cost = (parent_num * parent_num - parent_num)/4 + 1;
            break;}
            
        case root_op:
            {if(parent_num > 1 && fmod(sqrt(parent_num), 1) == 0)
                cost = (parent_num - sqrt(parent_num))/4;
            break;}

        default:
            {printf("The type of act is invalid, try to use increase, decrease or double_op\n");
            cost = -1;
            break;}
    }

    return cost;
}

int heuristic(int a, int b) {
    double temp = abs(target - b) / 4.0;
    int result = ceil(temp);

    return result;
}

//calculation of h
int calcH(int parent_num, int type) {

    int dist_h = -1;
    int value  =  0;

    switch (type)
    {
        case -1:
            {dist_h = abs(target - parent_num);
            break;}

        case increase:
            {if(parent_num < pow(10, 9))
                dist_h = heuristic(parent_num, parent_num + 1);
            break;}

        case decrease:
            {if(parent_num > 0)
                dist_h = heuristic(parent_num, parent_num - 1);
            break;}

        case double_op:
            {if(parent_num > 0.2 * parent_num && 0.2 * parent_num <= pow(10,9))
               { value = parent_num * 2;
                dist_h = heuristic(parent_num, value);}
            break;}

        case half:
            {if(parent_num > 0)
               { value  = parent_num / 2;
                dist_h = heuristic(parent_num, value);}
            break;}

        case square:
            {if(pow(parent_num, 2) < pow(10,9))
                {value  = parent_num * parent_num;
                dist_h = heuristic(parent_num, value);}
            break;}
            
        case root_op:
            {if(parent_num > 1 && fmod(sqrt(parent_num), 1) == 0)
                {value  = sqrt(parent_num);
                dist_h = heuristic(parent_num, value);}
            break;}

        default:
            {printf("The type of act is invalid, try to use increase, decrease or double_op\n");
            dist_h = -1;
            break;}
    }

    return dist_h;
}

//take the start and target number and check the validity
int valid_input(double start_temp, double target_temp) {
    if(start_temp < 0 || target_temp < 0 || start_temp == target_temp || fmod(start_temp, 1) != 0 || fmod(target_temp, 1) != 0) {
        printf("Invalid input, try something else\n");
        return 0;
    }
    return 1;
}

int check_same_number (tree_node *child) {
    tree_node *temp = child;
    while (temp->parent != NULL) {
        if (child->number == temp->parent->number) {
            return 1;
        }
        temp = temp->parent;
    }
    return 0;
}



int add_frontier_front(tree_node *node)
{
	// Creating the new frontier node
	frontier_node *new_frontier_node = (frontier_node*)malloc(sizeof(frontier_node));
	if (new_frontier_node==NULL)
		return -1;

	new_frontier_node->leaf = node;
	new_frontier_node->prev = NULL;
	new_frontier_node->next = frontier_head;

	if (frontier_head==NULL)
	{
		frontier_head=new_frontier_node;
		frontier_tail=new_frontier_node;
	}
	else
	{
		frontier_head->prev=new_frontier_node;
		frontier_head=new_frontier_node;
	}

	return 0;
}

// This function adds a pointer to a new leaf search-tree node at the back of the frontier.
// This function is called by the breadth-first search algorithm.
// Inputs:
//		struct tree_node *node	: A (leaf) search-tree node.
// Output:
//		0 --> The new frontier node has been added successfully.
//		-1 --> Memory problem when inserting the new frontier node .
int add_frontier_back(tree_node *node)
{
	// Creating the new frontier node
	frontier_node *new_frontier_node = (frontier_node*) malloc(sizeof(frontier_node));
	if (new_frontier_node==NULL)
		return -1;

	new_frontier_node->leaf=node;
	new_frontier_node->next=NULL;
	new_frontier_node->prev=frontier_tail;

	if (frontier_tail==NULL)
	{
		frontier_head=new_frontier_node;
		frontier_tail=new_frontier_node;
	}
	else
	{
		frontier_tail->next=new_frontier_node;
		frontier_tail=new_frontier_node;
	}

	return 0;
}

// This function adds a pointer to a new leaf search-tree node within the frontier.
// The frontier is always kept in increasing order wrt the f values of the corresponding
// search-tree nodes. The new frontier node is inserted in order.
// This function is called by the heuristic search algorithm.
// Inputs:
//		struct tree_node *node	: A (leaf) search-tree node.
// Output:
//		0 --> The new frontier node has been added successfully.
//		-1 --> Memory problem when inserting the new frontier node .
int add_frontier_in_order(tree_node *node)
{
	// Creating the new frontier node
	frontier_node *new_frontier_node = (frontier_node*)malloc(sizeof(frontier_node));
	if (new_frontier_node==NULL)
		return -1;

	new_frontier_node->leaf=node;
	new_frontier_node->prev=NULL;
	new_frontier_node->next=NULL;

	if (frontier_head==NULL)
	{
		frontier_head=new_frontier_node;
		frontier_tail=new_frontier_node;
	}
	else
	{
		frontier_node *pt;
		pt = frontier_head;

		// Search in the frontier for the first node that corresponds to either a larger f value
		// or to an equal f value but larger h value
		// Note that for the best first search algorithm, f and h values coincide.
		while (pt!=NULL && (pt->leaf->f < node->f || (pt->leaf->f == node->f && pt->leaf->h < node->h)))
			pt = pt->next;

		if (pt!=NULL)
		{
			// new_frontier_node is inserted before pt .
			if (pt->prev!=NULL)
			{
				pt->prev->next=new_frontier_node;
				new_frontier_node->next=pt;
				new_frontier_node->prev=pt->prev;
				pt->prev=new_frontier_node;
			}
			else
			{
				// In this case, new_frontier_node becomes the first node of the frontier.
				new_frontier_node->next=pt;
				pt->prev=new_frontier_node;
				frontier_head=new_frontier_node;
			}
		}
		else
		{
			// if pt==NULL, new_frontier_node is inserted at the back of the frontier
			frontier_tail->next=new_frontier_node;
			new_frontier_node->prev=frontier_tail;
			frontier_tail=new_frontier_node;
		}
	}

	return 0;
}


int create_children (tree_node* temp) {

    //increase
    int flag_increase = TRUE;
    tree_node *child_increase = (tree_node *)malloc(sizeof(tree_node));
    child_increase->number = temp->number + 1;
    child_increase->parent = temp;
    child_increase->g = calc_cost(temp->number, increase) + temp->g;
    child_increase->h = calcH(temp->number, increase);
    child_increase->f = child_increase->g + child_increase->h;
    child_increase->last_operation = increase;
    if(check_same_number(child_increase) == 1 || calc_cost(temp->number, increase) == -1) {free(child_increase); flag_increase = FALSE;}

    //decrease
    int flag_decrease = TRUE;
    tree_node *child_decrease = (tree_node *)malloc(sizeof(tree_node));
    child_decrease->number = temp->number - 1;
    child_decrease->parent = temp;
    child_decrease->g = calc_cost(temp->number, decrease) + temp->g;
    child_decrease->h = calcH(temp->number, decrease);
    child_decrease->f = child_decrease->g + child_decrease->h;
    child_decrease->last_operation = decrease;
    if(check_same_number(child_decrease) == 1 || calc_cost(temp->number, decrease) == -1) {free(child_decrease); flag_decrease = FALSE;}

    //double_op
    int flag_double_op = TRUE;
    tree_node *child_double_op = (tree_node *)malloc(sizeof(tree_node));
    child_double_op->number = temp->number * 2;
    child_double_op->parent = temp;
    child_double_op->g = calc_cost(temp->number, double_op) + temp->g;
    child_double_op->h = calcH(temp->number, double_op);
    child_double_op->f = child_double_op->g + child_double_op->h;
    child_double_op->last_operation = double_op;
    if(check_same_number(child_double_op) == 1 || calc_cost(temp->number, double_op) == -1) {free(child_double_op); flag_double_op = FALSE;}

    //half
    int flag_half = TRUE;
    tree_node *child_half = (tree_node *)malloc(sizeof(tree_node));
    child_half->number = temp->number / 2;
    child_half->parent = temp;
    child_half->g = calc_cost(temp->number, half) + temp->g;
    child_half->h = calcH(temp->number, half);
    child_half->f = child_half->g + child_half->h;
    child_half->last_operation = half;
    if(check_same_number(child_half) == 1 || calc_cost(temp->number, half) == -1) {free(child_half); flag_half = FALSE;}

    //square
    int flag_square = TRUE;
    tree_node *child_square = (tree_node *)malloc(sizeof(tree_node));
    child_square->number = (temp->number * temp->number);
    child_square->parent = temp;
    child_square->g = calc_cost(temp->number, square) + temp->g;
    child_square->h = calcH(temp->number, square);
    child_square->f = child_square->g + child_square->h;
    child_square->last_operation = square;
    if(check_same_number(child_square) == 1 || calc_cost(temp->number, square) == -1) {free(child_square); flag_square = FALSE;}

    //root_op
    int flag_root_op = TRUE;
    tree_node *child_root_op = (tree_node *)malloc(sizeof(tree_node));
    child_root_op->number = sqrt(temp->number);
    child_root_op->parent = temp;
    child_root_op->g = calc_cost(temp->number, root_op) + temp->g;
    child_root_op->h = calcH(temp->number, root_op);
    child_root_op->f = child_root_op->g + child_root_op->h;
    child_root_op->last_operation = root_op;
    if(check_same_number(child_root_op) == 1 || calc_cost(temp->number, root_op) == -1) {free(child_root_op); flag_root_op = FALSE;}

    switch (method)
    {

        case breath: {
            if(flag_increase == TRUE)
                add_frontier_back(child_increase);
            if(flag_decrease == TRUE)
                add_frontier_back(child_decrease);
            if(flag_double_op == TRUE)
                add_frontier_back(child_double_op);
            if(flag_half == TRUE)
                add_frontier_back(child_half);
            if(flag_square == TRUE)
                add_frontier_back(child_square);
            if(flag_root_op == TRUE)
                add_frontier_back(child_root_op);
            break;
        }

        case deapth: {
            if(flag_increase == TRUE)
                add_frontier_front(child_increase);
            if(flag_decrease == TRUE)
                add_frontier_front(child_decrease);
            if(flag_double_op == TRUE)
                add_frontier_front(child_double_op);
            if(flag_half == TRUE)
                add_frontier_front(child_half);
            if(flag_square == TRUE)
                add_frontier_front(child_square);
            if(flag_root_op == TRUE)
                add_frontier_front(child_root_op);
            break;
        }
        case a_star:{
            if(flag_increase == TRUE)
                add_frontier_in_order(child_increase);
            if(flag_decrease == TRUE)
                add_frontier_in_order(child_decrease);
            if(flag_double_op == TRUE)
                add_frontier_in_order(child_double_op);
            if(flag_half == TRUE)
                add_frontier_in_order(child_half);
            if(flag_square == TRUE)
                add_frontier_in_order(child_square);
            if(flag_root_op == TRUE)
                add_frontier_in_order(child_root_op);
            break;
        }
            break;

        case best:{
            if(flag_increase == TRUE)
                add_frontier_in_order(child_increase);
            if(flag_decrease == TRUE)
                add_frontier_in_order(child_decrease);
            if(flag_double_op == TRUE)
                add_frontier_in_order(child_double_op);
            if(flag_half == TRUE)
                add_frontier_in_order(child_half);
            if(flag_square == TRUE)
                add_frontier_in_order(child_square);
            if(flag_root_op == TRUE)
                add_frontier_in_order(child_root_op);
            break;
        }
            break;

        default:
            {printf("The type of act is invalid, try to use breath, deapth, best, a_star\n");
            break;}
    }

    return TRUE;
}

int is_solution(int number) {
    return number == target;
}

tree_node * search_tree () {
    int err;

    clock_t t;
	
	frontier_node *temp_frontier_node;

	tree_node *current_node;
    tree_node *temp;

    while(frontier_head != NULL) {

        t=clock();

		if (t-t1 > CLOCKS_PER_SEC*TIMEOUT)
		{
			printf("Timeout\n");
			return NULL;
		}

		// Extract the first node from the frontier
		current_node = frontier_head->leaf;

		if (is_solution(current_node->number))
			return current_node;

		// Delete the first node of the frontier
		temp_frontier_node=frontier_head;
		frontier_head = frontier_head->next;
		free(temp_frontier_node);
		if (frontier_head==NULL)
			frontier_tail=NULL;
		else
			frontier_head->prev=NULL;
        //frontier_node *tempFrontierNode;
		
        switch (method)
            {

                case breath: {
                    temp = current_node;
                    err = create_children(temp);
                    break;
                }

                case deapth: {
                    temp = current_node;
                    err = create_children(temp);
                    break;
                }
                case a_star:{
                    temp = current_node;
                    err = create_children(temp);

                }
                    break;

                case best:{
                    temp = current_node;
                    err = create_children(temp);
                }
                    break;

                default:
                    printf("The type of act is invalid, try to use breath, deapth, best, a_star\n");
                    break;
            }

            if (err<0)
	        {
            		printf("Memory exhausted while creating new frontier node. Search is terminated...\n");
			return NULL;
        	}
    }

    return temp;
}

void initialize_tree() {
    int first = -1;

    tree_node *root = (tree_node *)malloc(sizeof(tree_node));
    root->number = start;
    root->h = calcH(root->number, first);
    root->g = calc_cost(root->number, first);
    root->parent = NULL;
    if (method==best)
		root->f=root->h;
	else if (method==a_star)
		root->f=root->g+root->h;
	else
		root->f=0;

    add_frontier_front(root);
}

void write_solution_to_file(char* filename, int solution_length)
{
	FILE *fp;
	fp=fopen(filename,"w");
	if (fp==NULL)
	{
		printf("Cannot open output file to write solution.\n");
		printf("Now exiting...");
		return;
	}
    fprintf(fp, "\n***** SOLUTION *****\n");
    int finalSteps = 0;
    for(int steps = 0; solution->parent != NULL; steps++) {
        switch (solution->last_operation)
        {
        case increase:
            {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->g ,"increase", solution->number);
            break;}
        case decrease:
            {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"decrease", solution->number);
            break;}
        case double_op:
            {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"double_op", solution->number);
            break;}
        case half:
            {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"half", solution->number);
            break;}
        case square:
           {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"square", solution->number);
            break;}
        case root_op:
            {fprintf(fp, "\nstep: %d cost:%d %s = %d\n", solution->parent->number, solution->h,"root_op", solution->number);
            break;}
        
        default:
            break;
        }
        solution = solution->parent;
        finalSteps = steps;
    }
    fprintf(fp, "\n***** FROM START %d IN %d STEPS *****\n", start, finalSteps + 1);
    fclose(fp);
}

int main(int argc, char *argv[])
{
    printf("%lf\n", pow(10, 2));
    if(get_method(argv[1]) == - 1) return 0; else method = get_method(argv[1]);
    if(valid_input(atof(argv[2]), atof(argv[3])) == 0) return 0; else {start = atoi(argv[2]);target = atoi(argv[3]);}
    t1=clock();
    initialize_tree();
    solution = search_tree();
    t2=clock();
    if (solution==NULL)
		printf("No solution found.\n");

	if (solution!=NULL)
	{
		printf("Solution found! (%d steps)\n",10);
		printf("Time spent: %f secs\n",((float) t2-t1)/CLOCKS_PER_SEC);
		write_solution_to_file(argv[4], 10);
	}
    printf("Enter the type of algo: %s is coded: %d\n", argv[1], method);
    printf("Enter the start number: %d\n", start);
    printf("Enter the target number: %d\n", target);

    return 0;
}
