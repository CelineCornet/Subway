#ifndef TRAVAUX_H_INCLUDED
#define TRAVAUX_H_INCLUDED

#include "matrix.h"
#include "initialization.h"
#define input "problems.bin"


/* code of problems
		1 = travaux
		2 = colis
		3 = accident (ralentissement) [toute la ligne possible si name == NULL]
*/

typedef struct pro
{
	char name[50]; // name station
	int line; // line of problems
	int code;
}Problem;


void on_ok_warnings_clicked (GtkWidget *wid, StructGtk *S);

void tot_init(int infos[2][size_lines], Line subways [size_lines], StructGtk *S);

void init_works (Line subways[size_lines], int infos [2][size_lines]);

void work (Problem toadd, Line subways [size_lines], int infos [2][size_lines]);

int display_state(int station_1, int station_2, StructGtk *S);

Station* search_tomodify(char *name, Line subways[], int infos[2][size_lines]);

void modify_problems (int weight_matrix [nb_station][nb_station], Station node_station[nb_station]);

int same_line (int line_1, Station s2);

int nb_common_line (Station s1, Station s2);

int real_node (Station tosee);

int is_there_a_way(int matrix[nb_station][nb_station]);

#endif // TRAVAUX_H_INCLUDED

