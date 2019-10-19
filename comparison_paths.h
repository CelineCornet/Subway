#ifndef COMPARISON_PATHS_H_INCLUDED
#define COMPARISON_PATHS_H_INCLUDED
#include "initialization.h"
#include "time_function.h"

void        sort_paths          (Paths *f, Parcours journeys[f->nb_path], int choice, Station start, Station arrival, Station s[nb_station]);
void        decode_found        (int **predecessor, int **decode, int size_f);
void        obtain_infos        (int choice, Paths found,int **decode, int *result);
int         check_sort          (int *result, int choice, int size_f, int chgt[size_f]);

void        clean_found         (Paths *found,int ***dec, int **res, Station *s);
void        suppress_line       (int cpt, Paths *found, int ***decode, int **result);

void        sorting             (Paths found,int **decode, int *result);
void        sorting_journey     (int size_f, Parcours journeys[size_f], int chgt[size_f]);
int         min_val             (int *result);
void        swap_f              (Paths found, int **decode, int *result, int pos1, int pos2);
void        swap_j              (int size_f, Parcours journeys[size_f], int chgt[size_f], int pos1, int pos2);

void        decode_f            (Paths found, Parcours journeys[found.nb_path], int *decode, Station start, Station arrival, Station s[nb_station]);
void        compare_lines       (Sample *way, int sizE);
int         look_for_line       (int sens, int sample, int sizE, Sample *way, int res[10]);

void        display_infos       (int **decode, int *result, int size_f);

#endif // COMPARISON_PATHS_H_INCLUDED
