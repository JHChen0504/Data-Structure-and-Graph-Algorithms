// Implement the following sorting algorithms and compare their performances.
//
// 1. Insertion sort
// 2. Selection sort
// 3. Quick sort
// 4. Merge sort
// 5. Heap sort
//
// author: C. H. Chen
// date: 2024/09/19

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define GenerateData 1

void insertionsort(const int n, int x[]){
    int y, j;
    for (int i = 2; i < n+1; i++)
    {
        y = x[i]; x[0] = y;
        j = i - 1;
        while (x[j] > y)
        {
            x[j+1] = x[j]; j--;
        }
        x[j+1] = y;
    }
    return;
}

void binaryinsertionsort(const int n, int x[]){
    int y, l, r, m;
    for (int i = 2; i < n+1; i++)
    {
        y = x[i];
        l = 1; r = i - 1;
        while (l <= r)
        {
            m = (l + r) >> 1;
            if(y < x[m]) r = m - 1; else l = m + 1;
        }
        for (int j = i - 1; j >= l; j--) x[j+1] = x[j];       
        x[l] = y;
    }
    return;
}

void selectionsort(const int n, int x[]){
    int y, k;
    for (int i = 1; i < n; i++)
    {
        y = x[i]; k = i;
        for (int j = 0; j <= n; j++)
        {
            if (x[j] < y)
            {
                y = x[j]; k = j;
            }
        }
        x[k] = x[i]; x[i] = y;
    }
    return;
}

void quicksort(int a[], const int l, const int r){
    int t, i = l, j = r, p = a[rand() % (r - l + 1) + l];
    while (i <= j)
    {
        while (a[i] < p) i++;
        while (p < a[j]) j--;
        if (i <= j)
        {
            t = a[i]; a[i] = a[j]; a[j] = t;
            i++; j--;
        }
    }
    if (l < j) quicksort(a, l, j);
    if (i < r) quicksort(a, i, r);
}

int nonrecursivequicksort(int x[], const int n){
    int *l, *r; //stack
    int s = 1, stack_height = 1;
    l = (int *) malloc(sizeof(int));
    r = (int *) malloc(sizeof(int));
    l[s] = 1; r[s] = n;
    while (s > 0)
    {
        l[0] = l[s]; r[0] = r[s]; s--;
        int i = l[0], j = r[0], p = x[rand() % (r[0] - l[0] + 1) + l[0]];
        while (i <= j)
        {
            while (x[i] < p) i++;
            while (p < x[j]) j--;
            if (i <= j)
            {
                int z = x[i]; x[i] = x[j]; x[j] = z;
                i++; j--;
            }
        }
        if (l < j) 
        {
            l = (int *) malloc(sizeof(int));
            r = (int *) malloc(sizeof(int));
            s++; l[s] = l; r[s] = j; stack_height++;
        }
        if (i < r) 
        {
            l = (int *) malloc(sizeof(int));
            r = (int *) malloc(sizeof(int));
            s++; l[s] = i; r[s] = r; stack_height++;
        }
    }
    return stack_height;
}

void mergesort(int a[] , const int l, const int r){
    int *b;
    b = (int *) malloc(sizeof(int) * sizeof(a));
    if (l < r)
    {
        int k = (l + r) >> 1;
        mergesort(a, l, k);
        mergesort(a, k + 1, r);
        merge(a, l, k, a, k + 1, r, b);
        for (int i = l; i <= r; i++) a[i] = b[i];
    }
}

void merge(int a[], const int l1, const int r1, int b[], const int l2, const int r2, int c[]){
    int i = l1, j = l2, k = l1;
    while ((i <= r1)&&(j <= r2))
    {
        if (a[i] < b[j]) {c[k] = a[i]; i++;}
        else {c[k] = b[j]; j++;}
        k++;
    }
    while (i <= r1){c[k] = a[i]; i++; k++;}
    while (j <= r2){c[k] = b[j]; j++; k++;}
}

void sift(int a[], const int r, const int n){
    int i = r, j = 2 * i, x = a[i];
    while (j <= n)
    {
        if (j < n) { if(a[j] < a[j + 1]) j++;}
        if (x > a[j]) break;
        a[i] = a[j]; i = j; j = 2 * i;
    }
    a[i] = x;
}

void heapsort(const int n, int x[]){
    int r = n >> 1;
    while (r > 0)
    {
        sift(x, r, n); r--;
    }
    int m = n;
    while (m > 0)
    {
        int y = x[1]; x[1] = x[m]; x[m] = y;
        m--; sift(x, 1, m);
    }
}

#ifdef DEBUG
int check(int arr[], const int n){
    for (int i = 1; i <= n; i++) if (arr[i] > arr[i+1]) return 1;
    return 0;
}
#endif

int main(unsigned int sn, int n){
    if (sn == NULL) sn = 5;
    srand(1);
    void (*iterativesorting)(int, int) = {&insertionsort, &binaryinsertionsort, 
    &selectionsort, &nonrecursivequicksort, &heapsort};
    void (*recursiveivesorting)(int, int, int) = {&mergesort, &quicksort};
    char sortingmethod[7][30] = {"Insertion sort", "Binary insertion sort", "Selection sort", 
                                "Non-recursive quick sort", "Heap sort", "Merge sort", "Quick sort"};
#if (GenerateData)
    int arr_size;
    printf("Input data size : \n");
    scanf("%d",&arr_size);
    printf("\nGenerating data of size %d...\n", arr_size);
    int arr[arr_size + 1];
    for (int i = 1; i <= arr_size; i++) arr[i] = i;
    for (int i = arr_size - 1; i > 0; i--)
    {
        int j = (rand() % (i+1)) + 1;
        int temp = arr[i];
        arr[i] = arr[j];
        arr[j] = temp;
    }
    printf("Storing data ...\n");
    char buff[20] = {'\0'}, filename[30] = {'\0'};
    int d = sprintf(buff,"%d", arr_size);
    snprintf(filename, sizeof(char) * d,"n_%s.txt", buff);
    FILE *randomdata;
    randomdata = fopen (filename, "w+");
    for (int i = 1; i <= arr_size; i++) fprintf(randomdata,"%d ", arr[i]);
    fclose(randomdata);
    printf("Complete\n\n");
#else
    char filename[30];
    printf("Input filename: \n");
    scanf("%s",filename);
    int arr_size = sscanf
    FILE *data;
    data = fopen (filename, "r");
    for (int i = 1; i <= arr_size; i++) fscanf(data,"%d", &arr[i]);
#endif

#ifndef DEBUG
    char buff[20] = {'\0'}, filename[30] = {'\0'};
    int d = sprintf(buff,"%d", arr_size), arr[arr_size + 1];
    snprintf(filename, sizeof(char) * d,"n_%s.txt", buff);
    FILE *data;
    data = fopen (filename, "r");
    for (int i = 1; i <= arr_size; i++) fscanf(data, "%d", &arr[i]);
    clock_t start, end;
    double cpu_time_used;
    if(sn < 1)
    printf("Insertion sort array size n = %d : ", arr_size);
    start = clock();
    insertionsort(arr_size, arr);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Insertion sort array size n = %d : ", arr_size);
#else 
    printf("Debug mode!\n");

#endif
    return 0;
}