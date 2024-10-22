// Implement appropriate data structures to support efficient operations on sparse matrices.
//
// author: C. H. Chen
// date: 2024/10/23

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#include "sparsematrix.h"

int main(void)
{
    clock_t start, end;
    double cpu_time_used = 0;
    char matrixA[] = "test_data_1.txt";
    char matrixB[] = "test_data_2.txt";

    int output = 0;

    SparseMatrix_t *A, *B, *A_plus_B, *A_minus_B, *AT, *AB;

    for (int i = 0; i < 6; i++)
    {
        switch (i)
        {
            case 0:
                start = clock();
                A = read_matrix(matrixA);
                end = clock();printf("A = :");
                if(output)
                {
                    
                    print_matrix(A);
                }
                break;
            case 1:
                start = clock();
                B = read_matrix(matrixB);
                end = clock();printf("B = :");
                if(output)
                {
                    
                    print_matrix(B);
                }
                break;
            case 2:
                start = clock();
                A_plus_B = matrix_operation(A, B, &add);
                end = clock();printf("A + B = :");
                if(output)
                {
                    
                    print_matrix(A_plus_B);
                }
                matrix_free(A_plus_B);
                break;
            case 3:
                start = clock();
                A_minus_B = matrix_operation(A, B, &sub);
                end = clock();printf("A - B = :");
                if(output)
                {
                    
                    print_matrix(A_minus_B);
                }
                matrix_free(A_minus_B);
                break;
            case 4:
                start = clock();
                AT = matrix_transpose(A);
                end = clock();
                printf("A^T = :");
                if(output)
                {
                    print_matrix(AT);
                }
                matrix_free(AT);
                break;
            case 5:
                start = clock();
                AB = matrix_multiply(A, B);
                end = clock();printf("AB = :");
                if(output)
                {
                    
                    print_matrix(AB);
                }
                matrix_free(AB);
                break;
        }
        cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
        printf("\nCpu time used : %.4f s\n\n", cpu_time_used);
    }
    matrix_free(A);
    matrix_free(B);
    return 0;
}