#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include "WeakClassifier.h"
#include "StrongClassifier.h"

int* Detect(GtkImage *img, const char pathClass[])
{
  FILE *fp;
  fp = fopen(pathClass, "r");
  float alpha,threshold;
  int parity,fx,fy,fw,fh;
  char type[60];
  int i = 0;
  StrongClassifier Classifier;
  WeakClassifier weaks;
  Classifier.WeakClassifiers = malloc(400 * sizeof weaks);
  while(fscanf(fp,"%e;%e;%d;%s;%d;%d;%d;%d\n", &alpha, &threshold, &parity, type, &fx, &fy, &fw, &fh) != EOF)
  {

    weaks = (WeakClassifier) {(double)alpha, (double)threshold, parity, type, fx, fy, fw, fh};
    Classifier.WeakClassifiers[i] = weaks;    
    i++;
  }
  fclose(fp);
}


