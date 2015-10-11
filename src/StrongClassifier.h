#ifndef STRONGCLASSIFIER_H
#define STRONGCLASSIFIER_H
#include "WeakClassifier.h"

typedef struct StrongClassifier
{
  WeakClassifier *WeakClassifiers;
  double GlobalAplha;
} StrongClassifier;
