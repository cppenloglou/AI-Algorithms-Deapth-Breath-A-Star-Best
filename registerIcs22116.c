/*
* Name : Chrysostomos Panagiotis Penloglou
* AEM : ICS22116
* ΕΤΟΣ : 3o
* Lesson : AIMA
* Teacher : Doc. Ioannis Refanidis
*/

#include "register_calculations.h"

frontier_node *frontier_head;
frontier_node *frontier_tail;
tree_node* solution;

int start, target;
int method;

clock_t t1;	// Start time of the search algorithm
clock_t t2;	// End time of the search algorithm

int get_method(char temp_method[]);

int heuristic(int a, int target);

int check_same_number (tree_node *child);

int valid_input(double start_temp, double target_temp);

//Calculation of g
int claculateCost(int parent_num, int type);

//Calculate number value of node
int claculateNumber(int parent_num, int type);

//Calculation of h
int calculateH_Value(int parent_num, int type, int target);

/*
This code adds a new frontier node to the front of 
the frontier list. The frontier list represents the
 set of tree nodes to be explored during a depth-first search (DFS)
*/
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

/*This function creates a new frontier node and adds
 it to the end of the frontier. If the frontier is empty,
 it sets the new node as both the head and tail of the frontier.
 Otherwise, it updates the next pointer of the current tail node
 and sets the new node as the new tail of the frontier.
 The function returns 0 if successful and -1 if memory allocation fails.
*/

int add_frontier_back(tree_node *node)
{
    // Creating new frontier node
    frontier_node *new_frontier_node = (frontier_node*) malloc(sizeof(frontier_node));
    if (new_frontier_node==NULL)
        return -1;

    // Setting node data and next/prev pointers
    new_frontier_node->leaf=node;
    new_frontier_node->next=NULL;
    new_frontier_node->prev=frontier_tail;

    // Adding node to the end of the frontier
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

/*
It searches for the position where the new node should be inserted,
considering both f and h values. The new node is then inserted at
the appropriate position, taking care of updating the next, prev,
and head/tail pointers.
*/
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
		// Note that for the BEST first search algorithm, f and h values coincide.
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

tree_node* createOnceChildren (tree_node* temp, int operation){
    tree_node *tempChild = (tree_node *)malloc(sizeof(tree_node));
    tempChild->number = claculateNumber(temp->number, operation);
    tempChild->parent = temp;
    tempChild->g = claculateCost(temp->number, operation) + temp->g;
    tempChild->h = calculateH_Value(temp->number, operation, target);
    if(method == BEST)
        tempChild->f = tempChild->h;
    if(method == A_STAR)
        tempChild->f = tempChild->g + tempChild->h;  
    tempChild->last_operation = operation;
    return tempChild;
}


int create_children (tree_node* temp) {

    // for(int operation = 0; operation < 6; operation++) {
    //     //Operation
    //     int flagOperation = TRUE;
    //     tree_node *childOperation = createOnceChildren(temp, operation);
    //     if(check_same_number(childOperation) == 1 || claculateCost(temp->number, operation) == -1) {free(childOperation); flagOperation = FALSE;}
    //     switch (method)
    //     {

    //         case BREATH: {
    //             if(flagOperation)add_frontier_back(childOperation);
    //             break;
    //         }

    //         case DEAPTH: {
    //             if(flagOperation)add_frontier_front(childOperation);
    //             break;
    //         }
    //         case A_STAR:
    //         case BEST:{
    //             if(flagOperation)add_frontier_in_order(childOperation);
    //             break;
    //         }

    //         default:
    //             {printf("The type of act is invalid, try to use BREATH, DEAPTH, BEST, A_STAR\n");
    //             break;}
    //     }
    // }

    //INCREASE
    int flag_INCREASE = TRUE;
    tree_node *child_INCREASE = createOnceChildren(temp, INCREASE);
    if(check_same_number(child_INCREASE) == 1 || claculateCost(temp->number, INCREASE) == -1) {free(child_INCREASE); flag_INCREASE = FALSE;}

    //DECREASE
    int flag_DECREASE = TRUE;
    tree_node *child_DECREASE = createOnceChildren(temp, DECREASE);
    if(check_same_number(child_DECREASE) == 1 || claculateCost(temp->number, DECREASE) == -1) {free(child_DECREASE); flag_DECREASE = FALSE;}

    //DOUBLE_OP
    int flag_DOUBLE_OP = TRUE;
    tree_node *child_DOUBLE_OP = createOnceChildren(temp, DOUBLE_OP);
    if(check_same_number(child_DOUBLE_OP) == 1 || claculateCost(temp->number, DOUBLE_OP) == -1) {free(child_DOUBLE_OP); flag_DOUBLE_OP = FALSE;}

    //HALF
    int flag_HALF = TRUE;
    tree_node *child_HALF = createOnceChildren(temp, HALF);
    if(check_same_number(child_HALF) == 1 || claculateCost(temp->number, HALF) == -1) {free(child_HALF); flag_HALF = FALSE;}

    //SQUARE
    int flag_SQUARE = TRUE;
    tree_node *child_SQUARE = createOnceChildren(temp, SQUARE);
    if(check_same_number(child_SQUARE) == 1 || claculateCost(temp->number, SQUARE) == -1) {free(child_SQUARE); flag_SQUARE = FALSE;}

    //ROOT_OP
    int flag_ROOT_OP = TRUE;
    tree_node *child_ROOT_OP = createOnceChildren(temp, ROOT_OP);
    if(check_same_number(child_ROOT_OP) == 1 || claculateCost(temp->number, ROOT_OP) == -1) {free(child_ROOT_OP); flag_ROOT_OP = FALSE;}

    switch (method)
    {

        case BREATH: {
            if(flag_INCREASE)add_frontier_back(child_INCREASE);
            if(flag_DECREASE)add_frontier_back(child_DECREASE);
            if(flag_DOUBLE_OP)add_frontier_back(child_DOUBLE_OP);
            if(flag_HALF)add_frontier_back(child_HALF);
            if(flag_SQUARE)add_frontier_back(child_SQUARE);
            if(flag_ROOT_OP)add_frontier_back(child_ROOT_OP);
            break;
        }

        case DEAPTH: {
            if(flag_INCREASE)add_frontier_front(child_INCREASE);
            if(flag_DECREASE)add_frontier_front(child_DECREASE);
            if(flag_DOUBLE_OP)add_frontier_front(child_DOUBLE_OP);
            if(flag_HALF)add_frontier_front(child_HALF);
            if(flag_SQUARE)add_frontier_front(child_SQUARE);
            if(flag_ROOT_OP)add_frontier_front(child_ROOT_OP);
            break;
        }
        case A_STAR:
        case BEST:{
            if(flag_INCREASE)add_frontier_in_order(child_INCREASE);
            if(flag_DECREASE)add_frontier_in_order(child_DECREASE);
            if(flag_DOUBLE_OP)add_frontier_in_order(child_DOUBLE_OP);
            if(flag_HALF)add_frontier_in_order(child_HALF);
            if(flag_SQUARE)add_frontier_in_order(child_SQUARE);
            if(flag_ROOT_OP)add_frontier_in_order(child_ROOT_OP);
            break;
        }

        default:
            {printf("The type of act is invalid, try to use BREATH, DEAPTH, BEST, A_STAR\n");
            break;}
    }

    return TRUE;
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

        //Is solution?
		if (current_node->number == target)
			return current_node;

		// Delete the first node of the frontier
		temp_frontier_node=frontier_head;
		frontier_head = frontier_head->next;
		free(temp_frontier_node);
		if (frontier_head==NULL)
			frontier_tail=NULL;
		else
			frontier_head->prev=NULL;
		
        switch (method)
            {

                case BREATH: {
                    temp = current_node;
                    err = create_children(temp);
                    break;
                }

                case DEAPTH: {
                    temp = current_node;
                    err = create_children(temp);
                    break;
                }
                case A_STAR:{
                    temp = current_node;
                    err = create_children(temp);

                }
                    break;

                case BEST:{
                    temp = current_node;
                    err = create_children(temp);
                }
                    break;

                default:
                    printf("The type of act is invalid, try to use BREATH, DEAPTH, BEST, A_STAR\n");
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
    int first = -1; // initialize the first value

    tree_node *root = (tree_node *)malloc(sizeof(tree_node)); // allocate memory for the root node
    root->number = start; // set the number value of the root node
    root->h = calculateH_Value(root->number, first, target); // calculate the heuristic value of the root node
    root->g = claculateCost(root->number, first); // calculate the cost of reaching the root node from the start
    root->parent = NULL; // set the parent of the root node to NULL
    if (method == BEST) // if the search method is BEST
        root->f = root->h; // set the f value to the heuristic value
    else if (method == A_STAR) // if the search method is A_STAR
        root->f = root->g + root->h; // set the f value to the sum of the cost and heuristic values
    else // if the search method is not supported
        root->f = 0; // set the f value to 0

    add_frontier_front(root); // add the root node to the frontier
}

// Write the solution sequence of nodes into the file solution.txt
void write_solution_to_file(char* filename, int solution_length)
{
    // Open file for writing
    FILE *fp;
    fp=fopen(filename,"w");
    if (fp==NULL)
    {
        printf("Cannot open output file to write solution.\n");
        printf("Now exiting...");
        return;
    }

    // Write solution header to file
    fprintf(fp, "\n***** SOLUTION *****\n");

    // Calculate and print the solution steps
    int finalSteps = 0;
    for(int steps = 0; solution->parent != NULL; steps++) {
        fprintf(fp, "\nstep: %d ", steps);
        switch (solution->last_operation)
        {
        case INCREASE:
            {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g ,"INCREASE", solution->number);
            break;}
        case DECREASE:
            {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"DECREASE", solution->number);
            break;}
        case DOUBLE_OP:
            {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"DOUBLE_OP", solution->number);
            break;}
        case HALF:
            {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"HALF", solution->number);
            break;}
        case SQUARE:
           {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"SQUARE", solution->number);
            break;}
        case ROOT_OP:
            {fprintf(fp, "\ndeapth: %d cost:%d %s = %d\n", solution->parent->number, solution->g,"ROOT_OP", solution->number);
            break;}
        
        default:
            break;
        }
        solution = solution->parent;
        finalSteps = steps;
    }

    // Write final solution details to file
    fprintf(fp, "\n***** FROM START %d IN %d STEPS *****\n", start, finalSteps + 1);

    // Close the file
    fclose(fp);
}

int main(int argc, char *argv[])
{
    // Check if command line arguments are valid
    if(argc == 5) {
        printf("%lf\n", pow(10, 2));
        
        // Set the algorithm method
        if(get_method(argv[1]) == - 1) return 0; else method = get_method(argv[1]);
        
        // Set the start and target numbers
        if(valid_input(atof(argv[2]), atof(argv[3])) == 0) return 0; else {start = atoi(argv[2]);target = atoi(argv[3]);}
        
        // Record the start time
        t1=clock();
        
        // Initialize the tree
        initialize_tree();
        
        // Search the tree for a solution
        solution = search_tree();
        
        // Record the end time
        t2=clock();
        
        // Print the solution, if any
        if (solution==NULL)
            printf("No solution found.\n");
        else
        {
            printf("Solution found! (%d steps)\n",10);
            printf("Time spent: %f secs\n",((float) t2-t1)/CLOCKS_PER_SEC);
            write_solution_to_file(argv[4], 10);
        }
        
        // Print the selected algorithm, start, and target numbers
        printf("Enter the type of algo: %s is coded: %d\n", argv[1], method);
        printf("Enter the start number: %d\n", start);
        printf("Enter the target number: %d\n", target);
    }
    
    return 0;
}
