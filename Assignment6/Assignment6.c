// Implement max segment tree data structure of size o(4n)
// and an algorithm to construct the data structure in linear time. Then, an
// O(log n) time algorithm to process each query using the data structure.
//
// author: C. H. Chen
// date: 2024/11/26

#include<stdio.h>
#include<stdlib.h>
#include <time.h>

#define INFTY 2147483646

int n, m, *x, *r, *l;

int max(const int a, const int b)
{
    if (a >= b) return a;
    return b;
}

int mid(const int s, const int e) 
{
    return s + ((e - s) >> 1) ;
}

int intv_max(const int* st, const int ss, const int se,
             const int ll, const int rr, const int node)
{
    if (ll <= ss && rr >= se)
        return st[node];

    if (se < ll || ss > rr)
        return -INFTY;

    int m = mid(ss, se);
    return max(intv_max(st, ss, m, ll, rr, node << 1), intv_max(st, m + 1, se, ll, rr, 1 + (node<< 1)));
}

int get_max(const int* st, const int node, const int ll, const int rr)
{
    // Check for erroneous input values
    if (ll < 1 || rr > n || ll > rr) 
    {
        printf("Invalid Input");
        return -INFTY;
    }
    return intv_max(st, 1, node, ll, rr, 1);
}

void build_stree(int *stree, const int node)
{
    int tnode = node;
    for (int i = 0; i < node; i++)
    {
        stree[i] = -INFTY;
    }
    
    tnode >>= 1;
    for (int i = 0; i < n; i++)
    {
        stree[i+tnode] = x[i];
    }
    
    tnode >>= 1;
    while (tnode > 0)
    {
        for (int i = tnode; i < tnode << 1; i++)
        {
            stree[i] = max(stree[i << 1], stree[1 + (i << 1)]);
        }
        tnode >>= 1;
    }
}

void read_input(char *filename)
{
    FILE *fp;
    fp = fopen(filename, "r");

    // read n
    fscanf(fp,"%d", &n);
    
    x = (int*)malloc(n*sizeof(int));
    // read x
    for (int i = 0; i < n; i++)
    {
        fscanf(fp,"%d",&x[i]);
    }
    // read m
    fscanf(fp,"%d",&m);

    l = (int*)malloc(m*sizeof(int));

    r = (int*)malloc(m*sizeof(int));
    // read l,r
    for (int i = 0; i < m; i++)
    {
        fscanf(fp,"%d%d",&l[i], &r[i]);
    }
    fclose(fp);
}

#ifdef VERIFY
int verify(const int l, const int r, const int M)
{
    int err = 1;
    for (int i = l; i <= r; i++)
    {
        if (M == x[i - 1])
            err = 0;
        if (M < x[i - 1])
            return 1;
    }
    return err;
}
#endif

int main(void){
    read_input("test.txt");
    
    int node = 1;
    while (node < n)
    {
        node <<= 1;
    }
    node <<= 1;
    int *stree = (int*)malloc(node * sizeof(int));
    // Space complexity o(4n);
    build_stree(stree, node);
    node >>= 1;

#ifdef DEBUG
    clock_t start, end;
    double cpu_time_used = 0;
    int err = 0;
    for (int i = 0; i < m; i++)
    {   
        start = clock();
        get_max(stree, node, l[i], r[i]);
        end = clock();
        cpu_time_used += ((double) (end - start)) / CLOCKS_PER_SEC;
#ifdef VERIFY
        int M = get_max(stree, node, l[i], r[i]);
        err = verify(l[i], r[i], M);
        if(err)
        {
            printf("Error\n");
            break;
        }
#endif
    }
    if(!err)
        printf("No error\n");
    printf("Cpu time used : %.16f s\n", cpu_time_used);
#else
    for (int i = 0; i < m; i++)
    {
        printf("Max of values from %6d to %6d is %6d\n",
                l[i],r[i],get_max(stree, node, l[i], r[i]));
    }
#endif
    free(stree);free(x);free(r);free(l);
    return 0;
}