#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>

static void destroy(GtkWidget *widget, gpointer data)
{
    gtk_main_quit ();
}



static void initialize_window(GtkWidget* window)
{
  gtk_window_set_title(GTK_WINDOW(window),"My Window"); //Set window title
  gtk_window_set_default_size (GTK_WINDOW (window), 640, 400); //Set default size for the window
  g_signal_connect (window, "destroy", G_CALLBACK (destroy), NULL); //End application when close button clicked

}

int main (int argc, char *argv[])
{
  GtkWidget *window,*table,*label,*entry;
  gtk_init(&argc, &argv);


  //Create the main window
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  initialize_window(window);


   /* Create a 1x2 table */
   table = gtk_table_new (5, 2, TRUE);
   gtk_container_add (GTK_CONTAINER (window), table);

   /* Fix the Spacing - FUCKED UP */ 
   gtk_table_set_row_spacing (GTK_TABLE(table),1,20);
   gtk_table_set_col_spacing (GTK_TABLE(table),1,20);
   
   gtk_table_set_row_spacing (GTK_TABLE(table),2,20);
   gtk_table_set_col_spacing (GTK_TABLE(table),2,20);

   gtk_table_set_row_spacing (GTK_TABLE(table),3,20);
   gtk_table_set_col_spacing (GTK_TABLE(table),3,20);

  /* create a new label. */
  label = gtk_label_new("R1" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 0, 1);

  label = gtk_label_new("R2" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 1, 2);

  label = gtk_label_new("R3" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);

  label = gtk_label_new("R4" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 2, 3);

  label = gtk_label_new("R5" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 3, 4);

  label = gtk_label_new("SP" );
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 4, 5);

  label = gtk_label_new("PC");
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 5, 6);

  label = gtk_label_new("NZVC");
  gtk_table_set_homogeneous(GTK_TABLE (table), TRUE);
  gtk_table_attach_defaults (GTK_TABLE (table), label, 0, 1, 6, 7);
  
  
  
  //create a text box
  //entry = gtk_entry_new ();
  //gtk_entry_set_max_length (GTK_ENTRY (entry),0);
  //gtk_table_attach_defaults (GTK_TABLE (table), entry, 0, 1, 0, 1);


    gtk_widget_show_all(window);

  gtk_main ();
  return 0;
}
