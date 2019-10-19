#ifndef INITIALIZATION_H_INCLUDED
#define INITIALIZATION_H_INCLUDED


#include <gtk/gtk.h>

typedef struct
{
    GtkWidget *window;
    GtkWidget *results;
    GtkWidget *warning;
    GtkWidget *travaux;
    GtkWidget *goodbye;
    GtkWidget *menu;
    GtkWidget *alone;
    GtkWidget *works;
    GtkWidget *see_lines;
    GtkWidget *line_1;
    GtkWidget *line_4;
    GtkWidget *line_6;
    GtkWidget *line_7;
    GtkWidget *line_9;

    GtkEntry *entryd;
    GtkEntry *entrya;

    int choice;
    int choice2;

    GtkWidget *min_time;
    GtkWidget *min_corr;

    GtkLabel *disp_warning;
    GtkLabel *disp_trav;
    GtkLabel *disp_results;

    GtkLabel * label1_what;
    GtkLabel * label2_what;
    GtkLabel * label3_what;
    GtkLabel * label1_line;
    GtkLabel * label2_line;
    GtkLabel * label3_line;
    GtkLabel * what;

    GtkComboBox *h;
    GtkComboBox *m;

    char path1[4000] ;
    char path2[4000] ;
    char path3[4000] ;

    GtkWidget *now;
    GtkWidget *soon;

    gint hour;
    gint min;

} StructGtk;



#define database "subway_lines.bin"
#define size_lines 5 //total number of lines


typedef struct pos
{
	int line;
	int code;
	struct station * prev;
	struct station * next;
	int info; //declare problems
}Position;

typedef struct station
{
	char name[50];
	int nb_line; // number of lines
	Position tab[5];
} Station;

typedef Station* Line;



void create_list (Line subways [size_lines], int infos[2][size_lines]);


void init_tab (int infos [2][size_lines], Line subways [size_lines]);


void insert_stations (Line subways[], Station *toinsert, int i, int infos[2][size_lines]);


void display_lines(Line todisplay);


void insert_top(Station *toinsert, Line *initializing);


Station* search_init(Station *cur, Line subways[], int i, int infos[2][size_lines]);


void insert_end (Station * tmp, Station *toinsert);


void insert_search (Station *tmp, Station *toinsert, Station *searched);


int search_station(char *name, Station *tofind, Line subways[], int infos[2][size_lines]);


char* keep_letters(char *tomodify);

char replace_accent(char c);


#endif // INITIALIZATION_H_INCLUDED
