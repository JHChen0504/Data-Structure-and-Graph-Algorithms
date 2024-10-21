// Implement efficient data structure and algorithm for finding shortest
// path from the start cell s to the destination cell t in a maze of dimension m × n.
//
// author: C. H. Chen
// date: 2024/10/9

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <time.h>

// This library only contains the function associated to read and print maze file.
// This library is from https://weinman.cs.grinnell.edu/courses/CSC161/2021F/homework/src/maze/maze.c
#include "maze.h"

// All the following library is implemented by myself.
#include "position.h"
#include "path.h"
#include "queue.h"


#define INFTY 2147483647
#define dir 4

typedef struct {
    short int v, h;
}direction;

direction move[dir] = {{1,0},{-1,0},{0,1},{0,-1}};

/*  opt = 0 => end when reach terminal. 
*   opt = 1 => search whole maze. */
void mazeBFS( const maze_t* maze, int *distance, const int opt)
{
    assert(maze != NULL);
    cell_t maze_cells[maze->height*maze->width];

    for (int i = 0; i < maze->height*maze->width; i++)
    {
        distance[i] = INFTY;
        maze_cells[i] = maze->cells[i];
    }

    distance[offset(maze, maze->end)] = 0;
    list_t adjacent_point;
    position_t adjacent;
    adjacent_point.position = maze->end;
    adjacent_point.next = NULL;
    queue_t q = {NULL, NULL};
    enqueue(&q, &adjacent_point);
    int flag = 0;
    while (!queue_empty(&q))
    {
        list_t * point = dequeue(&q);
        for (int i = 0; i < dir; i++)
        {
            adjacent.col = point->position.col + move[i].h;
            adjacent.row = point->position.row + move[i].v;
            if (adjacent.col < 0 || adjacent.row < 0 || adjacent.col > maze->width || adjacent.row > maze->height){
                continue;
            }
            adjacent_point.position = adjacent;
            switch (maze_cells[offset(maze, adjacent)])
            {
            case OPEN:
                enqueue(&q, &adjacent_point);
                maze_cells[offset(maze, adjacent)] = VISITED;
                distance[offset(maze, adjacent)] = distance[offset(maze, point->position)] + 1;
                break; 
            case START:
                if (!opt && !flag)
                {
                    flag = 1;
                    distance[offset(maze, adjacent)] = distance[offset(maze, point->position)] + 1;
                    queue_initialize(&q);
                }
                else if (!flag)
                {
                    flag = 1;
                    enqueue(&q, &adjacent_point);
                    distance[offset(maze, adjacent)] = distance[offset(maze, point->position)] + 1;
                }
                break;
            default:
                break;
            }
        }
        free(point);
    }
}// mazeBFS

// Return the path lists containing the shortest path from end position to start position.
list_t * shortest_path( const maze_t* maze, const int *distance, cell_t *path_cells){
    list_t *path = (list_t*)malloc(sizeof(list_t));
    position_t adjacent;
    path->next = NULL;
    path->position = maze->start;
    for (int i = 0; i < maze->height * maze->width; i++)
    {
        path_cells[i] = OPEN;
    }
    int d = distance[offset(maze, maze->start)];
    if (d == INFTY)
    {
        printf("No path from start to end.\n");
        return NULL;
    }
    while (d > 0)
    {
        for (int i = 0; i < dir; i++)
        {
            adjacent.col = path->position.col + move[i].v;
            adjacent.row = path->position.row + move[i].h;
            if(distance[offset(maze, adjacent)] == d - 1){
                d--;
                if(d!=0) path_cells[offset(maze, adjacent)] = PATH;
                path = list_add(adjacent, path);
                i = dir;
            }
        }
    }
    return path;
}

int main(void){
    clock_t start, end;
    double cpu_time_used = 0;

    // Read maze file
    FILE * fp;
    fp = fopen ("maze79.txt", "r");
    maze_t *maze = readMaze(fp);
    fclose(fp);

    // Declare distance array and path cell array.
    int maze_size = maze->height * maze->width;
    int distance[maze_size];
    cell_t path_cells[maze_size];

    // Search the maze and find the shortest path.
    start = clock();
    mazeBFS(maze, distance, 0);
    list_t *path = shortest_path(maze, distance, path_cells);
    end = clock();

    // Output section.
    for (int i = 0; i < maze_size; i++)
    {
        if (path_cells[i] == PATH)
        {
            maze->cells[i] = PATH;
        }
    }
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printMaze(maze);
    list_print_reverse(path);
    printf("\n\nShortest path length : %d\n",offset(maze, maze->start));
    printf("Cpu time used : %.16f\n", cpu_time_used);

    // Free all the dynamic allocated memories.
    list_free(path);
    freeMaze(maze);
    return 0;
}