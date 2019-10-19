#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "initialization.h"


void create_list (Line subways [size_lines], int infos[2][size_lines])
{
	FILE *database_stations = fopen (database, "rb");
	Station *read;
	int c, l;

	if (database_stations == NULL)
	{
		printf("File not found\n");
		exit(0);
	}

	for (c = 0; c < size_lines; c++)
	{

		for (l = 0; l < infos [1][c]; l++)
		{
		    read = (Station*)malloc(1*sizeof(Station));
			fread (read, sizeof(Station),1,database_stations);

            insert_stations (subways, read, c, infos);
		}
	}

	fclose (database_stations);
}

void init_tab (int infos [2][size_lines], Line subways [size_lines])
{
    int i;

	//intiialize each number of lines in the tab;
	infos [0][0] = 6;
	infos [0][1] = 7;
	infos [0][2] = 1;
	infos [0][3] = 4;
	infos [0][4] = 9;

	//initialize the numbers of stations in each line;
	infos [1][0] = 28;
	infos [1][1] = 33;
	infos [1][2] = 25;
	infos [1][3] = 27;
	infos [1][4] = 37;

	for (i = 0; i < size_lines; i++) // initialize pointers
        subways[i] = NULL;
}

void insert_stations (Line subways[], Station *toinsert, int i, int infos[2][size_lines])
{
    // i is the position of the current initialization.
	Station * tmp = subways[i];
	Station * searched;

	if (tmp == NULL) //if empty list
	    insert_top(toinsert, &subways[i]);

	else
	{
	    if (toinsert->nb_line == 1 || i == 0) //only one line going through the station
            insert_end(subways[i], toinsert);

	    else // for station having multiples lines
	    {
            searched = search_init(toinsert, subways,i, infos); // search earlier declaration

            if (searched == NULL) // if no previous declaration
                insert_end(subways[i], toinsert);
            else
                insert_search(subways[i], toinsert, searched);
	    }
	}
}

void insert_search (Station *tmp, Station *toinsert, Station *searched)
{
    int k = 0, j = 0;

    while (tmp->tab[j].line != toinsert->tab[0].line)
        j++;

    while (tmp->tab[j].next != NULL) // go to the end of the list
    {
    	tmp = tmp->tab[j].next; // the station changed into the next station of the same line
        j = 0;
        while (tmp->tab[j].line != toinsert->tab[0].line)
            j++;
    }


    while (searched->tab[k].line != -1) // find the case to put the data in
        k++;

    tmp->tab[j].next = searched;

    // Add informations of the new line in the station already declared
    searched->tab[k].line = toinsert->tab[0].line;
    searched->tab[k].code = toinsert->tab[0].code;
    searched->tab[k].next = NULL;
    searched->tab[k].prev = tmp;
    searched->tab[k].info = toinsert->tab[0].info;

}

Station* search_init(Station* cur, Line subways[], int i, int infos[2][size_lines])
{
    int cpt, k, line; //flag = 0;
    Station *found; // Station we want equal to current
    char *tocompare = (char*)malloc(50*sizeof(char));
    char *name = (char*)malloc(50*sizeof(char));

    name = keep_letters(cur->name);

    for (cpt = 0; cpt < i; cpt ++) // search in the 3 lines
	{
	    found = subways[cpt];// initialize the search to the beginning of each lines
        line = infos[0][cpt];

        do
        {
            k = 0;
            strcpy(tocompare, found->name);
            tocompare = keep_letters(tocompare);

            if (strcmp(tocompare, name)== 0)// earlier creation of the station found
                return found;

            while (found->tab[k].line != line)
                k++;

            found = found->tab[k].next;

        }while (found != NULL);//while (found->tab[flag].next != NULL);
	}

	return NULL; // Station not found -> not declared
}

void insert_top(Station *toinsert, Line *initializing)
{
	toinsert->tab[0].prev = NULL;
	toinsert->tab[0].next = NULL;
	*initializing = toinsert;
}


void display_lines(Line todisplay)
{
	Station *tmp = todisplay;
	int line_dis = todisplay->tab[0].line;
	int k;

	printf("line display : %d\n", line_dis);

	if (tmp == NULL)
		printf("Can't display the linked list\n");

	while (tmp != NULL)
	{
	    printf("Name of the station : %s, code : %d\n",tmp->name, tmp->tab[0].info);
	    k = 0;

        while (tmp->tab[k].line != line_dis && k < tmp->nb_line)
            k++;

		tmp = tmp->tab[k].next;
	}
}

void insert_end (Station * tmp, Station *toinsert)
{
    int j = 0;

    while (tmp->tab[j].line != toinsert->tab[0].line)
        j++;

    while (tmp->tab[j].next != NULL)
    {
    	tmp = tmp->tab[j].next; // the station changed into the next station of the same line
        j = 0;
        while (tmp->tab[j].line != toinsert->tab[0].line)
            j++;
    }

    tmp->tab[j].next = toinsert;
    toinsert->tab[0].next = NULL;
    toinsert->tab[0].prev = tmp;
}


int search_station(char *name,Station *tofind, Line subways[], int infos[2][size_lines])
{
    int cpt, k, flag = 0, tmp;
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
            {
                *tofind = *found;
                tmp = 0;
                for (k = 0; k < found->nb_line; k++)
                {
                    if (found->tab[k].info != 0)
                        tmp ++;
                }
                if (tmp == found->nb_line)
                    return 2; // return 2 if there is a problem on the entire station
                return 1;
            }

            for (k = 0; k < found->nb_line; k++)
            {
                if (found->tab[k].line == infos[0][cpt])
                    flag = k;
            }

            found = found->tab[flag].next;

        }while (found != NULL); //
	}

	return 0; // Station not found
}

char* keep_letters(char *name)
{
    int i, j, size;
    char *tomodify = (char*)malloc(50*sizeof(char));

    size = strlen(name);

    for (i = 0; i < size; i++)
        name[i] = replace_accent(name[i]);

    strcpy(tomodify,name);
    strlwr(tomodify);

    for(i = 0; tomodify[i] != '\0'; i++)
    {
        while (!( (tomodify[i] >= 'a' && tomodify[i] <= 'z') || tomodify[i] == '\0') )
        {
            for(j = i; tomodify[j] != '\0'; ++j)
            {
                tomodify[j] = tomodify[j+1];
            }
            tomodify[j] = '\0';
        }
    }

    return tomodify;
}

char replace_accent(char c)
{
	if (c == - 126 || c == - 118 || c == - 120)
	    c = 'e';

	else if (c == - 123 || c == - 125)
		c = 'a';

	else if (c == - 116 || c == - 117)
		c = 'i';

	else if (c == -121)
		c = 'c';

	else if (c == - 105 || c == - 106 || c == - 127)
		c = 'u';

    return c;
}
