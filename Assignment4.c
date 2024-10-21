// Implement appropriate data structures to support efficient operations on sparse matrices.
//
// author: C. H. Chen
// date: 2024/10/19

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

#define EMPTY 0
#define INFTY 2147483647

typedef struct matrix_element{
    int col_id, value;
} element_t;

element_t EMPTY_DATA = {EMPTY, EMPTY};

typedef struct list{
    element_t data;
    struct list *next;
} list_t;

typedef struct SparseMatrix{
    int m, n;
    list_t **row;
} SparseMatrix_t;

/* Add the given data to the end of a list */
void list_add( element_t data, list_t* last)
{
    list_t *added = (list_t*)malloc(sizeof(list_t));
    if (added == NULL){
        fprintf(stderr, "Error: unable to allocate required memory\n");
        return;
    }
    last->next = added;
    added->next = NULL;
    added->data = data;
}

/* Releases all the memory associated with the list. */
void list_free( list_t* list)
{
    list_t *temp;
    while (list!=NULL) {
        temp = list;
        list = list->next;
        free(temp);
    }
    free(list);
}

/* Verify the maze parameters have been read correctly and are valid (positive) */
int isValidMatrixHeader( int numTokens, int m, int n)
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
        fprintf(stderr,"Colunm dimension must be positive, received %d\n", n);
        return 0;
    }
    return 1; 
}

/* Free the dynamically allocated memory associated with a SparseMatrix_t pointer. */
void matrix_free( SparseMatrix_t * M)
{
    assert(M != NULL);
    for (int i = 0; i < M->m; i++)
    {
        list_free(M->row[i]);
    }
    free(M);
}

/* Read a matrix from the given file pointer. */
SparseMatrix_t * read_matrix( FILE* fp)
{
    assert( fp!= NULL );
    
    /* Matrix dimensions m x n */
    int m, n; 
    
    /* Read and verify header information */
    int numTokens = fscanf(fp, "%d %d ", &m, &n);

    if (!isValidMatrixHeader(numTokens, m, n))
        return NULL;
        
    /* Create cell array of the appropriate size */
    list_t **M_rows = (list_t**)malloc(m * sizeof(list_t*));

    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory");
        exit(EXIT_FAILURE);
    }

    /* Create the complete sparse matrix structure */
    SparseMatrix_t* M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));

    if (M==NULL)
    {
        perror("Unable to allocate matrix structure");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }

    /* Populate matrix structure */
    M->m = m;
    M->n = n;
    M->row = M_rows;

    /* Loop over all file, reading and storing data */
    int row, col[2], value;
    for (row = 0; row < m; row++)
    {
        /* Allocate list head for each row */
        M->row[row] = (list_t*)malloc(sizeof(list_t));
        if (M->row[row]==NULL)
        {
            perror("Unable to allocate required memory");
            matrix_free(M); /* Clean up before aborting */
            exit(EXIT_FAILURE);
        }

        /* Initialize list head = [ø] -> NULL */
        M->row[row]->next = NULL;
        M->row[row]->data = EMPTY_DATA;

        col[1] = INFTY;/* To check if the input colunm index is in ascending order */
        do
        {
            /* Read colunm index */
            fscanf(fp, "%d", col[0]);
            if (col[0] > n || col[0] < 0 || col[1] <= col[0])
            {
                fprintf(stderr,"Invalid colunm index input");
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
                    fprintf(stderr,"Premature end of input while reading matrix");
                    else if (ferror(fp))
                    perror("Error reading matrix");
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
    return M;
} // ReadMatrix

SparseMatrix_t * add_matrix( SparseMatrix_t * A, SparseMatrix_t * B)
{
    if (A->m != B->m || A->n != B->n)
    {
        printf(stderr,"Matrix dimension not the same");
        return NULL;
    }
    list_t **M_rows = (list_t**)malloc(A->m * sizeof(list_t*));
    if (M_rows==NULL)
    {
        perror("Unable to allocate required memory");
        exit(EXIT_FAILURE);
    }
    SparseMatrix_t * M = (SparseMatrix_t*)malloc(sizeof(SparseMatrix_t));
    if (M==NULL)
    {
        perror("Unable to allocate matrix structure");
        free(M_rows); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }
    
}


int main(void)
{
    clock_t start, end;
    double cpu_time_used = 0;
    FILE * fp;
    char filename[] = 'matrix.txt';
    fp = fopen (filename, "r");
    fclose(fp);
    start = clock();
    

    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("\n\nCpu time used : %.16f\n", cpu_time_used);
    return 0;
}