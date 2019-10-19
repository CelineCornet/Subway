#ifndef FIND_JOURNEYS_H_INCLUDED
#define FIND_JOURNEYS_H_INCLUDED

#define nothing 100

#include "matrix.h"

typedef struct paths
{
    int nb_path;
    int **weight;         //from 1 to the other stations
    int **predecessor;     //predecessor from the station
}Paths;

void     dijkstra                (Paths *f, int weight_matrix[nb_station][nb_station], Station s[nb_station]);

int     empty_array             (int to_look[nb_station]);
int     min_weight_in_to_look   (int to_look[nb_station], int weight[nb_station]);
void    update_array            (int looked_station[nb_station], int to_look[nb_station], int min);
void    find_successor          (int to_look[nb_station], int weight[nb_station], int successor[15]);
int     find_i                  (int i, int to_look[nb_station]);

void    process_successor       (int d, int successor, int cur, int line_weight, Paths *found);

void    display_arrays          (Paths found);


#endif // FIND_JOURNEYS_H_INCLUDED
