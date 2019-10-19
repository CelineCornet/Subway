#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#include "create_problems.h"

void create_works (Line subways[size_lines], int infos [2][size_lines])
{
    Problem A;
    Station tmp;
    int i, r;
    FILE *input_file = fopen(input, "wb");
    fclose(input_file);
    srand(time(NULL));

    /*for (i = 0; i < nb_prob; i++)
	{
	    r = rand()%total_station;
	    tmp = return_station(r, subways, infos);
	    r = rand()%tmp.nb_line;

        if (tmp.tab[r].info == 0)
	    {
	        strcpy(A.name, tmp.name);
            A.line = tmp.tab[r].line;
            A.code = 1;
            Write(A);
	    }
	    else
            i --;
	}*/

	strcpy(A.name, "Montparnasse");
	A.line = 4;
    A.code = 1;
    Write(A);

	strcpy(A.name, "Raspail");
	A.line = 4;
    A.code = 1;
    Write(A);

    strcpy(A.name, "Denfert rochereau");
	A.line = 6;
    A.code = 1;
    Write(A);
}

Station return_station (int number, Line subways[size_lines], int infos [2][size_lines])
{
    int cpt, k;
    Station * found;

    for (cpt = 0; cpt < size_lines; cpt ++) // search in the lines declared
    {
        found = subways[cpt];// initialize the search to the beginning of each lines

        do
        {
            k = 0;
            if (number == 0)
                return *found;

            number = number - 1;

            while (found->tab[k].line != infos[0][cpt])
                k ++;

            found = found->tab[k].next;
        }while (found != NULL); //
	}

	return *subways[0];
}

void Write (Problem new)
{
	FILE * database_pro = fopen(input,"ab"); // open the file
	fwrite(&new, sizeof(Problem), 1, database_pro);
	fclose(database_pro); // close the file
}


void Read (StructGtk *S)
{
	Problem read;
	FILE * database_lines = fopen(input,"rb");
	char temp [500] = "";
	char temp2 [1000] = "";

    if (database_lines == NULL)
    {
        printf("File %s failed to open\n",input);
        exit(EXIT_FAILURE);
    }

    sprintf(temp, "<span font_family=\"Courier New\">Today we have those perturbations on the lines : </span>\n");
    strcat(temp2, temp);

	fread(&read, sizeof(Problem), 1, database_lines);
    while (feof(database_lines) == 0)
    {
        sprintf(temp,"\n\tStation <b> %s </b> -  line ", read.name);
        strcat (temp2, temp);

        color(read.line, temp);

        strcat (temp2, temp);
    	fread(&read, sizeof(Problem), 1, database_lines);
    }

    gtk_label_set_markup (S->disp_trav, temp2);

    fclose (database_lines);
}

void color(int line, char temp[500])
{
    if (line == 1)
            sprintf(temp,"<span foreground=\"#F8CB01\" weight = \"heavy\"><b> %d </b></span>", line);
    else if (line == 6)
             sprintf(temp,"<span foreground=\"#48D892\" weight = \"heavy\"><b> %d </b></span>", line);
    else if (line == 4)
            sprintf(temp,"<span foreground=\"#DB1EDB\" weight = \"heavy\"><b> %d </b></span>", line);
    else if (line == 9)
            sprintf(temp,"<span foreground=\"#C7EC95\" weight = \"heavy\"><b> %d </b></span>", line);
    else if (line == 7)
            sprintf(temp,"<span foreground=\"#F8A6DC\" weight = \"heavy\"><b> %d </b></span>", line);
}
