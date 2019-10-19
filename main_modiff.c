#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <time.h>

#include "initialization.h"
#include "matrix.h"
#include "find_journeys.h"
#include "comparison_paths.h"
#include "time_function.h"
#include "display.h"
#include "travaux.h"
#include "create_problems.h"

Line subways [size_lines];
int infos[2][size_lines];

void on_stop_clicked (GtkWidget *wid, StructGtk *S)
{
    gtk_widget_show (S->goodbye);
    gtk_widget_hide (S->menu);
    gtk_widget_hide (S->travaux);
    free_lists (subways, size_lines, infos);
}

void on_min_time_activate (GtkWidget *wid, StructGtk *S)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(S->min_time)))
        S->choice = 2;
    else S->choice = 1;
}

void on_min_corr_activate (GtkWidget *wid, StructGtk *S)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(S->min_corr)))
        S->choice = 1;
    else S->choice = 2;
}

void on_alone_destroy_event(GtkWidget * wid, StructGtk *S)
{
    gtk_widget_show(S->goodbye);
    gtk_widget_hide(S->alone);
    free_lists (subways, size_lines, infos);
}

void on_immediate_activate (GtkWidget *wid, StructGtk *S)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(S->now)))
        S->choice2 = 2;
    else S->choice2 = 1;
}

void on_later_activate (GtkWidget *wid, StructGtk *S)
{
    if (gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(S->soon)))
        S->choice2 = 1;
    else S->choice2 = 2;
}

static void on_ok_button_clicked (GtkWidget *wid, StructGtk *S)
{
    int matrix[node+2][node+2];
    int dep_time, wait_time;
    int i, station_1, station_2;
    int schedule[2][8];


    Station start;
    Station arrival;
    Station s[nb_station];

    const char *depart = ( const char *)malloc(sizeof( const char) *20);
    depart = gtk_entry_get_text((GtkEntry *) S->entryd);

    char * depart2 = (char * ) depart;

    const char *arrive = ( const char *)malloc(sizeof(  const char) *20);
    arrive = gtk_entry_get_text((GtkEntry *) S->entrya);

    char * arrive2 = (char * ) arrive;

    station_1 = search_station(depart2, &start, subways, infos);
    station_2 = search_station(arrive2, &arrival, subways, infos);

    if (strcmp(depart2, arrive2) == 0)
        station_1 = 3;

    if (display_state(station_1, station_2, S))
        return;

    // we free the variables we don't need anymore
    free ((char*) depart);
    free (depart2);
    free ((char*) arrive);
    free (arrive2);

    s[0] = start;
    s[nb_station-1] = arrival;

    stationCompare(subways,&start,&arrival, matrix, s, infos);

    //find paths
    Paths found;

    dijkstra(&found, matrix,s);

    //sort paths and find corresponding time
    int duration[found.nb_path];
    Parcours journeys[found.nb_path];


    if (S->choice2 != 1 && S->choice2 !=2)
        S->choice2 = 1;

    if (S->choice2 == 1)
    {
        dep_time = current_time();
        convert_time(dep_time, &S->hour, &S->min);
    }

    else
    {

        S -> hour = gtk_combo_box_get_active(S->h)   ;
        S -> min = gtk_combo_box_get_active(S->m)   ;
        (S -> hour) ++;

        dep_time = convert_second(S->hour,S->min);
    }

    wait_time = find_waiting_time(dep_time);// res is the waiting time for the metro
    printf("wait_time : %d\n", wait_time);

    if (wait_time == -1) /// SI WAIT TIME == - 1 lancer POP UP NEW SEARCH
    {
         gtk_widget_show(S->alone);
         gtk_widget_hide(S->window);
         gtk_widget_hide(S->travaux);
         return;
    }
    else
    {
        if (S->choice != 1 && S->choice != 2)
            S->choice = 1;

        if (S->choice == 1)                        // we sort according to the time of each journey
        {
            sort_paths(&found, journeys, 1, start, arrival, s);

            for (i = 0; i < found.nb_path; i++)
                duration[i] = time_calcul (journeys[i], dep_time);

            sort_by_time(duration, journeys,found.nb_path);
        }
        else                                    // we sort according to the nb of line we take
            sort_paths(&found, journeys, 2, start, arrival, s);

        //we display the result : the path + the duration or the nb of change
        //printf("WE ARRIVED TO THE DISPLAY\n");

        display_result (journeys, duration, s, found.nb_path, S->choice, S);

        gtk_widget_show (S->menu);
        gtk_widget_hide(S->window);

    }

    free_arrays (found.nb_path, found.predecessor, found.weight, journeys);

    //gtk_entry_set_text ((GtkEntry*)S->entrya, "");
    //gtk_entry_set_text ((GtkEntry*)S->entryd, "");
}

void on_new_clicked (GtkWidget * wid, StructGtk *S)
{
    gtk_widget_hide (S->results);
    gtk_widget_hide (S->menu);

    gtk_widget_show (S->window);

    //gtk_entry_set_text ((GtkEntry*)S->entrya, "");
    //gtk_entry_set_text ((GtkEntry*)S->entryd, "");
    //printf("coucou");

    gtk_main();
}

void on_exit_button_clicked (GtkWidget *wid, StructGtk *S)
{
    gtk_widget_show (S->goodbye);
    gtk_widget_hide (S->window);
    gtk_widget_hide (S->travaux);
    free_lists (subways, size_lines, infos);
}

void on_ok_warnings_clicked (GtkWidget *wid, StructGtk *S)
{
    gtk_widget_hide(S->warning);
}

void on_button2_clicked (GtkWidget * wid, StructGtk *S)
{
    gtk_label_set_text (S->disp_results, S->path2);
    gtk_widget_show (S->results);
    gtk_widget_hide (S->menu);
}

void on_button3_clicked (GtkWidget * wid, StructGtk *S)
{
    gtk_label_set_text (S->disp_results, S->path3);
    gtk_widget_show (S->results);
    gtk_widget_hide (S->menu);
}

void on_button1_clicked (GtkWidget * wid, StructGtk *S)
{
    gtk_label_set_text (S->disp_results, S->path1);
    gtk_widget_show (S->results);
    gtk_widget_hide (S->menu);
}


void on_close_clicked (GtkWidget * wid, StructGtk *S)
{
    gtk_widget_hide (S->results);
    gtk_widget_show (S->menu);
}

int main(int argc, char* argv[])
{
    //main

    gtk_init(&argc, &argv);
    StructGtk S;
    GtkBuilder * builder = gtk_builder_new();

    if (gtk_builder_add_from_file (builder, "glade4.glade", NULL) == 0)
    {
        fprintf (stderr, "Erreur: ouverture du fichier GLADE\n") ;
        exit(EXIT_FAILURE);
    }

    S.window = GTK_WIDGET(gtk_builder_get_object(builder, "main_window"));
    S.results = GTK_WIDGET(gtk_builder_get_object(builder, "results"));
    S.menu = GTK_WIDGET(gtk_builder_get_object(builder, "results1"));
    S.warning = GTK_WIDGET(gtk_builder_get_object(builder, "warnings"));
    S.travaux = GTK_WIDGET(gtk_builder_get_object(builder, "travaux"));
    S.goodbye = GTK_WIDGET(gtk_builder_get_object(builder, "Goodbye"));
    S.alone = GTK_WIDGET(gtk_builder_get_object(builder, "alone"));

    gtk_widget_realize (S.window);
    gtk_widget_realize (S.results);
    gtk_widget_realize (S.menu);
    gtk_widget_realize (S.warning);
    gtk_widget_realize (S.travaux);
    gtk_widget_realize (S.goodbye);
    gtk_widget_realize (S.alone);


    g_signal_connect (S.window, "destroy", gtk_main_quit, NULL);

    S.entryd = (GtkWidget *)gtk_builder_get_object (builder, "entry_departure");
    S.entrya = (GtkWidget *)gtk_builder_get_object (builder, "entry_arrival");


    S.disp_warning = GTK_LABEL (gtk_builder_get_object(builder, "disp_warnings"));
    S.disp_trav = GTK_LABEL (gtk_builder_get_object(builder, "trav"));
    S.disp_results = GTK_LABEL (gtk_builder_get_object(builder, "display_results"));
    S.disp_end = GTK_LABEL(gtk_builder_get_object(builder, "end"));
    S.label1_what = GTK_LABEL(gtk_builder_get_object(builder, "label1_what"));
    S.label2_what = GTK_LABEL(gtk_builder_get_object(builder, "label2_what"));
    S.label3_what = GTK_LABEL(gtk_builder_get_object(builder, "label3_what"));
    S.label1_line = GTK_LABEL(gtk_builder_get_object(builder, "label1_line"));
    S.label2_line = GTK_LABEL(gtk_builder_get_object(builder, "label2_line"));
    S.label3_line = GTK_LABEL(gtk_builder_get_object(builder, "label3_line"));
    S.what = GTK_LABEL(gtk_builder_get_object(builder, "what"));

    S.min_corr = gtk_check_button_new_with_label("Min corr");
    S.min_time = gtk_check_button_new_with_label("Min time");
    S.now = gtk_check_button_new_with_label("immediate");
    S.soon = gtk_check_button_new_with_label("later");


    S.h = GTK_COMBO_BOX(gtk_builder_get_object(builder,"hour"));
    S.m = GTK_COMBO_BOX(gtk_builder_get_object(builder,"minute"));

    g_signal_connect (gtk_builder_get_object(builder, "exit_button"), "clicked", G_CALLBACK( on_exit_button_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "ok_button"), "clicked", G_CALLBACK( on_ok_button_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "min_time"), "clicked", G_CALLBACK( on_min_time_activate), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "min_corr"), "clicked", G_CALLBACK( on_min_corr_activate), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "stop"), "clicked", G_CALLBACK( on_stop_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "new"), "clicked", G_CALLBACK( on_new_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "ok_warnings"), "clicked", G_CALLBACK( on_ok_warnings_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "immediate"), "clicked", G_CALLBACK( on_immediate_activate), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "later"), "clicked", G_CALLBACK( on_later_activate), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "button1"), "clicked", G_CALLBACK( on_button1_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "button2"), "clicked", G_CALLBACK( on_button2_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "button3"), "clicked", G_CALLBACK( on_button3_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "close"), "clicked", G_CALLBACK( on_close_clicked), (gpointer) &S);
    g_signal_connect (gtk_builder_get_object(builder, "alone"), "destroy", G_CALLBACK( on_alone_destroy_event), (gpointer) &S);

    g_object_unref(builder);

    tot_init(infos, subways, &S);

    gtk_widget_show(S.window);
    gtk_widget_show (S.results);
    gtk_widget_show (S.warning);
    gtk_widget_hide (S.warning);
    gtk_widget_hide (S.results);
    gtk_widget_show(S.travaux);
    gtk_widget_show(S.menu);
    gtk_widget_hide(S.menu);
    gtk_main();
    return 0;
}
