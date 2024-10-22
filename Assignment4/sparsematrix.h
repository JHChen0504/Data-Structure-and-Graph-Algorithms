/* Sparse matrix data structure */

#ifndef __SPARSEMATRIX_H__
#define __SPARSEMATRIX_H__

#define INFTY 2147483647
#define NaN -2147483647
#define EMPTY 0

#include "list.h"
#include "arithmetic.h"

const element_t EMPTY_DATA = {EMPTY, NaN};

typedef struct SparseMatrix{
    int m, n;
    list_t **row;
} SparseMatrix_t;

/* Verify the maze parameters have been read correctly and are valid (positive) */
int isValidMatrixHeader( const int numTokens, const int m, const int n)
{
    if (numTokens != 2)
    {
        fprintf(stderr,"Unable to read height and width from first line");
        return 0;
    }
    if (m<=0)
    {
        fprintf(stderr,"Row dimension must be positive, received %d\n", m);
        return 0;
    }
    if (n<=0)
    {
        fprintf(stderr,"column dimension must be positive, received %d\n", n);
        return 0;
    }
    return 1; 
}

/* Free the dynamically allocated memory associated with a SparseMatrix_t pointer. */
void matrix_free( SparseMatrix_t * M)
{
    if(M == NULL) return;
    free(M->row[0]);
    for (int i = 1; i <= M->m; i++)
    {
        list_free(M->row[i]);
    }
    free(M->row);
    free(M);
}

 /* Initialize each the list of row of M, list head = [ø] -> NULL */
void Initialize_row(SparseMatrix_t * M)
{
    for (int i = 1; i <= M->m; i++)
    {
        M->row[i] = (list_t*)malloc(sizeof(list_t));
        if (M==NULL)
        {
            perror("Unable to allocate required memory\n");
            matrix_free(M);/* Clean up before exit */
            exit(EXIT_FAILURE);
        }
        M->row[i]->data = EMPTY_DATA;
        M->row[i]->next = NULL;
    }
}

/* Read a matrix from the given filename. */
SparseMatrix_t * read_matrix( char* filename)
{
    FILE * fp;
    fp = fopen (filename, "r");

    assert( fp!= NULL );
    
    /* Matrix dimensions m x n */
    int m, n; 
    
    /* Read and verify header information */
    int numTokens = fscanf(fp, "%d %d ", &m, &n);

    if (!isValidMatrixHeader(numTokens, m, n))
        return NULL;
        
    /* Create pointer to list array of the appropriate size */
    list_t **M_rows = (list_t**)malloc((m + 1) * sizeof(list_t*));

    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory\n");
        exit(EXIT_FAILURE);
    }

    /* Create the complete sparse matrix structure */
    SparseMatrix_t* M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));

    if (M==NULL)
    {
        perror("Unable to allocate matrix structuren\n");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }

    /* Populate matrix structure */
    M->m = m;
    M->n = n;
    M->row = M_rows;
    
    Initialize_row(M);

    /* Loop over all file, reading and storing data */
    int row, col[2], value;
    for (row = 1; row <= m; row++)
    {
        col[1] = -INFTY;/* To check if the input column index is in ascending order */
        do
        {
            /* Read column index */
            fscanf(fp, "%d", &col[0]);
            if ((col[0] > n || col[0] < 0 || col[1] >= col[0])&& col[0] != 0)
            {
                fprintf(stderr,"Invalid column index input\n");
                matrix_free(M); /* Clean up before aborting */
                return NULL;
            }
            else if (col[0] == 0)
            {
                continue;
            }
            else
            {
                /* Read corresponding value */
                if ( fscanf(fp,"%d", &value) != 1)
                { /* Verify input read acceptably */
                    if (feof(fp))
                    fprintf(stderr,"Premature end of input while reading matrix\n");
                    else if (ferror(fp))
                    perror("Error reading matrix\n");
                    matrix_free(M); /* Clean up before aborting */
                    return NULL;
                }
                else
                {
                    /* Storing data to the list : [ø] -> ... -> [read_data] -> NULL */
                    element_t read_data = {col[0], value};
                    list_add( read_data, M->row[row]);
                }
            }
            col[1] = col[0];
        }while(col[0] != 0);// for col
    } // for row
    fclose(fp);
    return M;
} // ReadMatrix

/* Print the sparse matrix*/
void print_matrix( const SparseMatrix_t * M)
{
    if (M == NULL)
    {
        printf("No matrix\n");
        return;
    }
    printf("\n");
    for (int i = 1; i <= M->m; i++)
    {
        list_t *M_row = M->row[i]->next;
        int j = 1;
        while (M_row)
        {
            if (j < M_row->data.col_id)
            {
                printf("%4d ", EMPTY);
                j++;
            }
            if (j == M_row->data.col_id)
            {
                printf("%4d ", M_row->data.value);
                j++;
                M_row = M_row->next;
            }
        }
        for ( ; j <= M->n; j++)
        {
            printf("%4d ", EMPTY);
        }
        printf("\n");
    }
}

/* Entry-wise arithmetic operations of two matrices with the same dimension */
SparseMatrix_t * matrix_operation(const SparseMatrix_t * A, const SparseMatrix_t * B, int (*operation)(int, int))
{
    if (A->m != B->m || A->n != B->n)
    {
        fprintf(stderr,"Matrix dimension not the same\n");
        return NULL;
    }
    list_t **M_rows = (list_t**)malloc((A->m + 1) * sizeof(list_t*));
    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory\n");
        exit(EXIT_FAILURE);
    }
    SparseMatrix_t * M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));
    if (M==NULL)
    {
        perror("Unable to allocate matrix structure\n");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }
    M->m = A->m;
    M->n = A->n;
    M->row = M_rows;
    element_t data;

    Initialize_row(M);

    for (int i = 1; i <= A->m; i++)
    {
        list_t * A_row = A->row[i]->next;
        list_t * B_row = B->row[i]->next;
        
        while (A_row || B_row)
        {
            if (B_row == NULL) // i-th row of B is all zero 
            {
                list_add( A_row->data, M->row[i]);
                A_row = A_row->next;
            }
            else if (A_row == NULL) // i-th row of A is all zero 
            {
                data.col_id = B_row->data.col_id;
                data.value = operation(0, B_row->data.value);
                list_add( data, M->row[i]);
                B_row = B_row->next;
            }
            // both i-th row of A,B contains nonzero element in different column
            else if (A_row->data.col_id > B_row->data.col_id) 
            {
                data.col_id = B_row->data.col_id;
                data.value = operation(0, B_row->data.value);
                list_add( data, M->row[i]);
                B_row = B_row->next;
            }
            else if (A_row->data.col_id < B_row->data.col_id)
            {
                list_add( A_row->data, M->row[i]);
                A_row = A_row->next;
            }
            else // both i-th row of A,B contains nonzero element in the same column
            {
                data.col_id = A_row->data.col_id;
                data.value = operation(A_row->data.value, B_row->data.value);
                list_add( data, M->row[i]);
                A_row = A_row->next;
                B_row = B_row->next;
            }
        }// for col
    }// for row
    return M;
}// Matrix operations

/* Input a sparse matrix A, return A-transpose */
SparseMatrix_t * matrix_transpose( const SparseMatrix_t * A)
{
    /* Create pointer to list array of the appropriate size */
    list_t **M_rows = (list_t**)malloc((A->n + 1) * sizeof(list_t*));

    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory\n");
        exit(EXIT_FAILURE);
    }

    /* Create the complete sparse matrix structure (M = A^T)*/
    SparseMatrix_t* M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));

    if (M==NULL)
    {
        perror("Unable to allocate matrix structure\n");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }

    /* Populate matrix structure */
    M->m = A->n;
    M->n = A->m;
    M->row = M_rows;
    
    Initialize_row(M);

    int i, j;
    for (i = 1; i <= M->m; i++)
    {
        /* Add all nonzero element in i-th column of A to i-th row of M (= A^T) */
        for (j = 1; j <= A->m; j++)
        {
            list_t * A_col = list_find( i, A->row[j]);
            if (A_col)
            {
                element_t data = {j, A_col->data.value};
                list_add(data , M->row[i]);
            }
        }
    }
    return M;
}// matrix transpose

/* Multiplication of two matrices of dimension l × m and m × n */
SparseMatrix_t * matrix_multiply( const SparseMatrix_t * A, const SparseMatrix_t * B)
{
    if (A->n != B->m)
    {
        fprintf(stderr,"Matrix dimension inaccurate to perform multiplication\n");
        return NULL;
    }
    list_t **M_rows = (list_t**)malloc((A->m + 1)* sizeof(list_t*));
    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory\n");
        exit(EXIT_FAILURE);
    }
    SparseMatrix_t * M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));
    if (M==NULL)
    {
        perror("Unable to allocate matrix structure\n");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }
    M->m = A->m;
    M->n = B->n;
    M->row = M_rows;

    SparseMatrix_t * BT = matrix_transpose(B);

    Initialize_row(M);
    
    for (int i = 1; i <= M->m; i++)
    {
        for (int j = 1; j <= BT->m; j++)
        {
            int row_dot = 0;
            list_t * A_row = A->row[i]->next;
            if (A_row == NULL)
            {
                j = BT->m + 1;
                continue;
            }
            list_t * BT_row = BT->row[j]->next;
            while (A_row && BT_row)
            {
                list_t * A_BT = list_find(A_row->data.col_id, BT_row);
                if (A_BT != NULL)
                {
                    row_dot = row_dot + A_row->data.value * A_BT->data.value;
                }
                A_row = A_row->next;
            }
            if (row_dot != 0)
            {
                element_t data = { j, row_dot};
                list_add( data, M->row[i]);
            }
        }
    }
    matrix_free(BT);
    return M;
}// matrix multiply

#endif