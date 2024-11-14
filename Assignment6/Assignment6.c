// Implement stack data structure and algorithm for the evaluation of
// arithmetic expressions and assignment statement.
//
// author: C. H. Chen
// date: 2024/11/6

#include<stdio.h>
#include<stdlib.h>

#define INFTY 2147483646

typedef struct data
{
    int n, m, *x, *l, *r;
}data_t;

int max(const int a, const int b)
{
    if (a >= b) return a;
    return b;
}

int * Stree(const int n, const int *x)
{
    int node = 1;
    while (node < n)
    {
        node <<= 1;
    }
    node <<=  1;
    int Stree[node];

    node >>= 1;
    for (int i = 0; i < n; i++)
    {
        Stree[i+node] = x[i];
    }
    for (int i = n + node; i < node << 1; i++)
    {
        Stree[i] = -INFTY;
    }

    while (node > 0)
    {
        node >>= 1;
        for (int i = node; i < node << 1; i++)
        {
            if (Stree[2 * i + 1] > -INFTY)
                Stree[i] = max(Stree[2 * i], Stree[2 * i + 1]);
            else
                i >>= 1;
        }
    }
    return Stree;   
}

data_t read_input(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");
    data_t data;
    char nextline;
    // read n
    fscanf(fp,"%d",&data.n);
    
    data.x = (int*)malloc(data.n*sizeof(int));
    // read x
    for (int i = 0; i < data.n; i++)
    {
        fscanf(fp,"%d",&data.x[i]);
    }
    // read m
    fscanf(fp,"%d",&data.m, &nextline);

    data.l = (int*)malloc(data.m*sizeof(int));

    data.r = (int*)malloc(data.m*sizeof(int));
    // read l,r
    for (int i = 0; i < data.m; i++)
    {
        fscanf(fp,"%d",&data.l[i]);
        fscanf(fp,"%d",&data.r[i]);
    }
    fclose(fp);
    return data;
}

int main(void){
    

    return 0;
}