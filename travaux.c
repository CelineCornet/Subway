#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>

#include "initialization.h"
#include "create_problems.h"
#include "travaux.h"


void tot_init(int infos[2][size_lines], Line subways [size_lines], StructGtk *S)
{
    init_tab(infos, subways);
    create_list (subways, infos);
    create_works(subways, infos); // create randomly works in stations
    Read(S); // display the works
    init_works(subways, infos); // to modify the list with the works
}

int display_state(int station_1, int station_2, StructGtk *S)
{
    char temp [100] = "";
    char temp2 [100] = "";
    char label [200] = "";

    if (station_1 == 1 && station_2 == 1)
        return 0;

    if (station_1 == 3)
    {
        sprintf(temp, "\tYou're already in this station. You do not need to move\t\n");
        strcat(label, temp);
    }



    else
    {
        if (station_1 == 0)
            sprintf(temp, "The start station is not a known station\n");
        else if (station_1 == 2)
            sprintf(temp, "The start station is under works, it is not accessible for the moment\n");
        strcat(label, temp);

        if (station_2 == 0)
            sprintf(temp2, "The arrival station is not a known station \n");
        else if (station_2 == 2)
            sprintf(temp2, "The arrival station is under works, it is not accessible for the moment\n");
        strcat(label, temp2);
    }

    gtk_label_set_text(S->disp_warning, label);
    gtk_widget_show(S->warning);

    return 1;
}

void init_works (Line subways[size_lines],  int infos [2][size_lines])
{
    FILE *input_file = fopen(input, "rb");
    Problem read;

    if (input_file == NULL)
    {
        printf("File not found\n");
        exit(0);
    }

    while (feof(input_file) == 0) // check if need to put a read before :D
    {
        fread (&read, sizeof(Problem),1,input_file);
        work(read, subways, infos);
    }

    fclose (input_file);
}


void work (Problem toadd, Line subways [size_lines], int infos [2][size_lines])
{
    Station *location;
    int k = 0;

    location = search_tomodify(toadd.name, subways, infos);

    if (location != NULL) //station found
    {
        while (location->tab[k].line != toadd.line) // find the right case to put the information
            k ++;

        location->tab[k].info = toadd.code; // put the code in the station
    }
}

Station* search_tomodify(char *name, Line subways[], int infos[2][size_lines])
{
    int cpt, k, flag = 0;
    Station *found;
    char *tocompare = (char*)malloc(50*sizeof(char));

    name = keep_letters(name);

    for (cpt = 0; cpt < size_lines; cpt ++) // search in the lines declared
    {
        found = subways[cpt];// initialize the search to the beginning of each lines

        do
        {
            strcpy(tocompare, found->name);
            tocompare = keep_letters(tocompare);

            if (strcmp(tocompare, name)== 0)// earlier creation of the station found
                return found;

            for (k = 0; k < found->nb_line; k++)
            {
                if (found->tab[k].line == infos[0][cpt])
                    flag = k;
            }

            found = found->tab[flag].next;

        }
        while (found != NULL);  //
    }

    return NULL; // Station not found
}

void modify_problems (int weight_matrix [nb_station][nb_station], Station node_station[nb_station])
{
    int res, i, k, j, comm;
    int link [2];
    int cpt = 0, add = 0;

    for (i = 1; i < nb_station-1; i ++) // i give us the position on the array thus on the matrix
    {
        for (k = 0; k < node_station[i].nb_line; k++) // k give us the pos tab
        {
            cpt = 0;
            if (node_station[i].tab[k].info != 0)
            {
                //printf("the station %s is being modify for the line %d\n",node_station[i].name,node_station[i].tab[k].line);
                /// for each other station of tab node station see if on the same line // if yes then get the station out
                for (j = 1; j < nb_station-1; j++)
                {
                    res = same_line (node_station[i].tab[k].line, node_station[j]);
                    comm = nb_common_line(node_station[i], node_station[j]);

                    if (res == 1 && i != j && comm == 0 && weight_matrix [i][j] != nothing) // put nothing instead of the weight it was between those station
                    {
                        //printf("matrix entre %s et %s modifié\n", node_station[i].name, node_station[j].name);
                        //printf("both station have %d line in commun\n", comm);
                        link[cpt] = j; // memorize the value of the connection in the matrix.
                        cpt ++;
                        add = add + weight_matrix [i][j];
                        weight_matrix [i][j] = weight_matrix [j][i] = nothing;
                    }
                }
            }
            if (cpt == 2) //connect both station in the matrix
            {
                //printf("BECAUSE matrix entre %s et %s modifié\n", node_station[link[0]].name, node_station[link[1]].name);
                weight_matrix [link[0]][link[1]] = weight_matrix [link[1]][link[0]] = add;
            }
            //system("pause");
        }
    }

}

int same_line (int line_1, Station s2)
{
    int i;

    for (i = 0; i < s2.nb_line; i ++)       // rajouter une condition si les stations sont reliés sur une autre ligne !!
    {
        if (line_1 == s2.tab[i].line)
            return 1;
    }
    return 0; // both station do not have a line in common
}

int nb_common_line (Station s1, Station s2)
{
    int i, k, cpt = 0;

    for (i = 0; i < s1.nb_line; i ++)       // rajouter une condition si les stations sont reliés sur une autre ligne !!
    {
        for (k = 0; k < s2.nb_line; k ++)
        {
            if (s1.tab[i].line == s2.tab[k].line && s2.tab[k].info != 1 && s1.tab[i].info != 1)
            {
                cpt ++;
                //printf("station %s et %s line commun %d\tcpt : %d\n", s1.name, s2.name, s1.tab[i].line, cpt);
            }
        }
    }
    return cpt; // both station do not have a line in common
}

int real_node (Station tosee)
{
    int cpt = 0, i;

    for (i = 0; i < tosee.nb_line; i++)
    {
        if (tosee.tab[i].info == 0)
            cpt = cpt + 1;
    }

    return cpt;
}

int is_there_a_way(int matrix[nb_station][nb_station])
{
    int i, cpt = 0;

    for (i = 0; i < nb_station; i++)
    {
        if (matrix[0][i] != nothing && matrix[0][i] != 0)
            cpt++;
    }

    if(cpt == 0)
        return 0;

    cpt = 0;
    for (i = 0; i < nb_station; i++)
    {
        if (matrix[nb_station-1][i] != nothing && matrix[nb_station-1][i] != 0)
            cpt++;
    }

    if (cpt == 0)
        return 0;

    return 1; //the arrival and end station are linked to others stations
}
