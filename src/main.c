#include <gtk/gtk.h>
#include "GreyImage.h"
#include <stdio.h>


static void activate (GtkApplication *app, gpointer user_data)
{
	(void)(user_data);
	
	GtkWidget *window;
	GtkWidget *image = gtk_image_new_from_file("../misc/photo.png");

	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "Window");
	gtk_window_set_default_size (GTK_WINDOW (window), 1500, 1000);


	//gtk_container_add (GTK_CONTAINER (window), image);

	GreyImage *greyImage = GreyImageNewFromImage((GtkImage *) image);
	greyImage = GreyImageScale(greyImage, 1.5);

	GtkWidget *newImage = (GtkWidget *) GreyImageToImage(greyImage);
	gtk_container_add(GTK_CONTAINER(window), newImage);


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
