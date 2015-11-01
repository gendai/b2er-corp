#include <gtk/gtk.h>
#include "GreyImage.h"
#include "StrongClassifier.h"
#include "Detector.h"
#include "Recon.h"
#include <stdio.h>
#include <string.h>


static void activate (GtkApplication *app, gpointer user_data)
{
	(void)(user_data);
	
	GtkWidget *window;

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (window), 1500, 1000);
	
	
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
		StrongClassifier *sc = StrongClassifierNewFromFile("../misc/clas.data");
		detectFaces((GtkImage *) image, sc);
		gtk_container_add (GTK_CONTAINER (window), image);
	}
	
	
	
	
	gtk_widget_show_all (window);
}


static int startDetection(char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.example", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), 1, argv);
	g_object_unref (app);

	return status;
}

static int startDatabse()
{
	return 0;
}

int main (int argc, char **argv)
{
	if(argc < 2)
	{
		printf("Error: undefined action\n");
		return 0;
	}

	char *action = argv[1];

	if(!strcmp(action, "detection"))
		return startDetection(argv);
	else if(!strcmp(action, "database"))
		return startDatabse();


	printf("Error: Unknown action %s\n", action);
	return 1;
}
