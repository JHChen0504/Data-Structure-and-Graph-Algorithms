// Implement stack data structure and algorithm for the evaluation of
// arithmetic expressions and assignment statement.
//
// author: C. H. Chen
// date: 2024/11/6

#include<stdio.h>
#include<stdlib.h>

#define NaN -2147483647
#define MAX_LENGTH 32
#define STACK_EMPTY -1

typedef enum {OPERATOR, OPERANT, VARIABLE}datatype;

typedef enum {false, true}bool;

typedef struct stack
{
    char op[3*MAX_LENGTH]; 
    double a[MAX_LENGTH];
    int op_top, a_top;
}stack_t;

/* Use linked list to store variable */
typedef struct list{
    char var_name;
    double data;
    struct list *next;
} list_t;

const char operator[] = "+-*/()";
const char number[] = "0123456789.";
const int precedence[6] = {1,1,2,2,0,0};
const int l_associate[6] = {1,1,1,1,1,1};

int op_num(const char op)
{
    for (int i = 0; i < 6; i++)
    {
        if (op == operator[i])
            return i;
    }
    return NaN;
} 

bool isnumber( const char c)
{
    for (int i = 0; i < 11; i++)
    {
        if (c == number[i])
            return true;
    }
    return false;
}

bool isoperator( const char c)
{
    for (int i = 0; i < 6; i++)
    {
        if (c == operator[i])
            return true;
    }
    return false;
}

/* Add the given data to the end of a list */
void list_add( char var_name, double data, list_t *list)
{
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    }
    list_t * last = list;
    while (last->next)
    {
        last = last->next;
    }
    last->next = added;
    added->next = NULL;
    added->var_name = var_name;
    added->data = data;
}

/* Find the corresponding data in the given list */
double list_find( const char var_name, list_t *list)
{
    list_t *temp = list;
    while (temp)
    {
        if (temp->var_name == var_name)
        {
            break;
        }
        temp = temp->next;
    }
    if (temp)   return temp->data; 
    return NaN;
}

/* Releases all the memory associated with the list. */
void list_free(list_t *head){
    list_t *temp;
    while(head){
        temp=head;
        head=head->next;
        free(temp);
    }
}

void stack_init(stack_t *s)
{
    s->a_top = STACK_EMPTY;
    s->op_top = STACK_EMPTY;
}

void push_char(stack_t *s, char c)
{
    s->op_top++;
    if (s->op_top == 3*MAX_LENGTH)
    {
        fprintf(stderr,"Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->op[s->op_top] = c;
}

void push_double(stack_t *s, double a)
{
    s->a_top++;
    if (s->a_top == MAX_LENGTH)
    {
        fprintf(stderr,"Stack overflow\n");
        exit(EXIT_FAILURE);
    }
    s->a[s->a_top] = a;
}

char pop_char(stack_t *s)
{
    s->op_top--;
    return s->op[s->op_top + 1];
}

double pop_double(stack_t *s)
{
    s->a_top--;
    return s->a[s->a_top + 1];
}

double evaluate(double a, double b, char op)
{
    switch (op)
    {
    case '+':
        return a + b;
        break;
    case '-':
        return a - b;
        break;
    case '*':
        return a * b;
        break;
    case '/':
        return a / b;
        break;
    default:
        fprintf(stderr,"Invalid input\n");
        exit(EXIT_FAILURE);
        break;
    }
}

list_t * Shunting_Yard(FILE *fp)
{
    stack_t s; 
    stack_init(&s);
    double a, b;
    char op = ' ', var_name = '\0', top, buff[620] = {'\0'};

    list_t * var = (list_t*)malloc(sizeof(list_t));
    if (var == NULL)
    {
        fprintf(stderr,"malloc failed\n");
        exit(EXIT_FAILURE);
    }
    var->var_name = var_name;
    var->data = NaN;
    var->next = NULL;
 

    while(op != EOF)
    {
        while (op == ' ' || op == '\n')
        {
            op = fgetc(fp);
        }
        if (!isoperator(op) && !isnumber(op))
        {
            var_name = op;
            do
            {
                op = fgetc(fp);
            }while(op == ' ');
            if (op != '=')
            {
                fprintf(stderr,"Invalid input\n");
                list_free(var);
                exit(EXIT_FAILURE);
            }
            op = fgetc(fp);
        }

        bool nextline = false;
        while (!nextline)
        {
            while (op == ' ')
            {
                op = fgetc(fp);
            }
            
            if (isnumber(op))
            {
                int i = 0;
                while (isnumber(op))
                {
                    buff[i] = op;
                    op = fgetc(fp);
                    i++;
                }
                sscanf(buff, "%lf", &a);
                push_double(&s, a);
                for ( ; i >= 0; i--)
                {
                    buff[i] = '\0';
                }
            }
            else
            {
                if(op == '\n') 
                {
                    while (s.op_top > 0)
                    {
                        b = pop_double(&s);
                        a = pop_double(&s);
                        push_double(&s, evaluate(a, b, pop_char(&s)));
                    }
                    b = pop_double(&s);
                    a = pop_double(&s);
                    list_add(var_name, evaluate(a, b, pop_char(&s)), var);
                    nextline = true;
                    continue;
                }
                if (op == EOF)
                {
                    while (s.op_top > 0)
                    {
                        b = pop_double(&s);
                        a = pop_double(&s);
                        push_double(&s,evaluate(a,b,pop_char(&s)));
                    }                    
                    b = pop_double(&s);
                    a = pop_double(&s);
                    list_add(var_name, evaluate(a, b, pop_char(&s)), var);
                    break;
                }  

                if (list_find(op, var)!=NaN)
                {
                    push_double(&s, list_find(op, var));
                    op = fgetc(fp);
                    continue;
                }
                
                switch (op)
                {
                case '(':
                    push_char(&s, op);
                    op = fgetc(fp);
                    break;
                case ')':
                    op = pop_char(&s);
                    while (op != '(')
                    {
                        b = pop_double(&s);
                        a = pop_double(&s);
                        push_double(&s, evaluate(a, b, op));
                        op = pop_char(&s);
                    }
                    op = fgetc(fp);
                    break;
                case '+':case '-':case '*':case '/':
                    if (s.op_top == STACK_EMPTY || s.op[s.op_top] == '(')
                    {
                        push_char(&s, op);
                        op = fgetc(fp);
                    }
                    else if(precedence[op_num(op)] > precedence[op_num(s.op[s.op_top])] 
                            || (precedence[op_num(op)] == precedence[op_num(s.op[s.op_top])] && !l_associate[op_num(op)]))
                    {
                        push_char(&s, op);
                        op = fgetc(fp);
                    }
                    else if(precedence[op_num(op)] < precedence[op_num(s.op[s.op_top])] 
                            || (precedence[op_num(op)] == precedence[op_num(s.op[s.op_top])] && l_associate[op_num(op)]))
                    {
                        while ((precedence[op_num(op)] < precedence[op_num(s.op[s.op_top])] 
                            || (precedence[op_num(op)] == precedence[op_num(s.op[s.op_top])] && l_associate[op_num(op)])) && (s.op_top != STACK_EMPTY))
                        {
                            top = pop_char(&s);
                            b = pop_double(&s);
                            a = pop_double(&s);
                            push_double(&s, evaluate(a, b, top));
                        }
                        push_char(&s, op);
                        op = fgetc(fp);
                    }
                    break;
                default:
                    fprintf(stderr,"Unknown variable name\n");
                    list_free(var);
                    exit(EXIT_FAILURE);
                    break;
                }// switch
            }// if else
        } //while next line
    }// while EOF
    return var;
}

int main(void){

    FILE * fp;
    char filename[]= "test.txt";
    char c = '\0';
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Fail to open file\n");
        exit(EXIT_FAILURE);
    }
    list_t *var =  Shunting_Yard(fp), *temp;
    fclose(fp);
    fp = fopen(filename, "r");
    if (fp == NULL)
    {
        fprintf(stderr,"Fail to open file\n");
        exit(EXIT_FAILURE);
    }
    while (c != EOF)
    {
        putchar(c);
        c = fgetc(fp);
    }
    putchar('\n');
    fclose(fp);
    temp = var->next;
    while (temp){
        if (temp->var_name == '\0')
        {
            printf("Ans = %3.10f\n", temp->data);
            temp = temp->next;
        }
        else
        {
            printf("%c = %3.10f\n", temp->var_name, temp->data);
            temp = temp->next;
        }
    }
    list_free(var);
}