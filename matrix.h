#ifndef MATRIX_H_INCLUDED
#define MATRIX_H_INCLUDED

#include "initialization.h"

#define node 13
#define nb_station 15
#define nothing 100

void init_weight_matrix (int weight_matrix[nb_station][nb_station]);

int is_node(Station* S, char croisement[nb_station-2][30]);

void init_node (char node_station[nb_station-2][30], Station s[nb_station], Line subways[size_lines], int infos[2][size_lines]);

void display_matrix(int matrix[node+2][node+2]);

void stationCompare(Line subways[size_lines], Station* start_station, Station* arr_station, int weight_matrix[nb_station][nb_station], Station s[nb_station], int infos[2][size_lines]);

int CompleteMatrix(Station* S, Station* S2, int weight_matrix[nb_station][nb_station],int index, char croisement[nb_station-2][30]);


#endif // MATRIX_H_INCLUDED
