/* Set of functions to read and print a maze data structure */

#ifndef __MAZE_H__
#define __MAZE_H__

#include "position.h"

typedef /* Various states a maze cell may be in */
enum cell {OPEN, BLOCKED, VISITED, PATH, START, END}
  cell_t;

/* Maze structure with dimensions height x width. Maze cells are a height*width
   contiguous block of memory in row-major order (offset = width*row + col). */
typedef struct maze {
    int height, width;
    position_t start, end;
    cell_t* cells;
} maze_t;

/* Verify the maze parameters have been read correctly and are valid (positive) */
int isValidMazeHeader(int numTokens, int height, int width)
{
    if (numTokens != 2)
    {
        fprintf(stderr,"Unable to read height and width from first line");
        return 0;
    }
    if (height<=0)
    {
        fprintf(stderr,"height must be positive, received %d\n", height);
        return 0;
    }
    if (width<=0)
    {
        fprintf(stderr,"width must be positive, received %d\n", width);
        return 0;
    }
    return 1; 
}

/* Free the dynamically allocated memory associated with a maze_t pointer. */
void freeMaze(maze_t * maze)
{
    assert(maze != NULL);
    free(maze->cells);
    free(maze);
}

/* Read a maze from the given file stream pointer. */
maze_t* readMaze(FILE* stream)
{
    assert( stream!= NULL );

    int height, width; /* Maze dimensions */
    position_t start = {-1, -1};
    position_t end = {-1, -1};
    
    /* Read and verify header information */
    int numTokens = fscanf(stream, "%d %d ",&height,&width);

    if (!isValidMazeHeader(numTokens,height,width))
        return NULL;
        
    /* Create cell array of the appropriate size */
    cell_t *p_cells = (cell_t*)malloc(height*width*sizeof(cell_t));

    if (p_cells==NULL)
    {
        perror("Unable to allocate cell array");
        exit(EXIT_FAILURE);
    }

    /* Create the complete maze structure */
    maze_t* maze = (maze_t*)malloc(sizeof(maze_t));

    if (maze==NULL)
    {
        perror("Unable to allocate maze structure");
        free(p_cells); /* Clean up before exit */
        exit(EXIT_FAILURE);
    }

    /* Populate maze structure */
    maze->height = height;
    maze->width = width;
    maze->cells = p_cells;

    /* Loop over all cells, reading and storing data */
    int row, col;
    cell_t* p_cell = p_cells;
    char cellChar;
    for (row=0 ; row<height; row++)
    {
        for (col=0 ; col<width ; col++, p_cell++)
        {
        /* Read next character */
        if ( fscanf(stream,"%c",&cellChar) != 1)
        { /* Verify input read acceptably */
            if (feof(stream))
            fprintf(stderr,"Premature end of input while reading maze");
            else if (ferror(stream))
            perror("Error reading maze");
            freeMaze(maze); /* Clean up before aborting */
            return NULL;
        }
        else
        switch(cellChar)
        {
        case 's': case 'S':
            *p_cell = START;
            start.row = row;
            start.col = col;
            maze->start = start;
            break;
        case 't': case 'T':
            *p_cell = END;
            end.row = row;
            end.col = col;
            maze->end = end;
            break;
        case ' ': case '0':
            *p_cell = OPEN;
            break;
        default:
            *p_cell = BLOCKED;
            break;
        } // switch
    } // for col
    /* Read newline */
    int lineEnd = fgetc(stream);

    if (lineEnd != '\n' && row!=height-1) /* Newline not required in last row */
    {
        if (feof(stream))
            fprintf(stderr,"Premature end of input while reading maze");
        else if (ferror(stream))
            perror("Error reading maze");
        freeMaze(maze); /* Clean up before aborting */
        return NULL;
    }
    } // for row

    if (start.row == -1)
    {
            fprintf(stderr, "Error in maze input: No start position denoted");
            freeMaze(maze);
            return NULL;
    }

    if (end.row == -1)
    {
        fprintf(stderr, "Error in maze input: No end position denoted");
        freeMaze(maze);
        return NULL;
    }
    return maze;
} // readMaze

/* Print a maze visualization to standard output */
void printMaze(const maze_t* maze)
{
    assert(maze != NULL);
    int height = maze->height;
    int width = maze->width;
    cell_t * p_cell = maze->cells;
    char ch;
    int color = 0;
    
    for(int row=0 ; row<height ; row++)
    {
        for (int col=0 ; col<width ; col++, p_cell++)
        {
        switch (*p_cell)
        {
        case OPEN:    ch=' '; break;
        case BLOCKED: ch='X'; break;
        case VISITED: ch=' '; break;
        case START:   ch='S'; break;
        case PATH:    ch='.'; break;
        case END:     ch='T'; break;
        default:      ch='?'; break; /* Included in case someone changes cell_t */
        }
        printf("%c",ch);
        }
        printf("\n");
    }
}
      
/* Get the offset for a position in the given maze */
int offset(const maze_t* maze, position_t position)
{
    assert( position.row >= 0);
    assert( position.col >= 0);
    assert( position.row < maze->height);
    assert( position.col < maze->width);

    return maze->width * position.row  +  position.col;
}

#endif