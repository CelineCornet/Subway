#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "find_journeys.h"

void dijkstra(Paths *f, int weight_matrix[nb_station][nb_station], Station s[nb_station])
{
    Paths found = *f;
    int looked_station[nb_station]= {0} ;    //looked station = S
    int to_look[nb_station];               // to look = T
    int i, cpt=0, min;
    int successor[15];                                // successors of the value found
    int d, size_origin;                                                 // distance between a station and 1
    int linked = -1;

    //initialization to_look && looked_station
    to_look[0] = -1;
    for (i=1; i<nb_station; i++)
    {
        to_look[i] = i;
        looked_station[i] = -1;
    }


    //initialization of successor
    for(i=0; i<15; i++)
        successor[i] = nothing;

    //we allocate found.weight and found.predecessor with 1 line and "nb_station" column
    found.nb_path = 1;
    found.weight = (int **) malloc (sizeof(int*));

    if (found.weight == NULL)
    {
        printf ("error");
        exit (0);
    }

    found.weight[0] = (int*) malloc (sizeof(int)*nb_station);

    found.predecessor = (int**) malloc (sizeof(int*));

    if (found.predecessor == NULL)
    {
        printf("error");
        exit(0);
    }

    found.predecessor[0]= (int*) malloc (sizeof(int)*nb_station);


    //we fill in the array found.weight
    for (i=0; i<nb_station; i++)
        found.weight[0][i] = weight_matrix[i][0];


    //we fill in the array found.predecessor
    for (i=0; i<nb_station; i++)
    {
        if (found.weight[0][i] != 0 && found.weight[0][i] != nothing)
            found.predecessor[0][i]=0;
        else
            found.predecessor[0][i]= nothing;
    }

    /*display_arrays(found);
    system("pause");*/

    while (empty_array(to_look)!=-1)          //while to_look isn't empty
    {
        min = min_weight_in_to_look(to_look, found.weight[0]);        //min = value in to_look having the smallest weight
        //printf("min : %d", min);

        // we remove min from to_look and put it in looked_station
        update_array (looked_station, to_look, min);
        if (found.weight[0][min] != 0)
        {
            //we find the successor of min in to_look
            find_successor(to_look, weight_matrix[min], successor);

            cpt=0;
            /*printf ("\nsuccessor : ");
            while (successor[cpt]!=nothing)
            {
                 printf("%d  ", successor[cpt]);
                 cpt++;
            }

            printf ("\n");*/

            // we check that no successors are equals (junction)
            cpt=0;
            while (successor[cpt]!=nothing)
                cpt++;
            cpt--;

            linked = -1;
            if (successor[cpt] == nb_station-1)
            {
                i=0;

                if (strcmp ( s[successor[cpt]].name, s[min].name) == 0 )
                    linked = min;

                while (i<cpt && linked == -1)
                {
                    //printf ("croisement 1: %s, croisement 2: %s\n",s[successor[cpt]].name, s[successor[i]].name);
                    //system("pause");
                    if (strcmp ( s[successor[cpt]].name, s[successor[i]].name) == 0 )
                    //if (found.weight[0][successor[i]] == found.weight[0][successor[cpt]] && found.predecessor[0][successor[i]]==found.predecessor[0][successor[cpt]])
                    {
                        linked = i;         //successor i is the same as the arrival station
                        //printf ("\nstation %d and %d linked ! \n", successor[cpt], successor[i]);
                    }
                    i++;
                }
            }

            cpt = 0;
            //We find the distance between each successors and 1
            while (successor[cpt]!= nothing)             //while there is still some successor to process
            {
                size_origin = found.nb_path;

                //we check the arrival station is not the same as the successor we check (junction) comparing the successor weight and predecessor to min
                if ( (successor[cpt] != nb_station-1 || linked == -1) && (successor[cpt] != nb_station-1 || min != linked))
                {
                    for (i = 0; i < size_origin; i++)
                    {
                        //printf ("successor : %d\n", successor[cpt]);
                        d = found.weight[i][min] + weight_matrix[successor[cpt]][min];
                        process_successor(d, successor[cpt], min, i, &found);

                        //we check it is useful to check the next line (to avoid duplication)
                        /*d_old = d;
                        if (i != size_origin-1)
                        {
                            d = found.weight[i+1][min] + weight_matrix[successor[cpt]][min];
                            if ( d == d_old && found.predecessor[i+1][successor[cpt]] != 100)
                                i++;            //we don't have to check the next line so we directly go after it
                        }*/

                    }

                    if (successor[linked] == successor[cpt])   //we copy in the arrival station what we found
                    {
                        for (i=0; i<found.nb_path; i++)
                        {
                            found.predecessor[i][nb_station-1] = found.predecessor[i][successor[linked]];
                            found.weight[i][nb_station-1] = found.weight[i][successor[linked]];
                        }
                    }
                    /*display_arrays(found);
                    system("pause");*/
                }
                cpt++;
            }

        }
        else        //departure station is a junction
        {
            for (i=0; i<found.nb_path; i++)
                found.predecessor[i][min] = 0;
        }
        //re initialization of the array successor
        for(i = 0; i < 15; i++)
            successor[i] = nothing;

    }
    *f = found;
}

int empty_array(int to_look[nb_station])
{
    int cpt=0;
    int check = 0;

    while (cpt < nb_station - 1 && check != 1)      //if there is only the arrival station left, we don't have to handle it
    {
        if (to_look[cpt]!=-1)
            check = 1;
        cpt++;
    }

    if (check == 1)
        return 0;
    return -1;
}

int min_weight_in_to_look (int to_look[nb_station], int weight[nb_station])
{
    int min = 0;
    int i;

    //min takes the first value different from -1
    while (to_look[min]==-1)
        min++;

    for (i=min+1; i<nb_station-1; i++)
    {
        if (to_look[i]!= -1 && weight[to_look[i]] < weight[min])
            min = to_look[i];
    }

    return min;
}

void update_array (int looked_station[nb_station], int to_look[nb_station], int min)
{
    int i = 0, j=0;

    //we find the value in to look
    while (to_look[i] != min)
        i++;

    //we find the first non used case of looked_station
    while (looked_station[j]!= -1)
        j++;

    looked_station[j] = to_look[i];
    to_look[i] = -1;
}

void find_successor (int to_look[nb_station], int weight[nb_station], int successor[15])
{
    int i=0, cpt=0;

    while (i< nb_station)
    {
        if (weight[i] != nothing && find_i (i, to_look) != -1)
        {
            successor[cpt]=i;
            cpt++;
        }
        i++;
    }
}

int find_i (int i, int to_look[nb_station])     // we look for the value i in the array to_look and return -1 if it is not
{
    int j=0;

    while (j<nb_station && to_look[j]!=i)
        j++;

    if (j==nb_station)
        return -1;
    return 0;

}

void display_arrays (Paths found)
{
    int i, j;

    for (j=0; j<found.nb_path; j++)
    {
        printf ("\n                 ");
        for (i=1; i<=nb_station; i++)
        {
            if (i<11)
                printf (" %d   ", i-1);
            else if (i==11)
                printf (" %d   ", i-1);
            else
                printf ("%d   ", i-1);

        }

        printf ("\nweight[%d] :      ", j);
        for (i=0; i<nb_station; i++)
        {
            if (found.weight[j][i] <10)
                printf(" %d   ", found.weight[j][i]);
            else if (found.weight[j][i] !=100)
                printf("%d   ", found.weight[j][i]);
            else
                printf("%d  ", found.weight[j][i]);
        }



        printf("\npredecessor[%d] : ", j);
        for (i = 0; i<nb_station; i++)
        {
            if (found.predecessor[j][i] <10)
                printf(" %d   ", found.predecessor[j][i]);
            else if (found.predecessor[j][i] !=100)
                printf("%d   ", found.predecessor[j][i]);
            else
                printf("%d  ", found.predecessor[j][i]);
        }

        printf("\n");
    }

}

void process_successor (int d, int successor, int cur, int line_weight, Paths *found)
{

    int d_old;                              //p_old = old predecessor
    int j;

    if (d < found->weight[line_weight][successor])
    {
        d_old = found->weight[line_weight][successor];

        if (d_old != nothing )
        {
            // we re allocate the arrays of the structure
            found->nb_path++;

            found->weight = (int **) realloc(found->weight,sizeof(int*)*(found->nb_path));                 ///très probable erreur : est ce que la dimension dans le main est modifié ?
            found->weight[found->nb_path-1] = NULL;

            found->weight[found->nb_path-1] = (int*) malloc (sizeof(int)*nb_station);

            found->predecessor = (int**) realloc(found->predecessor,sizeof(int*)*found->nb_path);                 ///très probable erreur : est ce que la dimension dans le main est modifié ?
            found->predecessor[found->nb_path-1] = NULL;

            found->predecessor[found->nb_path-1] = (int*) malloc (sizeof(int)*nb_station);

            //we copy the old path in the first empty line of found.weight = current size - 1 (since first case at 0 and not 1
            for (j = 0; j<nb_station; j++)
            {
                found->weight[found->nb_path-1][j]= found->weight[line_weight][j];
                found->predecessor[found->nb_path-1][j] = found->predecessor [line_weight][j];
            }
        }


        found->weight [line_weight][successor] = d;
        found->predecessor [line_weight][successor] = cur;

    }
    else
    {
        //we place in the next line d and copy the rest

        found->nb_path++;

        found->weight = (int **) realloc(found->weight,sizeof(int*)*found->nb_path);
        found->weight[found->nb_path-1] = NULL;

        found->weight[found->nb_path-1] = (int*) malloc (sizeof(int)*nb_station);

        found->predecessor = (int**) realloc(found->predecessor,sizeof(int*)*found->nb_path);
        found->predecessor[found->nb_path-1] = NULL;

        found->predecessor[found->nb_path-1] = (int*) malloc (sizeof(int)*nb_station);

        //we copy the new path in the first empty line of found.weight = current size - 1 (since first case at 0 and not 1)
        for (j = 0; j<nb_station; j++)
        {
            if (j!=successor)
            {
                found->weight[found->nb_path-1][j]= found->weight[line_weight][j];
                found->predecessor[found->nb_path-1][j] = found->predecessor [line_weight][j];
            }
            else
            {
                found->weight[found->nb_path-1][j]= d;
                found->predecessor[found->nb_path-1][j] = cur;
            }
        }

    }
}
