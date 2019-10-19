#ifndef CREATE_PROBLEMS_H_INCLUDED
#define CREATE_PROBLEMS_H_INCLUDED

#include "travaux.h"
#include "initialization.h"

#define input "problems.bin"
#define nb_prob 3
#define total_station 150

Station return_station (int number, Line subways[size_lines], int infos [2][size_lines]);

void create_works (Line subways[size_lines], int infos [2][size_lines]);

void Write (Problem new);

void Read (StructGtk *S);

void color(int line, char temp[500]);

#endif // CREATE_PROBLEMS_H_INCLUDED
