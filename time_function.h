#ifndef TIME_H_INCLUDED
#define TIME_H_INCLUDED

#include "initialization.h"
#include "time_function.h"

typedef struct sample
{
	Station departure;
	Station arrival;
	int line;
}Sample;

typedef struct parcours
{
	int nb_chgt;
	Sample *way;
}Parcours;

int time_calcul (Parcours calcul, int time_sec);

int find_waiting_time (int time_sec);

int search_code_station(Station station, int line_pos);

void convert_time(int time_conv, int *heures, int *minutes);

int convert_second(int heures, int minutes);

void schedule_tab(int tab[2][8]);

void init_schedule_tab(int tab [2][8]);

int find_horaire(int secondes_horaires,int tab[2][8]);

void display_time();

int current_time();

/*int time_calcul (Parcours calcul);

int search_code_station(Station station, int line_pos);

void convert_time(int time_conv, int *heures, int *minutes);*/

char * find_direction (Station arr, Station dep, int line_j);

#endif // TIME_H_INCLUDED
