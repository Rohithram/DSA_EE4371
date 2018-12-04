/*
Assignment 2
Name : Rohithram R
Roll no : EE16B031
Date: 25-08-2018
Aim : Program To evaluate a postfix expression (RPN) using C.
Procedure to run:
gcc rpneval.c
./a.out < input.txt
*/


//importing header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Assumptions made on max number of characters in each expression
#define max_len 256

//defining ascii values for operators
#define add 43
#define mul 42
#define sub 45
#define div 47

// To return when stack is empty
#define INT_MIN -1 

// A structure to represent a stack
struct StackNode
{
    double data;
    struct StackNode* next;
};

// Function to create a new node in stack
struct StackNode *newNode(double data){
    struct StackNode* node = (struct StackNode *) malloc(sizeof(struct StackNode));
    node -> data = data;
    node -> next = NULL;
    return node;
}

// Function to check whether the stack is empty or not
int isEmpty(struct StackNode *root)
{
    return !root;
}

// Function to insert an element into existing stack
void push(struct StackNode** root,double data){
    struct StackNode* stacknode = newNode(data);
    stacknode -> next = *root;
    *root = stacknode;
}

// Function to delete the top most element in the given stack
double pop(struct StackNode** root){
    if(isEmpty(*root))
        return INT_MIN;
    else{
        struct StackNode* temp = *root;
        *root = (*root) -> next;
        double popped = temp -> data;
        free(temp);
        return popped;
    }
}


/* 
Evaluate the expression using Reverse Polish Notation
Takes array of characters present in an expression and size of the array as input
Prints the evaluated result or Error if any encountered
*/
int rpneval(char **expression, int size)
{

    int i=0;
	int no_of_operands = 0;             // No of operands in an expression
	int no_of_operators = 0;            // No of operators in an expression
    int operand_ctr = 0;                // To track no of operands before an operator
    double res ;                        // To store the result after evaluation
    struct StackNode* root = NULL;      // Initialise the stack with Null pointer 

    // Scanning the expression
	for (i=0;i<size;i++)
	{

        //Checking if the character is a number or not and 
        //To count 0 as an operand , for cases like '0', '-0', '+0',etc
		char c1 = expression[i][0];
		char c2 = expression[i][1];
		if (atof(expression[i]) || (c1=='0') || (c2=='0'))
		{
			no_of_operands++;
            operand_ctr++;
			res = atof(expression[i]);
            push(&root,res);           // Inserting the operand into stack
			
		}   
        // To handle the case where operator should always be preceded by atleast 2 operands
        // for a valid expression
		else if(operand_ctr>=2)                 
		{
			no_of_operators++;
            // removing last 2 operands before current operator
			double oper1 = pop(&root);              
            double oper2 = pop(&root);
            
            //Storing the ascii value of operators
            int op = *expression[i];
            switch (op)
            {
                case add:
                    res = oper2 + oper1;
                    break;
                case sub:
                    res = oper2 - oper1;
                    break;
                case mul:
                    res  = oper2*oper1;
                    break;
                case div:
                    // To avoid DIVISION BY ZERO ERROR
                    if(oper1==0.0000){
                        printf("ERROR\n");
                        return 0;
                    }
                    else{
                        res = oper2/oper1;
                    }
                    break;
                default:
                    printf("Invalid operator \n");
                    break;
            }
            // Insert the result into the stack
            push(&root,res);
            // since we popped two elements and pushed the result into stack
            // so no of operands before current operator is just 1 less than previous value
            operand_ctr-=1;
        }else{
            // If an operator is not preceded by atleast two operands raise an ERROR
            printf("ERROR\n");
            return 0;
        }
    }

    // For a legal expression, the number of operands must be one more than the number of operators
	if (no_of_operands == no_of_operators + 1){
	    printf("%.4f\n",res);
    }	
	else printf("ERROR\n");
    return 0;
}

int main(){
 
    char* expression[max_len];			                // Array to store an expression characterwise
	char buffer[max_len];
	int charctr = 0;									// Count number of operands cum operators in an expression
	while (fgets(buffer,max_len, stdin))
	{
		charctr = 0;
		char* token = strtok(buffer, " \r\n");
		do 
		{
            // Dynamic memory allocation
			expression[charctr] = calloc(sizeof(char), strlen(token) + 1);
			strcpy(expression[charctr], token);
			charctr++;
		} while ((token = strtok(NULL, " \r\n")) != NULL);
        //function call to evaluate the expression
		rpneval(expression, charctr);
	}
	return 0;
}
