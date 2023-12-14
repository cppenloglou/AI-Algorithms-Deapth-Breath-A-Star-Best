#include "register_definitions.h"

#ifndef REGISTER_CALCULATIONS_H
#define REGISTER_CALCULATIONS_H

//Calculation of g
int claculateCost(int parent_num, int type) {

    int cost = -1;
    if(parent_num < 0)
        return cost;

    switch (type)
    {
        case -1:
            {cost = 0;
            break;}

        case INCREASE:
            {
                if(parent_num < pow(10, 9))
                    cost = 2;
                break;
            }

        case DECREASE:
            {
                if(parent_num > 0)
                    cost = 2;
                break;
            }

        case DOUBLE_OP:
            {
                if(parent_num > 0 && (2 * parent_num) <= pow(10,9))
                    cost = ceil(parent_num / 2.0) + 1;
                break;
            }

        case HALF:
            {
                if(parent_num > 0)
                    cost = ceil(parent_num / 4.0) + 1;
                break;
            }

        case SQUARE:
            {
                if(pow(parent_num, 2) <= pow(10,9))
                    cost = (parent_num * parent_num - parent_num)/4 + 1;
                break;
            }
            
        case ROOT_OP:
            {
                if(parent_num > 1 && fmod(sqrt(parent_num), 1) == 0)
                    cost = (parent_num - sqrt(parent_num))/4 + 1;
                break;
            }

        default:
            {printf("The type of act is invalid, try to use INCREASE, DECREASE or DOUBLE_OP\n");
            cost = -1;
            break;}
    }
    
    return cost;
}

int claculateNumber(int parent_num, int type) {

    int number = -1;
    int value  =  0;

    switch (type)
    {
        case -1:
            {number = parent_num;
            break;}

        case INCREASE:
            {
                if(parent_num < pow(10, 9))
                    number = parent_num + 1;
                break;
            }

        case DECREASE:
            {
                if(parent_num > 0)
                    number = parent_num - 1;
                break;
            }

        case DOUBLE_OP:
            {
                if(parent_num > 0 && (2 * parent_num) <= pow(10,9))
                {
                    value = parent_num * 2;
                    number = value;
                }
                break;
            }

        case HALF:
            {
                if(parent_num > 0)
                {
                    value  = parent_num / 2;
                    number = value;
                }
                break;
            }

        case SQUARE:
            {
                if(pow(parent_num, 2) <= pow(10,9))
                {
                    value  = parent_num * parent_num;
                    number = value;
                }
                break;
            }
            
        case ROOT_OP:
            {
                if(parent_num > 1 && fmod(sqrt(parent_num), 1) == 0)
                {
                    value  = sqrt(parent_num);
                    number = value;
                }
                break;
            }

        default:
            {printf("The type of act is invalid, try to use INCREASE, DECREASE or DOUBLE_OP\n");
            number = -1;
            break;}
    }

    return number;
}


//Calculation of h
int calculateH_Value(int parent_num, int type, int target) {

    int dist_h = -1;
    int value  =  0;

    switch (type)
    {
        case -1:
            {dist_h = heuristic(parent_num, target);
            break;}

        case INCREASE:
            {
                if(parent_num < pow(10, 9))
                    dist_h = heuristic(parent_num + 1, target);
                break;
            }

        case DECREASE:
            {
                if(parent_num > 0)
                    dist_h = heuristic(parent_num - 1, target);
                break;
            }

        case DOUBLE_OP:
            {
                if(parent_num > 0 && (2 * parent_num) <= pow(10,9))
                {
                    value = parent_num * 2;
                    dist_h = heuristic(value, target);
                }
                break;
            }

        case HALF:
            {
                if(parent_num > 0)
                {
                    value  = parent_num / 2;
                    dist_h = heuristic(value, target);
                }
                break;
            }

        case SQUARE:
            {
                if(pow(parent_num, 2) <= pow(10,9))
                {
                    value  = parent_num * parent_num;
                    dist_h = heuristic(value, target);
                }
                break;
            }
            
        case ROOT_OP:
            {
                if(parent_num > 1 && fmod(sqrt(parent_num), 1) == 0)
                {
                    value  = sqrt(parent_num);
                    dist_h = heuristic(value, target);
                }
                break;
            }

        default:
            {printf("The type of act is invalid, try to use INCREASE, DECREASE or DOUBLE_OP\n");
            dist_h = -1;
            break;}
    }

    return dist_h;
}

#endif