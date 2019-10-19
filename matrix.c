#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "initialization.h"
#include "matrix.h"
#include "travaux.h"


void init_node (char node_station[nb_station-2][30], Station s[nb_station], Line subways[size_lines], int infos[2][size_lines])
{
    int cpt;

    strcpy(node_station[0], "etoile");
    strcpy(node_station[1], "palaisroyalmuseedulouvre");
    strcpy(node_station[2], "chatelet");
    strcpy(node_station[3], "placeditalie");
    strcpy(node_station[4], "nation");
    strcpy(node_station[5], "strasbourgsaintdenis");
    strcpy(node_station[6], "garedelest");
    strcpy(node_station[7], "chausseedantinlafayette");
    strcpy(node_station[8], "franklinroosevelt");
    strcpy(node_station[9], "trocadero");
    strcpy(node_station[10],"montparnasse");
    strcpy(node_station[11],"raspail");
    strcpy(node_station[12],"denfertrochereau");

    for (cpt = 0; cpt < node; cpt++)
        search_station(node_station[cpt], &(s[cpt+1]), subways, infos);
}

void display_matrix(int matrix[node+2][node+2])
{
	int i, j;

    printf ("   ");
	for (i=0; i<node+2; i++)
    {
        if (i<10)
            printf (" %d   ", i);
        else
            printf ("%d   ", i);
    }
    printf ("\n");

	for (i = 0; i < node + 2; i++)
	{
	    if (i<10)
            printf ("%d  ", i);
        else
            printf ("%d ", i);

		for (j = 0; j < node + 2; j++)
        {
            if (matrix[i][j] == 100)
                printf("%d  ",matrix[i][j]);
            else if (matrix[i][j] >= 10)
                printf("%d   ",matrix[i][j]);
            else
                printf(" %d   ",matrix[i][j]);
        }

		printf("\n");
	}
}

void init_weight_matrix (int matrix[node+2][node+2])
{
	int i, j = 0, read_value = 0;

	FILE * tofill = fopen("fill_matrix.txt", "r");

	if (tofill == NULL)
    {
        printf("File not found\n");
        exit (0);
    }

	for (i = 0; i < node+2 ; i++)
	{
		for (j = 0; j < node+2 ; j++)
		{
			fscanf(tofill, "%d", &read_value);
			if (read_value == -1)
                read_value = nothing;
			matrix [i][j] = read_value;
		}
		fscanf(tofill, "\n");
	}

	fclose(tofill);
}

int is_node(Station* S, char croisement[nb_station-2][30])
{
    int cpt = 2;// on commence station 2 soit etoile
    int i;
    char *tocompare = (char*)malloc(50*sizeof(char));

    if (S == NULL)
        return -1;

    strcpy(tocompare, S->name);
    tocompare = keep_letters(tocompare);

    for(i = 0; i < nb_station - 2; i++)
    {
        if(strcmp(tocompare,croisement[i]) == 0)
            return cpt;  // exemple : 2 -> etoile

        else
            cpt++;
    }
    return -1; //the station is not a node
}

void stationCompare(Line subways[size_lines], Station* start_station, Station* arr_station, int weight_matrix[nb_station][nb_station], Station s[nb_station], int infos[2][size_lines])
{
    int nbStart, nbArr;
    int lineArrival,lineStarting;
    char croisement[nb_station-2][30];

    init_weight_matrix(weight_matrix);
    init_node(croisement, s, subways, infos);
    modify_problems (weight_matrix, s); // modify the matrix according to the works

    nbStart = CompleteMatrix(start_station,arr_station, weight_matrix,0, croisement);


     //partie arr station
    nbArr = CompleteMatrix(arr_station, start_station, weight_matrix,nb_station-1, croisement);

    // --> objectif: remplir les cases weight_matrix [0][6] et weight_matrix [6][0]

    if (nbStart != -1 && nbArr != -1) // Both a croisement Station
        weight_matrix[0][nb_station-1] = weight_matrix[nb_station-1][0]= weight_matrix[nbStart-1][nbArr-1];

    else if (nbStart == -1 && nbArr == -1) // Both not croisement
    {
         lineArrival = arr_station->tab[0].line; //ex : ligne 2
         lineStarting = start_station->tab[0].line;

         if(lineArrival != lineStarting)
             weight_matrix[0][nb_station-1] = weight_matrix[nb_station-1][0] = nothing;
    }
}

int CompleteMatrix(Station* S, Station* S2, int weight_matrix[nb_station][nb_station],int index, char croisement[nb_station-2][30])
{
        int numeroStation;
        Station *tmp = S;

        int weight = 0, j, k;
        int nbnext = -1, nbprev = -1;

        numeroStation = is_node(S,croisement);


        if(numeroStation != -1) //station is a node station
        {
            for(j = 1; j < nb_station-1; j++)
                weight_matrix[index][j] = weight_matrix[j][index] = weight_matrix[numeroStation-1][j];// bien besoin du -1 !
        }

        else    // not a node station
        {
            do
            {
                weight++;
                k = 0;

                while (tmp->tab[k].line != S->tab[0].line && k < tmp->nb_line)
                    k++;

                tmp = tmp->tab[k].next;

                if (tmp != NULL && real_node(*tmp) > 1) // see if their is more than 1 line at the station
                    nbnext = is_node(tmp,croisement);

            }while (nbnext == - 1 && tmp != NULL);

           if (nbnext != -1) //station croisement trouvée
           {
                if(strcmp(croisement[nbnext-2],keep_letters(S2->name))== 0) //si station 1 non croisement et station 2 croisement
                {
                    weight_matrix[index][nbnext-1] = weight_matrix[nbnext-1][index] = weight;
                    weight_matrix[0][nb_station-1] = weight_matrix[nb_station-1][0] = weight;
                }
                else    //station depart et arrivé non croisement
                    weight_matrix[index][nbnext-1] = weight_matrix[nbnext-1][index] = weight;
           }

            weight = 0;
            tmp = S;

            do
            {
                weight++;
                k = 0;

                while (tmp->tab[k].line != S->tab[0].line && k < tmp->nb_line)
                    k++;

                tmp = tmp->tab[k].prev;

                if (tmp != NULL && real_node(*tmp) > 1) // see if their is more than 1 line at the station
                    nbprev = is_node(tmp,croisement);

            }while (nbprev == - 1 && tmp != NULL);

            if (nbprev != -1) //station depart et arrivé non croisement
            {
                if(strcmp(croisement[nbprev-2],keep_letters(S2->name))== 0) //si station 1 non croisement et station 2 croisement
                {
                    weight_matrix[index][nbprev-1] = weight_matrix[nbprev-1][index] = weight;
                    weight_matrix[0][nb_station-1] = weight_matrix[nb_station-1][0] = weight;
                }
                else
                    weight_matrix[index][nbprev-1] = weight_matrix[nbprev-1][index] = weight;
            }

            for(j = 0;j < nb_station-1;j++)
                 weight_matrix[j][index] = weight_matrix[index][j]; // on remplie ligne

            if (nbnext != -1 && nbprev != -1)
                weight_matrix[nbprev-1][nbnext-1] = weight_matrix[nbnext-1][nbprev-1] = nothing; //plus jointes
        }

        return numeroStation;
}
