#ifndef DEF_STRONGCLASSIFIER
#define DEF_STRONGCLASSIFIER

#include "WeakClassifier.h"

struct StrongClassifier
{
	int size;
	WeakClassifier **wcs;
	double alphaTotal;
};
typedef struct StrongClassifier StrongClassifier;

StrongClassifier *StrongClassifierNew(int size, WeakClassifier **wcs);
StrongClassifier *StrongClassifierNewFromFile(char *path);
void StrongClassifierFree(StrongClassifier *sc);

int StrongClassifierCheck(StrongClassifier *cs, IntegralImage *image, int x, int y, double scale, int deviation);

#endif
