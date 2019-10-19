#include <stdlib.h>
#include <stdio.h>
#include "time_function.h"
#include "find_journeys.h"
#include "initialization.h"
#include "create_problems.h"
#include "display.h"
#include <string.h>

void display_result (Parcours *journeys, int duration[], Station s[nb_station], int size_j, int choice, StructGtk *S)
{
    int i, beg, disp;
    int nb, cpt;
    int h, min;
    char temp [4000] = "";
    char path[4000] = "";
    char temp2 [4000] = "";
    char path2 [10] = "";

    strcpy(S->path1, "");
    strcpy(S->path2, "");
    strcpy(S->path3, "");
    gtk_label_set_markup (S->label1_what, "");
    gtk_label_set_markup (S->label2_what, "");
    gtk_label_set_markup (S->label3_what, "");
    gtk_label_set_markup (S->label1_line, "");
    gtk_label_set_markup (S->label2_line, "");
    gtk_label_set_markup (S->label3_line, "");

    if (size_j > 3)
        disp = 3;
    else
        disp = size_j;

    if (choice == 2)
        gtk_label_set_markup(S->what, "Changes");


    for (i = 0; i < disp; i++)           //for each paths
    {
        //we convert the time of the path in hours and minutes
        if (choice ==1)
        {
            convert_time(duration[i], &h, &min);

            if (min < 10)
                sprintf (temp, "\n\n\tPath %d : %d h 0%d minutes\n\n", i+1, h, min);

            else
                sprintf (temp, "\n\n\tPath %d : %d h %d minutes\n\n", i+1, h, min);

            strcat(path, temp);

            sprintf(temp2, "%d h %d min", h, min);

            if ( i == 0 )  gtk_label_set_markup (S->label1_what, temp2);
            else if ( i == 1 )  gtk_label_set_markup (S->label2_what, temp2);
            else if ( i == 2 )  gtk_label_set_markup (S->label3_what, temp2);
        }
        else
        {
            //we find the nb of change
            nb = 0;
            cpt = 0;
            while (cpt < journeys[i].nb_chgt - 1)
            {
                if (journeys[i].way[cpt].line != journeys[i].way[cpt+1].line)
                    nb ++;
                cpt ++;
            }
            sprintf (temp, "\n\nPath %d : %d change(s)\n\n", i+1, nb);
            strcat(path, temp);

            sprintf(temp2, "%d change(s)", nb);

            if ( i == 0 )  gtk_label_set_markup (S->label1_what, temp2);
            else if ( i == 1 )  gtk_label_set_markup (S->label2_what, temp2);
            else if ( i == 2 )  gtk_label_set_markup (S->label3_what, temp2);
        }

        beg = 0;

        sprintf(temp, "\t     departure : %d h %d min\n\n", S->hour, S->min); //time of departure
        strcat(path, temp);

        do  //we display each sample
        {
            beg = display_sample (journeys[i].way,beg, journeys[i].nb_chgt, temp, path, path2);
        }while (beg != journeys[i].nb_chgt);

        ///fonction d'addition de temps !! :D


        sprintf(temp, "\t     arrival : "); //time of departure
        strcat(path, temp);
        arrival_time(temp, S->hour, S->min, h, min);
        strcat(path, temp);

        if ( i == 0 )  strcat (S->path1, path);
        else if ( i == 1 )  strcat (S->path2, path);
        else if ( i == 2 )  strcat (S->path3, path);

        strcpy(path, "");

        if ( i == 0 )  gtk_label_set_markup (S->label1_line, path2);
        else if ( i == 1 )  gtk_label_set_markup (S->label2_line, path2);
        else if ( i == 2 )  gtk_label_set_markup (S->label3_line, path2);
        strcpy (path2, "");
    }

    while (disp < 3)
    {
        if (disp == 1)
            sprintf(S->path2, "\n\tNothing - English\t\n\tNada - Español\t\n\tNéant - Français\t\n\tNeant - Română\t\n\tHiçlik - Türkçe\t\n\tNichts - Deutsch\t\n\tNulla - Italiano\t\n\tIngenting - Norsk\t\n\tНичто - Русский\t\n\tNüüß - Ripoarisch\t\n\tNic - Čeština\t\n\tWaxba - Soomaaliga\t\n\tΤίποτα - Ελληνικά\t\n\tништо - Македонски\t\n\thư vô - Tiếng Việt\t\n\t無 - 中文\n\t無 - 日本語\t\n\tไม่มีสิ่งใด - ไทย\t\n\tهیچ - فارسی\t\n\tاللا وجود - العربية\t\n");
        else if (disp == 2)
        {
            sprintf(S->path3, "\tYou want more details on nothing ?\t Here it is : \t\n\n\tNothing is a concept denoting the absence of something,\t\n\tand is associated with nothingness. \t\n\tIn nontechnical uses, nothing denotes things lacking importance,\t\n\tinterest, value, relevance, or significance. \n\tNothingness is the state of being nothing, \t\n\tthe state of nonexistence of anything,\t\n\tor the property of having nothing.\t\n");
        }
        disp++;
    }
}

int display_sample (Sample *way,int beg, int size_way, char temp[4000], char path[4000], char path2 [10])
{
    int j = beg;
    char temp2 [200] = "";

    while (j < size_way && way[beg].line == way[j].line)
        j++;

    color(way[beg].line, temp2);
    sprintf (temp, "            %s    ",temp2);
    strcat(path,temp);

    sprintf(temp, "%s", way[beg].departure.name);
    strcat(path, temp);
    sprintf(temp, "\t➙\t");
    strcat(path, temp);
    sprintf(temp, "%s\t\n                    %d stops,  direction  :  %s\t\n                    %dh %dmin\t\n\n", way[j-1].arrival.name, 1, find_direction(way[j-1].arrival,way[beg].departure,way[beg].line), 0, 0);
    strcat(path, temp);
    color(way[beg].line, temp2);
    strcat(path2, temp2);

    return j;
}

void sort_by_time (int *duration, Parcours *journeys, int size_tab)
{
    //  duration [i] is the time needed for the parcours journeys[i]

    int min = 0, temp, i, j;
    Parcours tmp;


    for (i = 0; i < size_tab; i++)
    {
        min = i;
        for (j = i; j < size_tab; j ++)
        {
            if (duration[min] > duration[j])
                min = j;
        }

        if (min != i)
        {
            tmp = journeys[i];
            journeys[i] = journeys[min];
            journeys[min] = tmp;
            temp = duration[i];
            duration[i]=duration[min];
            duration[min]=temp;
        }
    }

}

void free_arrays (int size_tab, int **tab1, int ** tab2, Parcours journeys[])
{
    int i;
    //we free the arrays of found and of journeys
    for (i = 0; i < size_tab; i++)
        free (tab1[i]);
    free(tab1);

    for (i = 0; i < size_tab; i++)
        free (tab2[i]);
    free(tab2);

    for (i=0; i < size_tab; i++)
        free(journeys[i].way);
}

void free_lists (Line * subways, int nb_line, int infos[2][size_lines])
{
    int i, j, line, k, l;
    Station *cur = NULL;

    for(i = 0; i < nb_line; i ++)
    {
        cur = *(subways + i);
        line = infos[0][i]; // line in progress of being deleted

        while (*(subways + i) != NULL)
        {
            cur = *(subways + i);

            /*NOUS ALLONS FREE UNE LISTE A LA FOIS EN REDIRIGEANT LES PREC DAUTRES LIGNES SUR LE SUIVANT DE LA STATION ACTUELLE*/
            if (cur->nb_line > 1)
            {
                for (j = 0; j < cur->nb_line; j ++)
                {
                    if (cur->tab[j].line != line && cur->tab[j].line != -1) //si ce n'est pas la ligne que nous freeons
                    {
                        if (cur->tab[j].prev != NULL && cur->tab[j].next != NULL)       //si pas la première ou la dernière station
                        {
                            k = 0;
                            while (cur->tab[j].prev->tab[k].line != cur->tab[j].line)           //on trouve la case de la ligne qu'on free dans la station précédente
                                k++;

                            cur->tab[j].prev->tab[k].next = cur->tab[j].next;       // la précédente pointe sur la suivante ( 1 case plus loin)

                            k = 0;
                            while (cur->tab[j].next->tab[k].line != cur->tab[j].line)           //on trouve la case de la ligne qu'on free dans la station précédente
                                k++;

                            cur->tab[j].next->tab[k].prev = cur->tab[j].prev;

                        }
                        else if (cur->tab[j].prev == NULL)
                        {
                            l = i;
                            while (cur->tab[j].line != infos [0][l]) // trouver la position de la ligne j dans le tableau subways
                                l ++;

                            *(subways + l) = cur->tab[j].next;
                        }
                    }
                    else if (cur->tab[j].line != -1)                //ligne qu'on free
                        *(subways + i) = cur->tab[j].next;
                }
            }
            else
                *(subways +i) = cur->tab[0].next;

            free(cur);
        }
    }
}


void arrival_time (char arrival[150], int hdep, int mdep, int htrip, int mtrip)
{
	int minute_time,minarr, harr;

	minute_time= (hdep + htrip)*60 + mdep + mtrip;

    harr = minute_time/60%60;
    minarr = minute_time - harr*60;

	sprintf (arrival, "%d h %d min",harr,minarr);
}
