#ifndef WEAKCLASSIFIER_H
#define WEAKCLASSIFIER_H

typedef struct WeakClassifier
{
  double Alpha;
  double Thresold;
  int parity;
  char *type;
  int fx;
  int fy;
  int fw;
  int fh;
} WeakClassifier;
