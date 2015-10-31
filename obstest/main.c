#include <gtk/gtk.h>
#include "GreyImage.h"
#include "StrongClassifier.h"
#include "Detector.h"
#include "Recon.h"
#include <stdio.h>


static void activate (GtkApplication *app, gpointer user_data)
{
	(void)(user_data);
	
	GtkWidget *window;

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (window), 1500, 1000);

	//StrongClassifier *newClassifier = StrongClassifierTrain("../misc/5000/");
	//StrongClassifierSave(newClassifier, "../misc/my_clas.data");
	//StrongClassifierPrint(newClassifier);
	

	
	
	GtkWidget *dialog = gtk_file_chooser_dialog_new("Open File",
													(GtkWindow *) window,
													GTK_FILE_CHOOSER_ACTION_OPEN,
													"Cancel",
													GTK_RESPONSE_CANCEL,
													"Open",
													GTK_RESPONSE_ACCEPT,
													NULL);

	GtkFileChooser *chooser  = GTK_FILE_CHOOSER(dialog);

	gint res = gtk_dialog_run(GTK_DIALOG(dialog));

	if(res == GTK_RESPONSE_ACCEPT)
	{
		char *imagePath = gtk_file_chooser_get_filename(chooser);
		gtk_widget_destroy(dialog);

		GtkWidget *image = gtk_image_new_from_file(imagePath);
		StrongClassifier *sc = StrongClassifierNewFromFile("../misc/my_clas.data");
		detectFaces((GtkImage *) image, sc);
		gtk_container_add (GTK_CONTAINER (window), image);
		}
	
	
	
	
	gtk_widget_show_all (window);
}




int main (int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}
