#ifndef DISPLAY_H_INCLUDED
#define DISPLAY_H_INCLUDED
#include "time.h"
#include "initialization.h"

void    display_result (Parcours *journeys, int *duration, Station *s, int size_j, int choice, StructGtk *S);
//int     display_sample (Sample *way,int beg, int size_way, char path[4000]);
int     display_sample (Sample *way,int beg, int size_way, char temp[400], char path[400], char path2[10]);

void    sort_by_time (int *duration, Parcours *journeys, int size_tab);

void    free_arrays (int size_tab, int **tab1, int ** tab2, Parcours journeys[]);

void    free_lists (Line * subways, int nb_line, int infos[2][size_lines]);

void    arrival_time (char arrival[150], int hdep, int mdep, int htrip, int mtrip);

#endif // DISPLAY_H_INCLUDED
