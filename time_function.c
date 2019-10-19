#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "time_function.h"

#include <stdio.h>
#include <stdlib.h>
#include "time_function.h"
#include <time.h>
#include "initialization.h"

int find_waiting_time (int time_sec)
{
    int tab[2][8];

    init_schedule_tab(tab);

    return find_horaire(time_sec,tab);
}

struct tm* localtime(const time_t *secondes);

int time_calcul (Parcours calcul, int time_sec)
{
	int loop = 0, nb_stations = 0, code_departure, code_arrival, time_estim = 0, nb_chgt = 0, i;

	int find_time = find_waiting_time(time_sec); // integrate le waiting time

	while (loop < calcul.nb_chgt)//calculer le nb de stations du parcours
	{
		code_departure = search_code_station(calcul.way[loop].departure, calcul.way[loop].line);
		code_arrival = search_code_station(calcul.way[loop].arrival, calcul.way[loop].line);

		nb_stations = nb_stations + abs(code_arrival - code_departure);
		loop++;
	}

	for (i = 0; i < calcul.nb_chgt - 1; i++)
	{
		if (calcul.way[i+1].line != calcul.way[i].line)
			nb_chgt++;
	}

	if (find_time == 180)//heure blindée entre 6-8h ou 18-20h
		time_estim = nb_stations*30;//ajout 30s/station


	time_estim = time_estim + nb_stations*90 + nb_chgt*120 + nb_chgt*find_time; //tmps en scd, nbchgt*180 = attente d'un métro (estimé à 3min)

	return time_estim;
}





int search_code_station(Station station, int line_pos)
{
	int find_code, i = 0;

	find_code = station.tab[0].code;

	while (station.tab[i].line != line_pos && i < station.nb_line)// i < taille du tab pos
	{
		find_code = station.tab[i + 1].code;
		i++;
	}
	return find_code;
}

void convert_time(int time_conv, int *heures, int *minutes)
{
    *heures = (time_conv/3600)%60;
    time_conv = time_conv-(*heures)*3600;

    *minutes = (time_conv/60)%60;
    time_conv = time_conv - (*minutes) *60;

    if (time_conv > 0)
    {
        *minutes = ((*minutes) + 1)%60;
        if ((*minutes) == 0)
            (*heures) ++;
    }
}

void schedule_tab(int tab[2][8])
{
	//int schedule_tab[2][8];
	int i, j;

	init_schedule_tab(tab);

	for (i = 0; i < 2 ; i++)
	{
		for (j = 0; j < 9; j++)
        {
			printf("%d ",tab [i][j]);
        }
		printf("\n");
	}
}

void init_schedule_tab(int tab [2][8])
{
	 //on part de 2h --> tab[0][0]=2h
	tab [0][0] = 7200; tab [1][0] = -1; //2h
	tab [0][1] = 18000; tab [1][1] = 900;//5h
	tab [0][2] = 21600; tab [1][2] = 180;//6h  6h-8h BLINDEE
	tab [0][3] = 28800; tab [1][3] = 420;//8h
	tab [0][4] = 43200; tab [1][4] = 420;//12h
	tab [0][5] = 64800; tab [1][5] = 180;//18h  18h-20h BLINDEE 3min d'attente
	tab [0][6] = 72000; tab [1][6] = 300;//20h
	tab [0][7] = 79200; tab [1][7] = 600;//22h
	tab [0][8] = 86400; tab [1][8] = 1200;//00h
}


int convert_second(int heures, int minutes)
{
	int time_sec;

	time_sec = heures*3600 + minutes*60;

	return time_sec;
}


int find_horaire(int secondes_horaires,int tab[2][8])
{
    int i = 0;
    int find_time;

    if(secondes_horaires > 18000)
    {
        while(i < 9 && secondes_horaires > tab[0][i])
            i++;

        find_time = tab[1][i-1];//on a trouvé l'heure et l'horaire correspondant

        return find_time;
    }


    else if(7200 < secondes_horaires && secondes_horaires < 18000)
        return -1;

    else
        return tab [1][8]; //si le temps rentré est inferieur a 7200s, alors il est entre minuit et 2h->20min d'attente donc 1200s d'attente
}


int current_time()
{
    time_t secondes;
    int res_secondes;
    struct tm instant;

    time(&secondes);
    instant = *localtime(&secondes);
    res_secondes =(instant.tm_hour)*3600 + instant.tm_min*60 + instant.tm_sec;
    return res_secondes;
}

char * find_direction (Station arr, Station dep, int line_j)
{
	int code_departure, code_arrival;
	Station *cur = &dep;
	int k = 0;
	char *toreturn = (char*)calloc(50,sizeof(char));

	code_departure = search_code_station(dep, line_j);
	code_arrival = search_code_station(arr, line_j);

	do
    {
        strcpy(toreturn,cur->name);

        k = 0;
        while (cur->tab[k].line != line_j)//find the last station of the linked list
				k++;

        if (code_departure < code_arrival)
            cur = cur->tab[k].next;

        else if (code_departure > code_arrival)
            cur = cur->tab[k].prev;

    }while (cur != NULL);

    return toreturn;
}
