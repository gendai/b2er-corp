#include <gtk/gtk.h>
#include "GreyImage.h"
#include "StrongClassifier.h"
#include "Detector.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "database.h"


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
  char *wanted = malloc(100);
  char *wanted2 = malloc(100);
  char *wanted3 = malloc(100);
  char *number = malloc(100);
  char *name = malloc(100);
  char *filename = malloc(100);
  for(;;)
  {
    printf("What do you want to do ? Write it\nDelete ?\nAdd ?\nDisplay?\nChange a name?\nQuit?\n");
    scanf("%s",wanted);
    if(strcmp(wanted,"quit")==0)
    {
      exit(0);
    }
    if(strcmp(wanted, "change") == 0)
    {
      printf("Give me the old name.");
      scanf("%s", wanted2);
      printf("Give me the new name.");
      scanf("%s", wanted3);
      if (search_id(wanted2)!=0)
      {
        change_name(wanted2,wanted3);
	printf("The name %s was changed to %s!\n", wanted2, wanted3);
      }
      else
      {
	printf("The name %s was not found!\n", wanted2);
      }
    }
    if(strcmp(wanted, "display") == 0)
    {
      printf("What do you want to display ?\nNames ?\n");
      scanf("%s",wanted2);
      if(strcmp(wanted2, "names") == 0)
        display_name();
    }
    else
    {
      if(strcmp(wanted, "delete") == 0)
      {
        printf("What do you want to delete?\nImages ?\n People ?\n");
        scanf("%s",wanted2);
        if(strcmp(wanted2, "images") == 0)
        {
          printf("Give me the name.\n");
          scanf("%s", wanted3);
          delete_img(wanted3);
        }
        if(strcmp(wanted2, "people") == 0)
        {
          printf("Give me the name.\n");
          scanf("%s", wanted3);
	  if (search_id(wanted3)!=0)
            {
              delete_person(wanted3);
            }
          else
          {
	      printf("The name %s was not found! \n", wanted3);
          }
        }
      }
      else
      {
        if(strcmp(wanted, "add") == 0)
        {
          printf("What do you want to add?\nImages ?\nPeople ?\n");
          scanf("%s", wanted3);
          if(strcmp(wanted3, "images") == 0)
          {
            printf("Give me the name of the guy\n");
            scanf("%s", name);
            int id = search_id(name);
            if (id!=0)
            {
        
              printf("Write the different names of the files separated by a -\n");
              scanf("%s", number);
            }
            else
            {
	      printf("The name %s was not found!\n", name);
            }
            while(*number != 0)
            {
              for(int i = 0; *number != '-' && *number != 0; i++)
              {
                *(filename+i) = *number;
                number++;
              }
              add_img(filename, id);
              for(int i = 0; *(filename+i) != 0; i++)
                 *(filename+i) = 0;
              number++;
            }
          }
          if(strcmp(wanted3, "people") == 0)
          {
           printf("Write the different names of the files separated by a : and every people by -\nExample : nico.jpg:nico-steven.jpg:steven\n");
            scanf("%s", number);
            while(*number != 0)
            {
              for(int i = 0; *number != ':' && *number != 0; i++)
              {
                *(filename+i) = *number;
                number++;
              }
              number++;
              for(int j = 0; *number != '-' && *number != 0; j++)
              {
                *(name+j) = *number;
                number++;
              }

                add(filename, name);
              
	      for(int i = 0; *(filename+i) != 0; i++)
                 *(filename+i) = 0;
              for(int j = 0; *(name+j) != 0; j++)
                 *(name+j) = 0;
              number++;
            }
          }
        }
      }
    }
  }
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
