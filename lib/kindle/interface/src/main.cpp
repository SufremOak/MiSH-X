#include <gtk-2.0/gtk/gtk.h>

int main(int argc, char* argv[]) {
  GtkWidget *window;
    
  gtk_init (&argc, &argv);
  
  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_window_set_title(GTK_WINDOW(window), "L:A_N:application_ID:org.kindlemodding.example-gtk-application_PC:T");

  g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

  gtk_widget_show(window);
  
  gtk_main();

  return 0;
}
