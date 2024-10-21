// Implement at least the following sorting algorithms and compare their performances.
//
// 1. Insertion sort
// 2. Selection sort
// 3. Quick sort
// 4. Merge sort
// 5. Heap sort
//
// author: C. H. Chen
// date: 2024/09/24

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

const char sortingmethod[7][20] = {"Insertion", "Binary Insertion", "Selection", "Quick", "Non-recursive Quick", "Merge", "Heap"};

void insertionsort(const int n, int x[]){
    int y, j, k = x[0];
    for (int i = 2; i <= n; i++)
    {
        y = x[i]; x[0] = y;
        j = i - 1;
        while (x[j] > y)
        {
            x[j+1] = x[j]; j--;
        }
        x[j+1] = y;
    }
    x[0] = k;
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
        for (int j = i + 1; j <= n; j++)
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

int nonrecursivequicksort(const int n, int x[]){
    int tmp = n,lgn = 1;
    while (tmp>0)
    {
        tmp = tmp / 2;
        lgn++;
    }
    printf("%d",lgn);
    int l[2*lgn], r[2*lgn]; //stack
    int s = 1, stack_height = 1;
    l[s] = 1; r[s] = n;
    while (s > 0)
    {
        l[0] = l[s]; r[0] = r[s]; s--;
        int k = rand()%(r[0]-l[0]+1)+l[0];
        int i = l[0], j = r[0], p = x[k];
        while (i <= j)
        {
            while (x[i] < p) { i++;}
            while (p < x[j]) { j--;}
            if (i <= j)
            {
                int z = x[i]; x[i] = x[j]; x[j] = z;
                i++; j--;
            }
        }
        if (l[0] < j) 
        {
            s++; 
            l[s] = l[0]; r[s] = j;
            if(stack_height < s) stack_height = s;
        }
        if (i < r[0]) 
        {
            s++; 
            l[s] = i; r[s] = r[0];
            if(stack_height < s) stack_height = s;
        }
    }
    printf("stack_height = %d\n",stack_height);
    return stack_height;
}

void merge(int a[], const int l, const int m, const int r){
    int i = l, j = m + 1, k = l;
    int b[r - l + 1];
    while ((i <= m)&&(j <= r))
    {
        if (a[i] < a[j]) {b[k - l] = a[i]; i++;}
        else {b[k - l] = a[j]; j++;}
        k++;
    }
    while (i <= m){b[k - l] = a[i]; i++; k++;}
    while (j <= r){b[k - l] = a[j]; j++; k++;}
    for (int n = 0; n < r - l + 1; n++)
    {
        a[n+l] = b[n];
    }
}

void mergesort(int a[], const int l, const int r){
    if (l < r)
    {
        int m = (l + r) / 2;
        mergesort(a, l, m);
        mergesort(a, m + 1, r);
        merge(a, l, m, r);
    }
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
    int r = n / 2;
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
int check(const int n, int arr[]){
    for (int i = 1; i < n; i++) if (arr[i] > arr[i+1]) return 1;
    return 0;
}
#endif

int main(void){
    unsigned int arr_size = 100, count = 0;
    clock_t start, end;
    double cpu_time_used[7][100] = {0};
    do{
        int i = 0;
        for ( ; i < 7; i++)
        {
            srand(arr_size);
            int arr[arr_size + 1];
            for (int j = 1; j <= arr_size; j++) arr[j] = j;
            for (int j = arr_size - 1; j > 0; j--)
            {
                int k = rand()%(j+1)+1;
                int temp = arr[j];
                arr[j] = arr[k];
                arr[k] = temp;
            }
            switch (i)
            {
            case 0:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                insertionsort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 1:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                binaryinsertionsort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 2:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                selectionsort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 3:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                quicksort(arr, 1, arr_size);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 4:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                nonrecursivequicksort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 5:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                mergesort(arr, 1, arr_size);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 6:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                heapsort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            }
#ifdef DEBUG
            if(check(arr_size, arr)){
                printf("Error\n");
                return -1;
            }
#endif
        }
        count++;
        if (arr_size < 1000) arr_size = arr_size+100;
        else if (arr_size < 10000 ) arr_size = arr_size+1000;
        else arr_size = arr_size+10000;
    }while (arr_size < 100000);
    do{
        int i = 3;
        for ( ; i < 7; i++)
        {
            srand(arr_size);
            int arr[arr_size + 1];
            for (int j = 1; j <= arr_size; j++) arr[j] = j;
            for (int j = arr_size - 1; j > 0; j--)
            {
                int k = rand()%(j+1)+1;
                int temp = arr[j];
                arr[j] = arr[k];
                arr[k] = temp;
            }
            switch (i)
            {
            case 3:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                quicksort(arr, 1, arr_size);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 4:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                nonrecursivequicksort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 5:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                mergesort(arr, 1, arr_size);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            case 6:
                printf("%s sort array size %d ...\n",sortingmethod[i], arr_size);
                start = clock();
                heapsort(arr_size, arr);
                end = clock();
                cpu_time_used[i][count] = ((double) (end - start)) / CLOCKS_PER_SEC;
                printf("Associated cpu time %.3f\n\n",cpu_time_used[i][count]);
                break;
            }
#ifdef DEBUG
            if(check(arr_size, arr)){
                printf("Error\n");
                return -1;
            }
#endif
        }
        count++;
        arr_size = arr_size+10000;
    }while (arr_size < 250000);

    FILE * fp;
    fp = fopen ("mytable.txt", "w+");
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 30; j++)
        {
            fprintf(fp, "%f ", cpu_time_used[i][j]);
        }
        fprintf(fp, "\n");
    }
    fclose(fp);

    return 0;
}
