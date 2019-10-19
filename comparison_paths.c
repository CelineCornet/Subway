#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "find_journeys.h"
#include "comparison_paths.h"
#include "time_function.h"
#include "display.h"


///Sort
void sort_paths(Paths *f, Parcours journeys[f->nb_path], int choice, Station start, Station arrival, Station s[nb_station])
{
    int *result;            // time or change
    int **decode;            //path to take
    int i, j;
    Paths found = *f;

    int chgt[found.nb_path+1], change, cpt;

    //either we sort them by weight (= time) or by nb of line to take

    //we allocate result according to the number of paths
    result = (int*) malloc (sizeof(int)*(found.nb_path+1));
    if (result == NULL)
    {
        printf ("error\n");
        exit (0);
    }
    //we initialize it to nothing (=100)
    for (i=0; i<found.nb_path+1; i++)
        result[i] = nothing;

    decode = (int**) malloc (sizeof(int*)*found.nb_path);        //nb_station+2 = nb_max of station to take
    if (decode == NULL)
    {
        printf ("error\n");
        exit (0);
    }

    for (i=0; i<found.nb_path; i++)
        decode[i] = (int*) malloc (sizeof(int)*(nb_station+2));

    // we initialize it to nothing
    for (i=0; i<found.nb_path; i++)
    {
        for (j=0; j<nb_station+2; j++)
            decode[i][j] = nothing;
    }

    decode_found (found.predecessor, decode, found.nb_path);
    obtain_infos (choice, found, decode, result);

    clean_found(&found, &decode, &result, s);

     //display_arrays(found);
     //system("pause");

    //according to "result" we sort the lines of found.weight and found.predecessor
    if (check_sort (result, 1, found.nb_path, chgt)!=1)         //not sorted
        sorting (found, decode, result);

    //we decode the structure found to send it to the function time

    for (i = 0; i < found.nb_path; i++)
        decode_f(found, &journeys[i], decode[i], start, arrival, s);

    for (i = 0; i < found.nb_path; i++)
    {
        //we find the nb of change
        change=0;
        cpt=0;
        while (cpt<journeys[i].nb_chgt-1)
        {
            if (journeys[i].way[cpt].line != journeys[i].way[cpt+1].line)
                change++;
            cpt++;
        }
        chgt[i]=change;
    }

    chgt[found.nb_path] = nothing;

    if (check_sort (result, 2, found.nb_path, chgt)!=1)         //not sorted
        sorting_journey (found.nb_path, journeys, chgt);

    *f = found;

    // we free result and decode
    free(result);

    for (i=0; i<found.nb_path; i++)
        free(decode[i]);
    free(decode);
}

void obtain_infos (int choice, Paths found,int **decode, int *result)
{
    int i, cpt = 0;

    // we find either the number of stations to take or the nb of change

    for (i=0; i<found.nb_path; i++)
    {
        cpt=0;
        if (choice == 1)        //time
        {
            // we read the weight in found.weight[nb_station-1]
            result[i] = found.weight[i][nb_station-1];
        }
        else                    //change
        {
            //we use a counter to know the number of change
            while (decode[i][cpt]!=nothing)
                cpt++;
            cpt--;              //nb of change = nb of station -1

            result[i]= cpt;
        }
    }
}

void decode_found(int **predecessor, int **decode, int size_f)
{
    int cpt = nb_station-1;
    int j=0, i;

    //starting from 7 (=6 in the code) we find each path using the arrays predecessor
    for (i=0; i<size_f; i++)    //for each paths
    {
        j=0;
        cpt = nb_station-1;

        while (cpt != 0 && cpt != nothing)        //we fill in decode
        {
            decode[i][j]= cpt;
            cpt = predecessor[i][cpt];
            j++;
        }
        decode[i][j]= cpt;
    }
}

///Sort
int check_sort (int *result, int choice, int size_f, int chgt[size_f])
{
    int cpt=1;
    int min;
    min = result[0];

    if (choice ==1)         //time
    {
        while (result [cpt] != nothing && min < result[cpt])
        {
            min = result[cpt];
            cpt++;
        }

        if (result[cpt]==nothing)
            return 1;
        return 0;
    }
    else                    //nb_change
    {
        min = chgt[0];
        while (min < chgt[cpt])
        {
            min = chgt[cpt];
            cpt++;
        }

        if (cpt == size_f)
            return 1;
        return 0;
    }

    return 0;
}

void sorting (Paths found,int **decode, int *result)
{
    int position;

    if (found.nb_path<=0)
        return;

    position = min_val (result);
    swap_f (found, decode, result, 0, position);

    found.nb_path--;
    sorting (found, decode, result+1);

}

void sorting_journey (int size_f, Parcours journeys[size_f], int chgt[size_f])
{
    int position;

    if (size_f <=0)
        return;

    position = min_val(chgt);
    swap_j (size_f, journeys,chgt, 0, position);

    sorting_journey (size_f-1, journeys+1, chgt+1);

}

int min_val (int *result)
{
    int cpt=1;
    int min = 0;

    while (result[cpt]!= nothing)
    {
        if (result[min] > result[cpt])
            min = cpt;
        cpt++;
    }

    return min;
}

void swap_f(Paths found, int **decode, int *result, int pos1, int pos2)
{
    int temp, i;
    int t[nb_station];

    //sort result
    temp = result[pos1];
    result[pos1]=result[pos2];
    result[pos2]=temp;

    //sort decode
    for (i=0; i<nb_station; i++)
    {
        t[i] = decode[pos1][i];
        decode[pos1][i]=decode[pos2][i];
        decode[pos2][i]=t[i];
    }


    //sort found.weight
    for (i=0; i<nb_station; i++)
    {
        t[i] = found.weight[pos1][i];
        found.weight[pos1][i]= found.weight[pos2][i];
        found.weight[pos2][i]= t[i];

    }

    //sort found.predecessor
    for (i=0; i<nb_station; i++)
    {
        t[i] = found.predecessor[pos1][i];
        found.predecessor[pos1][i]= found.predecessor[pos2][i];
        found.predecessor[pos2][i]= t[i];
    }



}

void swap_j(int size_f, Parcours journeys[size_f], int chgt[size_f], int pos1, int pos2)
{
    Parcours temp;
    int t;

    //sort chgt
    t = chgt[pos1];
    chgt[pos1] = chgt[pos2];
    chgt[pos2] = t;

    //sort journeys
    temp = journeys[pos1];
    journeys[pos1]= journeys[pos2];
    journeys[pos2]= temp;
}

///Decode
void decode_f (Paths found, Parcours journeys[found.nb_path], int *decode, Station start, Station arrival, Station *s)
{
    //we find the size of decode
    int cpt=0;


    while (decode[cpt] != 0 && decode[cpt] != nothing )
        cpt++;

    journeys->nb_chgt = cpt;                //temporary nb of change until we know how many lines we take

    (journeys->way) = (Sample*) malloc (sizeof(Sample)*(journeys->nb_chgt));
    if (journeys->way == NULL)
    {
        printf("error");
        exit(0);
    }
    //we copy in journeys the path to follow and find the line corresponding to each sample of journey
    for (cpt=0; cpt<(journeys->nb_chgt); cpt++)
    {
        journeys->way[cpt].departure = s[decode[journeys->nb_chgt-cpt]];
        journeys->way[cpt].arrival = s[decode[journeys->nb_chgt-(cpt+1)]];
    }

    compare_lines (journeys->way, journeys->nb_chgt);
    /*for (cpt=0; cpt<journeys->nb_chgt; cpt++)
        printf ("%s --> %s // line : %d\n", journeys->way[cpt].departure.name, journeys->way[cpt].arrival.name, journeys->way[cpt].line);
    *///system("pause");

}

void compare_lines (Sample *way, int sizE)
{
    int i, j, check=0, res[sizE][10];
    int cpt;

    Station arrival;
    Station departure;

    //initialization of res at 0
    for (i=0; i<sizE; i++)
    {
        for (j=0; j<sizE; j++)
            res[i][j]=0;
    }

    for (cpt=0; cpt<sizE; cpt++)
    {
        i = 0;
        arrival = way[cpt].arrival;
        departure = way[cpt].departure;

        if (arrival.nb_line == 1)
            way[cpt].line = arrival.tab[0].line;

        else if (departure.nb_line == 1)
            way[cpt].line = departure.tab[0].line;
        else
        {

            check = 0;
            i = 0;
            while (i<departure.nb_line )
            {
                j = 0;
                while (j < arrival.nb_line)
                {
                    if (departure.tab[i].line == arrival.tab[j].line && departure.tab[i].info != 1 && arrival.tab[j].info != 1)
                    {
                        //res = departure.tab[i].line;
                        res[cpt][check] = departure.tab[i].line;
                        check++;
                    }
                    j++;
                }
                i++;
            }

            if (check == 1)         //only one line found
                way[cpt].line = res[cpt][0];
            else                    //two lines found
                way[cpt].line = -1;

        }

    }

    for(cpt=0; cpt<sizE; cpt++)
    {

        if (way[cpt].line == -1)
            way[cpt].line = look_for_line (1, cpt, sizE, way, res[cpt]);
        if (way[cpt].line == -1)                        //if we didn't find any line on the right of this sample
            way[cpt].line = look_for_line (-1, cpt, sizE, way, res[cpt]);
        if (way[cpt].line == -1)                        //if we didn't find any line on the left of this sample
        {
            i = 0;
            while (res[cpt][i] != 6 && res[cpt][i] != 4 && res[cpt][i] != 1 && res[cpt][i] != 7)
                i++;

            way[cpt].line = res[cpt][i];
        }
    }
}

int look_for_line (int sens, int sample, int sizE, Sample *way, int res[10])
{
    int line = -1;
    int i = 0;

    if (sample+sens>0 && sample+sens <sizE)
    {
        if (way[sample+sens].line == -1)
            line = look_for_line(sens, sample+sens, sizE, way, res);
        else
        {
            while (way[sample+sens].line != res[i] && res[i]!=0)
                i++;
            if (res[i]!=0)                      //line found match with the sample
                line = way[sample+sens].line;
        }
    }
    return line;
}

///clean
void clean_found (Paths *found,int ***dec, int **res, Station *s)
{
    int i, j=0, m;
    int **decode = *dec;
    int *result = *res;
    int check=0;

    //we suppress the paths that goes twice through the same station (only possible when arrival = junction)
    char name_station [nb_station][50]= {{'\0'}};

    for (i=0; i<found->nb_path; i++)
    {
        // we collect the name of each station we have to pass through
        j = 0;
        while (decode[i][j]!= nothing)
        {
            strcpy (name_station[j], s[decode[i][j]].name);
            j++;
        }

        //we check if there are two times the same station
        m=0;
        check = -1;
        while (strcmp (name_station[m], "\0") != 0 && check == -1)
        {
            j = m+1;
            check = -1;
            while (strcmp (name_station[j], "\0") != 0 && check == -1)
            {
                if (strcmp (name_station[m], name_station[j]) == 0 )
                    check = i;
                j++;
            }

            if (check != -1)
            {
                suppress_line(i, found, &decode, &result);
                i--;
            }

            m++;
        }

    }


    //we compare the decode and weight of every line to suppress any duplication
    i=0;
    while (found->nb_path>1 && i<(found->nb_path))
    {
        j = i+1;
        while (j<found->nb_path)
        {
            m=0;

            while (m != nb_station && decode[i][m] == decode[j][m])
                m++;

            if (m==nb_station && result[i] == result[j])
            {
                //there is a duplication, we supress the line in found, decode and result
                suppress_line(j, found, &decode, &result);
                j--;
            }

            j++;
        }
        i++;
    }
    *res = result;
    *dec = decode;

}

void display_infos (int **decode, int *result, int size_f)
{
    int i, j;

    printf ("\n\nresult : ");
    for (i=0; i<size_f; i++)
        printf ("%d - ", result[i]);

    printf ("\ndecode :  \n");
    for (i=0; i<size_f; i++)
    {
        for (j=0; j<nb_station; j++)
            printf ("%d - ", decode[i][j]);
        printf("\n");
    }
    printf ("\n");
}

void suppress_line (int cpt, Paths *found, int ***decode, int **result)
{
    int k, m;
    int **decode_realloc = *decode;
    int *result_realloc = *result;

    //suppress in found
    for (k=cpt; k<(found->nb_path)-1; k++)
    {
        for (m=0; m<nb_station; m++)
        {
            found->weight[k][m] = found->weight[k+1][m];
            found->predecessor[k][m] = found->predecessor[k+1][m];
        }
    }

    free (found->weight[(found->nb_path)-1]);
    free (found->predecessor[(found->nb_path)-1]);

    found->weight = (int **) realloc(found->weight,sizeof(int*)*(found->nb_path)-1);
    found->predecessor = (int**) realloc(found->predecessor,sizeof(int*)*(found->nb_path)-1);

    //suppress in result            SIZE DE RESULT = NB_PATH + 1 !!!!!
    for (k=cpt; k<(found->nb_path); k++)
        result_realloc[k] = result_realloc[k+1];

    result_realloc = (int *) realloc (result_realloc, sizeof(int)*(found->nb_path));
    if (result_realloc != NULL)
        *result = result_realloc;

    //suppress in decode
    for (k=cpt; k<(found->nb_path)-1; k++)
    {
        for (m=0; m<nb_station; m++)
            decode_realloc[k][m] = decode_realloc[k+1][m];
    }

    free (decode_realloc[found->nb_path-1]);

    decode_realloc = (int **) realloc (decode_realloc, sizeof (int*)* (found->nb_path)-1);

    if (decode_realloc != NULL)
        *decode = decode_realloc;

    (found->nb_path)--;
}
