#ifndef DEF_DETECTOR
#define DEF_DETECTOR

#include <gtk/gtk.h>
#include "StrongClassifier.h"

struct DetectedFace
{
	int x;
	int y;
	int w;
	int h;
};
typedef struct DetectedFace DetectedFace;

void detectFaces(GtkImage *image, StrongClassifier *sc);


#endif
