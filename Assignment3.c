// Implement efficient data structure and algorithm for finding shortest
// path from the start cell s to the destination cell t in a maze of dimension m × n.
//
// author: C. H. Chen
// date: 2024/10/9

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "position.h"
#include "path.h"
#include "queue.h"
#include "maze.h"

#define INFTY 2147483647
#define dir 4

typedef struct {
    short int v, h;
}direction;

direction move[dir] = {{1,0},{-1,0},{0,1},{0,-1}};

/*  opt = 0 => end when reach terminal. 
*   opt = 1 => search whole maze. */
int mazeBFS(maze_t* maze, int distance[], const int opt)
{
    assert(maze != NULL);
    for (int row = 0; row < maze->height; row++)
    {
        for (int col = 0; col < maze->width; col++)
        {
            position_t temp = {row, col};
            distance[offset(maze, temp)] = INFTY;
        }
    }
    distance[offset(maze, maze->end)] = 0;
    list_t * point = (list_t*)malloc(sizeof(list_t));
    list_t * adjacent_point = (list_t*)malloc(maze->height * maze->width * sizeof(list_t));
    position_t adjacent;
    point->position = maze->end;
    queue_t q = {NULL, NULL};
    enqueue(&q, point);
    int flag = 0;
    while (!queue_empty(&q))
    {
        point->position = dequeue(&q)->position;
        for (int i = 0; i < dir; i++)
        {
            adjacent.col = point->position.col + move[i].h;
            adjacent.row = point->position.row + move[i].v;
            if (adjacent.col < 0 || adjacent.row < 0 || adjacent.col > maze->width || adjacent.row > maze->height){
                continue;
            }
            adjacent_point->position = adjacent;
            switch (maze->cells[offset(maze, adjacent)])
            {
            case OPEN:
                enqueue(&q, adjacent_point);
                adjacent_point++;
                maze->cells[offset(maze, adjacent)] = VISITED;
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
                    enqueue(&q, adjacent_point);
                    adjacent_point++;
                    distance[offset(maze, adjacent)] = distance[offset(maze, point->position)] + 1;
                }
                break;
            default:
                break;
            }
        }
    }
    free(adjacent_point);
    free(point);
}// mazeBFS

list_t *shortest_path(maze_t* maze, int distance[]){
    list_t *path = (list_t*)malloc(sizeof(list_t));
    position_t adjacent;
    path->next = NULL;
    path->position = maze->start;
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
                if(d!=0)maze->cells[offset(maze, adjacent)] = PATH;
                path = list_add(adjacent,path);
                i = dir;
            }
        }
    }
    return path;
}


int main(void){
    FILE * fp;
    fp = fopen ("maze3.txt", "r");
    maze_t *maze = readMaze(fp);
    fclose(fp);
    int distance[maze->height * maze->width];
    mazeBFS(maze, distance, 0);
    list_t *path = shortest_path(maze, distance);
    printMaze(maze);
    list_print_reverse(path);
    list_free(path);
    freeMaze(maze);
    return 0;
}